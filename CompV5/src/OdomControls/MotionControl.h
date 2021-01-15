#ifndef __MOTIONCONTROL_H__
#define __MOTIONCONTROL_H__

#include "vex.h"

const double PI_T = 3.14159;
double minSpeed = 20;

void initMotors(){
  motorLF.setBrake(brakeType::hold);
  motorLB.setBrake(brakeType::hold);
  motorRF.setBrake(brakeType::hold);
  motorRB.setBrake(brakeType::hold);
  motorLF.stop();
  motorLB.stop();
  motorRF.stop();
  motorRB.stop();
}

void setLeft(double vel){
  static bool isStopped = true;
  
  motorLF.setVelocity(vel, velocityUnits::dps);
  motorLB.setVelocity(vel, velocityUnits::dps);

  if(vel < minSpeed){
    motorLF.stop();
    motorLB.stop();
    isStopped = true;
  }else if(isStopped){
    motorLF.spin(fwd);
    motorLB.spin(fwd);
    isStopped = false;
  }
}

void setRight(double vel){
  static bool isStopped = true;
  
  motorRF.setVelocity(vel, velocityUnits::dps);
  motorRB.setVelocity(vel, velocityUnits::dps);

  if(vel < minSpeed){
    motorRF.stop();
    motorRB.stop();
    isStopped = true;
  }else if(isStopped){
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