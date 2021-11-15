#ifndef __ROBOTCONTROL_H__
#define __ROBOTCONTROL_H__
#include "vex.h"
#include "odom/robot.h"

//Point Pos, double CurrentHeading, bool headingGivenInDegrees, double linearKp, double angularKp
Robot robot = Robot(Point(0,0), 90, true, 1, 1);

const double cmsOverEncoders = 1;

void track(){
  static double lastHeading = 0;
  static double lastTime = 0;

  double leftEnc = leftB.rotation(deg);
  double rightEnc = rightB.rotation(deg);
  double head = Inertial.heading();
  double deltaT = Brain.timer(timeUnits::msec) - lastTime;
  deltaT = deltaT / 1000;

  double deltaFwd = 0.5*(leftEnc + rightEnc)*cmsOverEncoders;
  double deltaHead = head - lastHeading; //TODO Ensure Direction of heading is Pos in CCW!!!!!!!!!!!!!!!!!!!!!!!!
  Vector deltaPos = Vector(deltaFwd, head, true);

  robot.location.updatePosition(deltaPos, deltaHead, deltaT, true);

  lastHeading = head;
  leftB.resetRotation();
  rightB.resetRotation();
  lastTime = Brain.timer(timeUnits::msec);
}

void move(Vector target){
  
}


/*
TODO Track when not moving, then use track() once per cycle while moving
use start(Drive/Turn)For then do while isMoving track and wait
then reactivate tracking mode when done moving
*/
/*
int mode = 0;
bool controlLoop(){
  bool result = false;
  track();
  if(mode == 1){
    //Move
  }else if(mode == 2){
    //Rotate
  }
  if(! Drivetrain.isMoving()){
    mode = 0;
    result = true;
  }
  wait()
  return result;
}*/


#endif