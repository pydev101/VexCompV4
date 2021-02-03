#ifndef __MOTIONCONTROL_H__
#define __MOTIONCONTROL_H__

#include "vex.h"

const double PI_T = 3.14159;
double minSpeed = 30;
double maxSpeed = 1100;

double _MOTORABS(double x){
  if(x<0){x=-x;}
  return x;
}

int _MOTORSIGN(double x){
  if(x<0){return -1;}
  return 1;
}

void setBreaks(brakeType x){
  motorLF.setBrake(x);
  motorLB.setBrake(x);
  motorRF.setBrake(x);
  motorRB.setBrake(x);
}

void setLeft(double vel){
  static bool isStopped = true;
  
  if(_MOTORABS(vel) > maxSpeed){
    vel = maxSpeed*_MOTORSIGN(vel);
  }
  motorLF.setVelocity(vel, velocityUnits::dps);
  motorLB.setVelocity(vel, velocityUnits::dps);

  if(_MOTORABS(vel) < minSpeed){
    motorLF.stop();
    motorLB.stop();
    isStopped = true;
  }else{
    motorLF.spin(fwd);
    motorLB.spin(fwd);
    isStopped = false;
  }
}

void setRight(double vel){
  static bool isStopped = true;
  
  if(_MOTORABS(vel) > maxSpeed){
    vel = maxSpeed*_MOTORSIGN(vel);
  }
  
  motorRF.setVelocity(vel, velocityUnits::dps);
  motorRB.setVelocity(vel, velocityUnits::dps);

  if(_MOTORABS(vel) < minSpeed){
    motorRF.stop();
    motorRB.stop();
    isStopped = true;
  }else{
    motorRF.spin(fwd);
    motorRB.spin(fwd);
    isStopped = false;
  }
}

void setDPS(double velL, double velR){
  setLeft(velL);
  setRight(velR);
}

void setDPS(double speeds[2]){
  setDPS(speeds[0], speeds[1]);
}

void setIntake(int d){

}

void setOutput(int d){
  
}

//RETURN MUST BE UNBOUNDED
double getHeading(bool inDeg=false){
  static double Heading = 90;
  double r = ISensor.rotation();
  Heading -= r;
  ISensor.resetRotation();

  if(inDeg){
    return Heading;
  }
  return Heading*(PI_T/180);
}

void resetEncoders(){
  motorLB.resetRotation();
  motorRB.resetRotation();
}

double getLeft(){
  return motorLB.position(rotationUnits::deg);  
}

double getRight(){
  return motorRB.position(rotationUnits::deg);
}

double getHor(){
  return 0;
}

#endif