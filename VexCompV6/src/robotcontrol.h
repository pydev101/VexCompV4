#ifndef __ROBOTCONTROL_H__
#define __ROBOTCONTROL_H__
#include "vex.h"
#include "odom/odometry.h"

OdomGrid grid = OdomGrid(Point(0,0), PI/2, false);

extern motor leftMotorA;
extern motor rightMotorA;
extern inertial DrivetrainInertial;

void track(){
  double leftEnc = leftMotorA.rotation(deg);
  double rightEnc = rightMotorA.rotation(deg);
  double head = DrivetrainInertial.heading();
  double avg = 0.5*(rightEnc + leftEnc);

  grid.setCurrentHead(head, true);
  grid.shiftPos(avg, head, true);

  leftMotorA.resetRotation();
  rightMotorA.resetRotation();
}

/*
TODO Track when not moving, then use track() once per cycle while moving
use start(Drive/Turn)For then do while isMoving track and wait
then reactivate tracking mode when done moving
*/

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
}


#endif