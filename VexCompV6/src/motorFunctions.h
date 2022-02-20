#ifndef __MOTORFUNCTIONS_H__
#define __MOTORFUNCTIONS_H__

#include "vex.h"
using namespace vex;

/*
Name: motorFunctions.h
Written By: Carson Easterling

Functions related smarter motor control
*/

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


const double headingDriftFactor = 1;
double getHeadingUnbounded(){
  double head = Inertial.angle();
  return head*headingDriftFactor;
}
double getHeading(){
  double toss;
  double r = (360)*modf(getHeadingUnbounded()/(360), &toss);
  if(r<0){r+=(360);}
  return r;
}
double getHeadingCCW(){
  return (360 - getHeading());
}


double max(double a, double b){
  if(a > b){
    return a;
  }
  return b;
}
const int ControllerDisplayUpdateTime = 2000;
void updateControllerDisplay(){
  static int lastT = Brain.Timer.time(timeUnits::msec);
  if(Brain.Timer.time(timeUnits::msec)-lastT > ControllerDisplayUpdateTime){
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Battery: %d%", Brain.Battery.capacity());
      Controller1.Screen.newLine();
      double a = 0;
      a = max(a, rightA.temperature(temperatureUnits::celsius));
      a = max(a, rightB.temperature(temperatureUnits::celsius));
      a = max(a, rightC.temperature(temperatureUnits::celsius));
      a = max(a, leftA.temperature(temperatureUnits::celsius));
      a = max(a, leftB.temperature(temperatureUnits::celsius));
      a = max(a, leftC.temperature(temperatureUnits::celsius));
      Controller1.Screen.print("HighTemp: %.2f C", a);
      lastT = Brain.Timer.time(timeUnits::msec);
  }
}

#endif