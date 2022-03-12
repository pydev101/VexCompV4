#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
/*
Name: Intergration.h
Written By: Carson Easterling

Connects the motor and phycical characteritics of the robot to the abstracted math and algorithms into usable move function
*/

#include "motorFunctions.h"
#include "odom/robot.h"

//CONSTANTS


//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const Point startingPoint = Point(0, 0);
const double startingHead = 90; //Degrees (CCW +)

const double UnitsPerRev = 18.3207546*1.0167034; //Inches per revolution
const double RobotDiameter = 15; //Inches (Same Units as above)
const double RobotRadius = 0.5*RobotDiameter;

const double updateTime = 10; //msec
const double motionDelay = 15; //msec
const double cameraDelay = 20;

const double linThreashold = 1; //In
const double angularThreashold = degToRad(1);

const double maxVelocity = 100;
const double maxAngularVelocity = 100;

const double updateTargetHeadingMinThreashold = 5;
const double maxThetaErrorForMotion = 90; //15; //deg
bool maxThetaErrorForMotionGivenInDegrees = true;

const int armIntakeActivationThreshold = 400;

//linGains, rotGains

//PID Values used in move down state
const PIDGains linGain = {7,2,0}; //I is treated like a constant
const PIDGains rotGain = {30,0,0}; //53

//PID values moved in move up state
const PIDGains linGainReverse = {5,8,0}; //I is constant
const PIDGains rotGainReverse = {40,5,0}; //I is constant


/*
const PIDGains cameraGains[2][2] = {
  {{0.7,0,0}, {0.45,0,0}}, //0 towers
  {{0,0,0}, {0,0,0}} //1 tower
};*/


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

const char fileName[] = "Hello.txt";
//Graph graph(fileName);
PythonProgramLogger pythonLog(fileName);

///Robot Instantation
Robot robot = Robot(startingPoint, startingHead, true, 
                    linGain, rotGain,
                    linGainReverse, rotGainReverse,
                    linThreashold, angularThreashold, 
                    maxVelocity, maxAngularVelocity, 
                    updateTargetHeadingMinThreashold, maxThetaErrorForMotion, maxThetaErrorForMotionGivenInDegrees);


//End of user defined constants


//Threaded tracking function to update robot positon and target output
void track(){
  static double lastHeading = 0;
  static double lastTime = 0;
  static double lastLeft = 0;
  static double lastRight = 0;

  double leftEnc = getLeftEnc();
  double rightEnc = getRightEnc();
  double head = getHeadingUnbounded(); //Gets unbounded angle; Postive in CW direction
  double deltaT = Brain.timer(timeUnits::msec) - lastTime;
  deltaT = deltaT / 1000;

  double deltaHead = lastHeading - head; //+ is CCW; Heading given + in CW so sign is flopped
  lastHeading = head;
  head = 360 - normalizeAngle(head, false); //Heading in CCW + direction

  double deltaFwd = 0.5*((leftEnc-lastLeft) + (rightEnc-lastRight))*UnitsPerRev;

  lastLeft = leftEnc;
  lastRight = rightEnc;


  //Update robot class
  Vector deltaPos = Vector(deltaFwd, head, true);
  robot.updatePos(deltaT, deltaPos, deltaHead, true);
  robot.setHead(head, true); //Set heading using more accurate inertial senso
  robot.updateVelocity(deltaT);
  robot.updateStopStatus(deltaT);


  //Activate intake automatically based on arm position
  static int lastArmPos = arm.position(rotationUnits::deg);
  int currArmPos = arm.position(rotationUnits::deg);
  if(currArmPos >= armIntakeActivationThreshold){
    if(lastArmPos < armIntakeActivationThreshold){
      setIntake(-50);
    }
  }else{
    if(armIntakeActivationThreshold < lastArmPos){
      setIntake(0);
    }
  }
  lastArmPos = currArmPos;

  lastTime = Brain.timer(timeUnits::msec);
}

int frame = 0;

//Thread for update cycle, also contains logging code for robot motion for later analysis with python tools
int trakerFunction(){
  while(true){
    track();

    //std::cout << robot.location.getCurrHead() << ", " << robot.location.getTargetHead() << std::endl;

    #if 1
      if(frame >= 10){
        Vector tVec = robot.location.getTargetVector();
        pythonLog.addPoint(robot.location.pos, "green");
        pythonLog.addPoint(robot.location.targetPos, "blue");
        pythonLog.addVector(robot.location.pos, tVec, "teal");
        pythonLog.addVector(robot.location.pos, Vector(1, robot.location.getTargetHead(), false).scale(10), "yellow");
        pythonLog.addVector(robot.location.pos, robot.location.getRobotBasisVector().scale(10), "red");
        pythonLog.graph();
        pythonLog.print();
        frame = 0;
      }else{
        frame = frame + 1;
      }
    #endif

    wait(updateTime, msec);
  }
} //Activated in preauton


//Update motor outputs based on robot class output
bool updateMotors(double overide=0){
  double linearSpeed = robot.getLinearSpeedTarget();
  if(overide != 0){
    linearSpeed = overide;
  }
  double angularSpeed = robot.getRotationalSpeedTarget();
  
  double left = linearSpeed - angularSpeed;
  double right = linearSpeed + angularSpeed;

  if(abs(right) > maxVelocity){
    left = left - (abs(right) - maxVelocity)*sign(left);
    right = maxVelocity*sign(right);
  }
  if(abs(left) > maxVelocity){
    right = right - (abs(left) - maxVelocity)*sign(left);
    left = maxVelocity*sign(left);
  }

  setLeft(left, velocityUnits::pct);
  setRight(right, velocityUnits::pct);

  if(robot.isMoving()){
    return true;
  }else{
    setLeft(0);
    setRight(0);
    return false;
  }
}



//MOTION FUNCTIONS

//Block until robot arrives at target position
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

//Moves to absolute coordnate location
void moveAbs(double x, double y, bool fwd=true, bool blocking=true){
  //Point p = robot.location.getPos();
  double d = 0;
  if(!fwd){
    d = PI;
  }
  //turnTo(Vector(1, 0).getAngle(Vector(x-p.x, y-p.y)) + d, false);

  robot.setLineMode(fwd);
  robot.setAbsTarget(x, y);
  if(blocking){
    executeMove();
  }
}
void moveAbs(Point p, bool fwd=true, bool blocking=true){
  moveAbs(p.x, p.y);
}

//Moves realitive to last target location point
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

//Moves to target location with constant linear velocity
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
    updateMotors(linearSpeedTarget);
    wait(motionDelay, timeUnits::msec);
  }

  setLeft(0);
  setRight(0);
}
void moveCV(Vector v, double linearSpeedTarget){
  moveCV(v.getY(), v.getX(), linearSpeedTarget);
}


//Follow a generated path
void tracePath(smartPointPointer &points, double vel=20){
  if(points.size >= 2){
    moveAbs(points[0]);
    turnTo(Vector(1, 0).getAngle(Vector(robot.location.getPos(), points[1])), false);
    robot.traceMode(points, vel);
    executeMove();
  }
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

/*
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

  double t = Brain.timer(timeUnits::msec);
  while(((Brain.timer(timeUnits::msec) - t) <= 700) && (!LimitBack.pressing())){
    setLeft(20*d);
    setRight(20*d);
    wait(motionDelay, msec);
  }
  backPne.set(true);
  setLeft(0);
  setRight(0);
}
*/

#endif