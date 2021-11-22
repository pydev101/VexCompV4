#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__
//Intergrates phyical and virtual components

#include "motorFunctions.h"
#include "odom/robot.h"

//Set position and target using robot, get linear and angular speed from robot, set speed of motors to reflect robot
const double UnitsPerRev = PI*10.16; //Cms per revolution
const double RobotDiameter = 1; //Cms (Same Units as above) //TODO TODO TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const double linThreashold = 5; //CM
const double angularThreashold = 0.02*(2*PI); //2% of 2PI

Robot robot = Robot(Point(0, 0), 90, true, 1, 1);

void track(){
  static double lastHeading = 0;
  static double lastTime = 0;

  double leftEnc = getLeftEnc();
  double rightEnc = getRightEnc();
  double head = Inertial.heading(); //TODO Ensure this is NOT bounded between 0-2PI; Alternate option is orientation, or rotation; Also ensure + in CCW direction
  double deltaT = Brain.timer(timeUnits::msec) - lastTime;
  deltaT = deltaT / 1000;

  double deltaFwd = 0.5*(leftEnc + rightEnc)*UnitsPerRev;
  double deltaHead = head - lastHeading; //TODO Ensure Direction of heading is Pos in CCW!!!!!!!!!!!!!!!!!!!!!!!!
  Vector deltaPos = Vector(deltaFwd, head, true);

  robot.location.updatePosition(deltaPos, deltaHead, deltaT, true);

  lastHeading = head;
  resetEncoders();
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
void turn(double theta, bool inDeg){
  robot.setHeadTarget(theta, inDeg);
  //TODO Have "should loop" be in robot to provide more intellgence regarding when to stop, and allow it to govern how it stops
  while(abs(robot.location.getThetaError()) > angularThreashold){
    double angVel = robot.getRotationalSpeed()*(RobotDiameter*0.5)*0.5; //WR = V; Times 1/2 because two sets of wheels so 0.5WR = V
    setLeft(-angVel,velocityUnits::rpm);
    setRight(angVel,velocityUnits::rpm);
    wait(20, timeUnits::msec);
  }
  setLeft(0,velocityUnits::rpm);
  setRight(0,velocityUnits::rpm);
}
//Turns to abs orientation
void turnTo(double theta, bool inDeg){
  robot.setHeadTargetAbs(theta, inDeg);
  turn(0, false);
}

void move(Vector v){
  robot.setTargetRealitiveToRobotOrientation(v);

  turn(0, false);

  //TODO Have "should loop" be in robot to provide more intellgence regarding when to stop, and allow it to govern how it stops
  while(abs(robot.location.getLinearError()) > linThreashold){
    double linVel = robot.getLinearSpeed()/UnitsPerRev*60; //Speed in terms of Rev/Min
    double angVel = robot.getRotationalSpeed()*(RobotDiameter*0.5)*0.5; //WR = V; Times 1/2 because two sets of wheels so 0.5WR = V

    if(abs(robot.location.getThetaError()) < 0.1*(2*PI)){
      if(abs(robot.location.getThetaError()) < angularThreashold){
        setLeft(linVel,velocityUnits::rpm);
        setRight(linVel,velocityUnits::rpm);
      }else{
        setLeft(linVel-angVel,velocityUnits::rpm);
        setRight(linVel+angVel,velocityUnits::rpm);
      }
    }else{
      turn(0, false);
    }
    wait(20, timeUnits::msec);
  }
  setLeft(0,velocityUnits::rpm);
  setRight(0,velocityUnits::rpm);
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