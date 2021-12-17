#ifndef __MOTORFUNCTIONS_H__
#define __MOTORFUNCTIONS_H__

#include "vex.h"
using namespace vex;

void setM(motor m, double speed, velocityUnits uni=velocityUnits::pct){
  m.setVelocity(speed, uni);
  if(speed == 0){
    m.stop();
  }else{
    m.spin(forward);
  }
}
void setLeft(double speed,  velocityUnits uni=velocityUnits::pct){
  setM(leftA, speed, uni);
  setM(leftB, speed, uni);
  setM(leftC, speed, uni);
}
void setRight(double speed, velocityUnits uni=velocityUnits::pct){
  setM(rightA, speed, uni);
  setM(rightB, speed, uni);
  setM(rightC, speed, uni);
}
void setArm(double speed){
  setM(arm, speed);
}
void setIntake(double speed){
  setM(intakeM, speed);
}

void moveTime(double speed, int dur, bool rotate=false){
  if(rotate){
    setRight(speed);
    setLeft(-speed);
  }else{
    setRight(speed);
    setLeft(speed);
  }
  wait(dur, timeUnits::msec);
  setRight(0);
  setLeft(0);
  wait(500, msec);
}
void moveArm(double speed, int dur){
  setArm(speed);
  wait(dur, timeUnits::msec);
  setArm(0);
}
void setFront(bool active){
  frontPne.set(active);
  wait(500, timeUnits::msec);
}
void setBack(bool active){
  backPne.set(active);
  wait(500, timeUnits::msec);
}
void resetEncoders(){
  rightC.resetPosition();
  leftC.resetPosition();
}
double getLeftEnc(){
  return leftC.position(rotationUnits::rev);
}
double getRightEnc(){
  return rightC.position(rotationUnits::rev);
}

#endif