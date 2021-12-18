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

//Recommended low P (0.1) and High I (1-10), no D
Robot robot = Robot(Point(0, 0), 90, true, {0.1,5,0}, {0.1,5,0}, linThreashold, angularThreashold);

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
  robot.location.updatePosition(deltaPos, deltaHead, deltaT, true);
  robot.location.setHead(head, true);

  lastTime = Brain.timer(timeUnits::msec);
}

int trakerFunction(){
  while(true){
    track();
    wait(20, timeUnits::msec);
  }
} //Called in Pre-Auton




//Moves realitive to current position using robot orientation
//Rotates realitive to target
void turn(double theta, bool inDeg=true){
  robot.setHeadTarget(theta, inDeg);
  robot.rotate(0,true);

  double lastTime = Brain.timer(timeUnits::msec);
  double timeInZone = 0;
  wait(5, msec);

  while(timeInZone < 500){
    double t = Brain.timer(timeUnits::msec);
    double deltaT = t - lastTime;
    lastTime = t;

    double o = robot.rotate(deltaT);
    setLeft(-o);
    setRight(o);

    if(abs(robot.location.getThetaError()) < angularThreashold){
      timeInZone = timeInZone + deltaT;
    }else{
      timeInZone = 0;
    }

    wait(15, msec);
  }
  setLeft(0);
  setRight(0);
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
    robot.location.setTargetHead(180, true);
  }
  turn(0);

  robot.linear(0,true);

  double lastTime = Brain.timer(timeUnits::msec);
  double timeInZone = 0;
  wait(5, msec);

  while(timeInZone < 500){
    double t = Brain.timer(timeUnits::msec);
    double deltaT = t - lastTime;
    lastTime = t;

    double o = robot.linear(deltaT);
    double a = robot.rotate(deltaT);
    setLeft(o*sign(dir)-a);
    setRight(o*sign(dir)+a);

    if(abs(robot.location.getLinearError()) < linThreashold){
      timeInZone = timeInZone + deltaT;
    }else{
      timeInZone = 0;
    }

    wait(15, msec);
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