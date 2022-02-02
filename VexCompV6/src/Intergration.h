#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
//Intergrates phyical and virtual components

#include "motorFunctions.h"
#include "odom/robot.h"

//CONSTANTS


//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const Point startingPoint = Point(0, 0);
const double startingHead = 90; //Degrees (CCW +)

const double UnitsPerRev = 18.3207546*1.0167034; //Inches per revolution
const double RobotDiameter = 15; //Inches (Same Units as above)
const double RobotRadius = 0.5*RobotDiameter;

const double updateTime = 20; //msec
const double motionDelay = 25; //msec
const double cameraDelay = 20;

const double linThreashold = 1; //In
const double angularThreashold = degToRad(1);
const double maximumAccelerationLinear = 1000;
const double maximumAngularAcceleration = 1000;
const double maxVelocity = 1000;
const double maxAngularVelocity = 1000;
const double updateTargetHeadingMinThreashold = 5;
const double maxThetaErrorForMotion = 15; //deg
bool maxThetaErrorForMotionGivenInDegrees = true;

//linGains, rotGains
const PIDGains basicGains[3][2] = {
  {{1.8,0,0}, {7.42,0,0.2}}, //1 towers
  {{0,0,0}, {0,0,0}}, //2 tower
  {{0,0,0}, {0,0,0}} //3 tower
};
const PIDGains cameraGains[2][2] = {
  {{0.7,0,0}, {0.45,0,0}}, //0 towers
  {{0,0,0}, {0,0,0}} //1 tower
};


typedef struct{
  int xOffset;
  int yOffset;
  int xThreashold;
  int yThreashold;
  int minWidth;
} CameraSettings;
const CameraSettings frontCameraSettings = {153, 140, 15, 15, 30};
const CameraSettings backCameraSettings = {153, 140, 15, 15, 30};
const int camStopWait = 500; //msec
const int camDelayLoop = 20; //msec

Graph graph = Graph("tracker.txt", &Brain);

///Robot Instantation
Robot robot = Robot(startingPoint, startingHead, true, 
                    basicGains[0][0], basicGains[0][1], 
                    linThreashold, angularThreashold, 
                    maxVelocity, maxAngularVelocity, 
                    maximumAccelerationLinear, maximumAngularAcceleration, 
                    updateTargetHeadingMinThreashold, maxThetaErrorForMotion, maxThetaErrorForMotionGivenInDegrees);

void track(){
  static double lastHeading = 0;
  static double lastTime = 0;
  static double lastLeft = 0;
  static double lastRight = 0;

  double leftEnc = getLeftEnc();
  double rightEnc = getRightEnc();
  double head = Inertial.angle(); //Gets unbounded angle; Postive in CW direction
  double deltaT = Brain.timer(timeUnits::msec) - lastTime;
  deltaT = deltaT / 1000;

  double deltaHead = lastHeading - head; //+ is CCW; Heading given + in CW so sign is flopped
  lastHeading = head;
  head = 360 - normalizeAngle(head, false); //Heading in CCW + direction

  double deltaFwd = 0.5*((leftEnc-lastLeft) + (rightEnc-lastRight))*UnitsPerRev; //TODO Breaks while turning

  lastLeft = leftEnc;
  lastRight = rightEnc;

  Vector deltaPos = Vector(deltaFwd, head, true);
  robot.updatePos(deltaT, deltaPos, deltaHead, true);
  robot.setHead(head, true);
  robot.updateVelocity(deltaT);
  robot.updateStopStatus(deltaT);

  lastTime = Brain.timer(timeUnits::msec);
}

int frame = 0;

int trakerFunction(){
  while(true){
    track();

    if(frame >= 10){
      #if 0
        Vector tVec = robot.location.getTargetVector();
        graph.addPoint({robot.location.pos, "green"});
        graph.addPoint({robot.location.targetPos, "blue"});
        graph.addVector({robot.location.pos, tVec, "teal"});
        graph.addVector({robot.location.pos, Vector(1, robot.location.getTargetHead(), false).scale(10), "yellow"});
        graph.addVector({robot.location.pos, robot.location.getRobotBasisVector().scale(10), "red"}); 

        graph.addPID({robot.getLinearErrorForPID(), robot.linearPid, robot.getLinearSpeedTarget(), robot.location.getVel().dot(robot.location.getRobotBasisVector())}, true);
        graph.addPID({robot.getThetaError(), robot.rotationalPid, robot.getRotationalSpeedTarget(), robot.location.getAngularVel()}, false);

        std::cout << graph.getString() << std::flush;

        graph.clear();
      #endif
      frame = 0;
    }else{
      frame = frame + 1;
    }

    wait(updateTime, msec);
  }
} //Called in Pre-Auton

bool updateMotors(double overide=0, bool overideLinear=true){
  double linearSpeed = robot.getLinearSpeedTarget();
  double angularSpeed = robot.getRotationalSpeedTarget();
  linearSpeed = (linearSpeed/UnitsPerRev)*60; //Units/Sec to RPM
  angularSpeed = angularSpeed * (RobotRadius/UnitsPerRev) * 60 * 0.5; //Converts Rad/S to RPM and splits in half because there are 2 drive sides
  setLeft(linearSpeed - angularSpeed, velocityUnits::rpm);
  setRight(linearSpeed + angularSpeed, velocityUnits::rpm);

  if(robot.isMoving()){
    return true;
  }else{
    setLeft(0);
    setRight(0);
    return false;
  }
}



//MOTION FUNCTIONS

void executeMove(){
  wait(updateTime+1, msec);
  bool inMotion = robot.isMoving();

  while(inMotion){
    inMotion = updateMotors();
    wait(motionDelay, timeUnits::msec);
  }
}

//Moves realitive to current position using robot orientation
//Rotates realitive to target
void turn(double theta, bool inDeg=true, bool blocking=true){
  robot.setRotateMode();
  robot.setTargetHead(theta, inDeg);
  if(blocking){
    executeMove();
  }
}
//Turns to abs orientation
void turnTo(double theta, bool inDeg=true, bool blocking=true){
  robot.setHeadTargetAbs(theta, inDeg);
  turn(0, false, blocking);
}

void moveAbs(double x, double y, bool fwd=true, bool blocking=true){
  Point p = robot.location.getPos();
  double d = 0;
  if(!fwd){
    d = PI;
  }
  turnTo(Vector(1, 0).getAngle(Vector(x-p.x, y-p.y)) + d, false);

  robot.setLineMode(fwd);
  robot.setAbsTarget(x, y);
  if(blocking){
    executeMove();
  }
}
void moveAbs(Point p, bool fwd=true, bool blocking=true){
  moveAbs(p.x, p.y);
}
void move(Vector v, bool fwd=true, bool blocking=true){
  robot.setLineMode(fwd);
  robot.setTarget(v);
  if(blocking){
    executeMove();
  }
}
void move(double fwd, double hor, bool dir=true, bool blocking=true){
  move(Vector(hor, fwd), dir, blocking);
}
void move(double mag, double theta, bool inDeg, bool dir, bool blocking=true){
  move(Vector(mag, theta, inDeg), dir, blocking);
}

void moveCV(double fwd, double hor, double linearSpeedTarget){
  bool dir = true;
  double d = 0;
  if(linearSpeedTarget < 0){
    dir = false;
    d = PI;
  }

  turnTo(Vector(1, 0).getAngle(Vector(hor, fwd)) + d, false);
  robot.setLineMode(dir, false);
  robot.usePIDControls(false, true);
  robot.setTarget(Vector(hor, fwd));
  wait(updateTime+1, msec);
  
  while(abs(robot.getLinearErrorForPID()) > robot.linearThreshold){
    double angularSpeed = robot.getRotationalSpeedTarget();
    double linearSpeed = (linearSpeedTarget/UnitsPerRev)*60; //Units/Sec to RPM
    angularSpeed = angularSpeed * (RobotRadius/UnitsPerRev) * 60 * 0.5; //Converts Rad/S to RPM and splits in half because there are 2 drive sides

    setLeft(linearSpeed - angularSpeed, velocityUnits::rpm);
    setRight(linearSpeed + angularSpeed, velocityUnits::rpm);
    wait(motionDelay, timeUnits::msec);
  }

  setLeft(0);
  setRight(0);
}


//Tracing functions
void tracePath(smartPointPointer &points, double vel=20){
  robot.traceMode(points, vel);
  executeMove();
}



//Camera functions

//Function returns a Vector objects repersenting the error in camera pixels that the biggest object with a given signature is from the shifted center of the robot
Vector getErrorFromCamera(vex::vision *cam, vex::vision::signature &sig, CameraSettings camsett){
  int num = cam->takeSnapshot(sig);
  double x = 0;
  double y = 0;

  if(num > 0){
    //If an object exists
    if(cam->largestObject.width > camsett.minWidth){
      //If an object is real and not some random color it spotted
      x = cam->largestObject.centerX - camsett.xOffset; //X offset
      y = cam->largestObject.centerY - camsett.yOffset; //Y offset
      x = x*-1; //Flip x and y values to change camera coornates to match with robot orientation
      y = y*-1;
    }
  }
  return Vector(x, y); //Return a new vector object
}

//Sets motor values directly proporional to the value of error determined by the camera; Lines robot up with object while moving torward it
//Example: trackWithCam(&BackCam, -1, backCameraSettings, 0, BackCam__YELLOWGOAL);
void trackWithCam(vex::vision *camera, int d, CameraSettings settings, const int gainsIndex, vex::vision::signature &sig){
  PIDOutput linValues = {0,0,0}; //Storage structs for illitertive PID
  PIDOutput rotValues = {0,0,0};
  int rotMove = 0; //Time spent within threashold range
  int linMove = 0;

  robot.usePIDControls(false); //Disable continous PID update for primary robot tracking so an extended break from 
  while((rotMove<camStopWait) || (linMove<camStopWait)){
    //While time spent within threasHold values is less than minimum time within threashold than the robot is not considered fully stopped
    Vector r = getErrorFromCamera(camera, sig, settings); //Error values

    linValues = PID(r.getY(), camDelayLoop/1000.0, cameraGains[gainsIndex][0], linValues); // Get motor values using PID algorithm (error, deltaTime, proporionality constant values, previous PID info)
    rotValues = PID(r.getX(), camDelayLoop/1000.0, cameraGains[gainsIndex][1], rotValues);

    setLeft(d*linValues.output - rotValues.output); //Speed = direction*speedBasedOnError - rotationalSpeed Value (So + rotSpeed is turning the robot CCW)
    setRight(d*linValues.output + rotValues.output);

    if(abs(r.getX()) < settings.xThreashold){
      //If within threashold add delay time to the counter; If not within threashold set counter to zero; Only considers robot stopped if the robot has spent signifgant time within its threashold boundries
      rotMove += camDelayLoop;
    }else{
      rotMove = 0;
    }
    if(abs(r.getY()) < settings.yThreashold){
      linMove += camDelayLoop;
    }else{
      linMove = 0;
    }
    wait(camDelayLoop, msec);
  }
  //Once within threashold and stopped activate motor breaks and end function
  setLeft(0);
  setRight(0);
}

void trackWithCam(vex::vision *camera, int d, CameraSettings settings, const int gainsIndex, vex::vision::signature &sig, const double setLinSpeed){
  PIDOutput rotValues = {0,0,0};
  int rotMove = 0;
  int linMove = 0;

  robot.usePIDControls(false);
  while((rotMove<camStopWait) || (linMove<camStopWait)){
    Vector r = getErrorFromCamera(camera, sig, settings);

    rotValues = PID(r.getX(), camDelayLoop/1000.0, cameraGains[gainsIndex][1], rotValues);

    setLeft(d*setLinSpeed - rotValues.output);
    setRight(d*setLinSpeed + rotValues.output);

    if(abs(r.getX()) < settings.xThreashold){
      rotMove += camDelayLoop;
    }else{
      rotMove = 0;
    }
    if(abs(r.getY()) < settings.yThreashold){
      linMove += camDelayLoop;
    }else{
      linMove = 0;
    }
    wait(camDelayLoop, msec);
  }
  setLeft(0);
  setRight(0);
}

#endif