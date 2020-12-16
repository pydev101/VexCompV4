#include "vex.h"

double minSpeed = 20;

void initMotors(){
  motorLF.setBrake(brakeType::coast);
  motorLB.setBrake(brakeType::coast);
  motorRF.setBrake(brakeType::coast);
  motorRB.setBrake(brakeType::coast);
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

void setIntake(int d){

}

void setOutput(int d){
  
}

void resetHeading(){

}

void resetEncoders(){

}

double getLeft(){
  
}

double getRight(){

}

double getHor(){

}