#ifndef __INTERGRATION_H__
#define __INTERGRATION_H__

#include "vex.h"
using namespace vex;

void setM(motor m, int speed){
  m.setVelocity(speed, percent);
  if(speed == 0){
    m.stop();
  }else{
    m.spin(forward);
  }
}
void setLeft(int speed){
  setM(leftA, speed);
  setM(leftB, speed);
  setM(leftC, speed);
}
void setRight(int speed){
  setM(rightA, speed);
  setM(rightB, speed);
  setM(rightC, speed);
}
void setArm(int speed){
  setM(arm, speed);
}
void setIntake(int speed){
  setM(intakeM, speed);
}

void move(int speed, int dur, bool rotate=false){
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
void moveArm(int speed, int dur){
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

#endif