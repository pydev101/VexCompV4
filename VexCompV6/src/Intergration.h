#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
//Intergrates phyical and virtual components

#include "motorFunctions.h"
#include "odom/robot.h"

//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const double UnitsPerRev = 18.3207546*1.0167034; //Inches per revolution
const double RobotDiameter = 15; //Inches (Same Units as above)
const double RobotRadius = 0.5*RobotDiameter;

const double updateTime = 20; //msec
const double motionDelay = 25; //msec

const double linThreashold = 1; //In
const double angularThreashold = degToRad(1);
const double maximumAccelerationLinear = 1000;
const double maximumAngularAcceleration = 1000;
const double maxVelocity = 1000;
const double maxAngularVelocity = 1000;
const double updateTargetHeadingMinThreashold = 5;
const double maxThetaErrorForMotion = 15; //deg
bool maxThetaErrorForMotionGivenInDegrees = true;

Robot robot = Robot(Point(0, 0), 90, true, {1.3,0,0}, {5,0,0}, linThreashold, angularThreashold, maxVelocity, maxAngularVelocity, maximumAccelerationLinear, maximumAngularAcceleration, updateTargetHeadingMinThreashold, maxThetaErrorForMotion, maxThetaErrorForMotionGivenInDegrees);

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
  robot.updatePID(deltaT);
  robot.updateStopStatus(deltaT);

  lastTime = Brain.timer(timeUnits::msec);
}

int frame = 0;
int trakerFunction(){
  while(true){
    track();
    if(frame > 5){
      Vector tVec = robot.location.getTargetVector();
      Vector tRVec = Vector(0, robot.getLinearError());

      graph.addPoint({robot.location.pos, "green"});
      graph.addPoint({robot.location.targetPos, "blue"});
      graph.addVector({robot.location.pos, tVec, "teal"});
      graph.addVector({robot.location.pos, tRVec, "purple"});
      graph.addVector({robot.location.pos, Vector(1, robot.location.getTargetHead(), false).scale(2), "yellow"});
      graph.addVector({robot.location.pos, robot.location.getRobotBasisVector().scale(2), "red"}); 


      graph.output();
      graph.clear();
      frame = 0;
    }else{
      frame = frame + 1;
    }
    wait(updateTime, timeUnits::msec);
  }
} //Called in Pre-Auton



void executeMove(){
  wait(updateTime+1, msec);
  while(robot.isMoving()){
    double linearSpeed = robot.getLinearSpeedTarget();
    double angularSpeed = robot.getRotationalSpeedTarget();

    linearSpeed = (linearSpeed/UnitsPerRev)*60; //Units/Sec to RPM
    angularSpeed = angularSpeed * (RobotRadius/UnitsPerRev) * 60 * 0.5; //Converts Rad/S to RPM and splits in half because there are 2 drive sides

    setLeft(linearSpeed - angularSpeed, velocityUnits::rpm);
    setRight(linearSpeed + angularSpeed, velocityUnits::rpm);
    wait(motionDelay, msec);
  }
  setLeft(0);
  setRight(0);
}

//Moves realitive to current position using robot orientation
//Rotates realitive to target
void turn(double theta, bool inDeg=true){
  robot.setRotateMode();
  robot.setTargetHead(theta, inDeg);
  executeMove();
}
//Turns to abs orientation
void turnTo(double theta, bool inDeg){
  robot.setHeadTargetAbs(theta, inDeg);
  turn(0, false);
}

void moveAbs(double x, double y, bool fwd=true){
  robot.setMoveMode(fwd);
  robot.setAbsTarget(x, y);
  executeMove();
}

void move(Vector v, bool fwd=true){
  robot.setMoveMode(fwd);
  robot.setTargetRealitiveToRobotOrientation(v);
  executeMove();
}
void move(double fwd, double hor, bool dir=true){
  move(Vector(hor, fwd), dir);
}
void move(double mag, double theta, bool inDeg, bool dir){
  move(Vector(mag, theta, inDeg), dir);
}

#endif