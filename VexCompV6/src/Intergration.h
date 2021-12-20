#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
//Intergrates phyical and virtual components

#include "motorFunctions.h"
#include "odom/robot.h"

//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const double UnitsPerRev = 18.3207546*1.0167034; //Inches per revolution
const double RobotDiameter = 15; //Inches (Same Units as above)
const double RobotRadius = 0.5*RobotDiameter;
const double linThreashold = 1; //In
const double angularThreashold = degToRad(1);
const double maximumAccelerationLinear = 1000;
const double maximumAngularAcceleration = 1000;
const double maxVelocity = 1000;
const double maxAngularVelocity = 1000;

//Recommended low P (0.1) and High I (1-10), no D
Robot robot = Robot(Point(0, 0), 90, true, {0.1,5,0}, {0.1,10,0}, linThreashold, angularThreashold, maxVelocity, maxAngularVelocity, maximumAccelerationLinear, maximumAngularAcceleration);

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

int trakerFunction(){
  while(true){
    track();
    wait(20, timeUnits::msec);
  }
} //Called in Pre-Auton




void executeMove(int dir=1){
  while(robot.isMoving()){
    double linearSpeed = robot.getLinearSpeedTarget()*dir;
    double angularSpeed = robot.getRotationalSpeedTarget();

    linearSpeed = (linearSpeed/UnitsPerRev)*60; //Units/Sec to RPM
    angularSpeed = angularSpeed * (RobotRadius/UnitsPerRev) * 60 * 0.5; //Converts Rad/S to RPM and splits in half because there are 2 drive sides

    setLeft(linearSpeed - angularSpeed, velocityUnits::rpm);
    setRight(linearSpeed + angularSpeed, velocityUnits::rpm);
    wait(15, msec);
  }
  setLeft(0);
  setRight(0);
}

//Moves realitive to current position using robot orientation
//Rotates realitive to target
void turn(double theta, bool inDeg=true){
  robot.setTargetHead(theta, inDeg);
  executeMove();
}
//Turns to abs orientation
void turnTo(double theta, bool inDeg){
  robot.setHeadTargetAbs(theta, inDeg);
  turn(0, false);
}

void move(Vector v, bool backwards=false){
  robot.setTargetRealitiveToRobotOrientation(v);
  int dir = 1;
  if(backwards){
    dir = -1;
    robot.setTargetHead(180, true);
  }
  executeMove(dir);
}
void move(double fwd, double hor){
  move(Vector(hor, fwd));
}
void move(double mag, double theta, bool inDeg){
  move(Vector(mag, theta, inDeg));
}

#endif