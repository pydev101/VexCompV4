#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
//Intergrates phyical and virtual components

#include "motorFunctions.h"
#include "odom/robot.h"

//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const double UnitsPerRev = PI*10.16*1.4911*0.37924; //Inches per revolution + 3/2 gear ratio
const double RobotDiameter = 15; //Inches (Same Units as above)
const double RobotRadius = 0.5*RobotDiameter;
const double linThreashold = 1; //In
const double angularThreashold = 0.0056*(2*PI); //1 deg

Robot robot = Robot(Point(0, 0), 90, true, {0.8,0,0}, {8,0,0}, linThreashold, angularThreashold);

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

  //double deltaFwd = 0.5*((leftEnc-lastLeft) + (rightEnc-lastRight))*UnitsPerRev; //TODO Breaks while turning
  double deltaL = (leftEnc-lastLeft)*UnitsPerRev;
  double deltaR = (rightEnc-lastRight)*UnitsPerRev;
  double deltaC = degToRad(deltaHead)*RobotRadius;
  deltaL = deltaL + deltaC;
  deltaR = deltaR - deltaC;
  double deltaFwd = 0.5*(deltaL + deltaR);

  lastLeft = leftEnc;
  lastRight = rightEnc;

  Vector deltaPos = Vector(deltaFwd, head, true);
  robot.location.updatePosition(deltaPos, deltaHead, deltaT, true);
  robot.location.setHead(head, true);

  lastTime = Brain.timer(timeUnits::msec);
}

int trakerFunction(){
  Point p = Point(0,0);
  while(true){
    track();
    p = robot.location.getPos();
    std::cout << p.x << ", " << p.y << ", " << robot.location.getCurrHead() << std::endl;
    wait(20, timeUnits::msec);
  }
} //Called in Pre-Auton

//Moves realitive to current position using robot orientation
//Rotates realitive to target
void turn(double theta, bool inDeg){
  robot.setHeadTarget(theta, inDeg);
  double speed = robot.turnCV(33);
  while(abs(speed) > 0){
    setLeft(-speed);
    setRight(speed);
    speed = robot.turnCV(33);
  }
  setLeft(0);
  setRight(0);
}
//Turns to abs orientation
void turnTo(double theta, bool inDeg){
  robot.setHeadTargetAbs(theta, inDeg);
  turn(0, false);
}

void move(Vector v){
  robot.setTargetRealitiveToRobotOrientation(v);
  turn(0, true);
  double speed = robot.moveCV(300);
  while(abs(speed) > 0){
    setLeft(speed);
    setRight(speed);
    speed = robot.moveCV(300);
  }
  setLeft(0);
  setRight(0);
}
void move(double fwd, double hor){
  move(Vector(hor, fwd));
}
void move(double mag, double theta, bool inDeg){
  move(Vector(mag, theta, inDeg));
}

//TODO Continous tracking
//Important to call linear before theta
//TODO Program Drive and turn; Begin by seetting the target; Get the values from robot; Set values to motors after conversion; Repeat until within threashold and velcoity near zero

#endif