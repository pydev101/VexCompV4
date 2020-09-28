/*----------------------------------------------------------------------------
//                                                                            
//    Module:       BaseMotor.h                                               
//    Author:       Carson E                                  
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that contains functions that interface with motors and sensors directly                                               
//                                                                            
//--------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

//Constants - Adjustible based on robot design
const double minDPSSpeed = 6; //To adjust; TEMP set to 1RPM; = 6 DPS;

//Constants based on known facts
const double maxDPSSpeed = 1200; //18:1 cartiage has 200 RPM; 200 RPM = 1200 DPS;

//Wheel Bias
double fRB = 1;
double fLB = 1;
double bRB = 1;
double bLB = 1;

//Function Definitions
void setDPS(double targetVel){
  //Sets velocity of motors; directon depends on current axis; Units in degrees per second
  frontLeft.setVelocity(targetVel*fLB, velocityUnits::dps);
  frontRight.setVelocity(targetVel*fRB, velocityUnits::dps);
  backLeft.setVelocity(targetVel*bLB, velocityUnits::dps);
  backRight.setVelocity(targetVel*bRB, velocityUnits::dps);
}

void stopMotors(){
  setDPS(0);
  fRB = 1;
  fLB = 1;
  bRB = 1;
  bLB = 1;
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

//Non Rotation Movement

void setAxis(int axis){
  //Sets/Starts wheels spining in direction needed for axis

  if(axis == 0){
    //Vertical - Positive is fwd
    fRB = 1;
    fLB = 1;
    bRB = 1;
    bLB = 1;
    frontRight.spin(directionType::fwd);
    frontLeft.spin(directionType::fwd);
    backRight.spin(directionType::fwd);
    backLeft.spin(directionType::fwd);

  }else if(axis == 1){
    //Horizontal - Positive is right
    fRB = -1;
    fLB = 1;
    bRB = 1;
    bLB = -1;
    frontRight.spin(directionType::fwd);
    frontLeft.spin(directionType::fwd);
    backRight.spin(directionType::fwd);
    backLeft.spin(directionType::fwd);
  }else if(axis == 2){
    //Diagnal - Positive is fwd-right
    fLB = 1;
    bRB = 1;
    frontLeft.spin(directionType::fwd);
    backRight.spin(directionType::fwd);
  }
  else if(axis == 3){
    //Diagnal - Positive is fwd-left
    fRB = 1;
    bLB = 1;
    backLeft.spin(directionType::fwd);
    frontRight.spin(directionType::fwd);
  }
}

void setDPSVector(int axis, double vel){
  //Sets Axis and Velocity
  setAxis(axis);
  setDPS(vel);
}

//Rotation Movement
void rotate(){
  //Starts the robot moving counterclockwise around the center of the base
  fRB = 1;
  fLB = -1;
  bRB = 1;
  bLB = -1;
  frontRight.spin(directionType::fwd);
  frontLeft.spin(directionType::fwd);
  backRight.spin(directionType::fwd);
  backLeft.spin(directionType::fwd);
}

bool isSpining(){
  if(frontLeft.isSpinning() || frontRight.isSpinning() || backLeft.isSpinning() || backRight.isSpinning()){
    return true;
  }else{
    return false;
  }
}

//Encoder
void resetEncoders(){
  horEncoder.resetRotation();
  rightEncoder.resetRotation();
  leftEncoder.resetRotation();
}

void resetHeading(){
  ISensor.resetHeading();
}

double getRightVertEnc(){
  return -1*rightEncoder.position(rotationUnits::deg);
}

double getLeftVertEnc(){
  return leftEncoder.position(rotationUnits::deg);
}

double getHorEnc(){
  return -1*horEncoder.position(rotationUnits::deg);
}

double getHeading(){
  return ISensor.heading();
}

double getAxisEncoder(int axis){
  if(axis == 0){
    //Vertical - Positive is fwd
    return getRightVertEnc();
  }else if(axis == 1){
    //Horizontal - Positive is right
    return getHorEnc();
  }else{
    double ver = getRightVertEnc();
    double hor = getRightVertEnc();
    if(axis == 2){
      //Diagnal - Positive is fwd-right
      if((hor>0) && (ver>0)){
        return sqrt((ver*ver)+(hor*hor));
      }else if((hor<0) && (ver<0)){
        return -sqrt((ver*ver)+(hor*hor));
      }
    }else if(axis == 3){
      //Diagnal - Positive is fwd-left
      if((hor<0) && (ver>0)){
        return sqrt((ver*ver)+(hor*hor));
      }else if((hor>0) && (ver<0)){
        return -sqrt((ver*ver)+(hor*hor));
      }
    }
  }
  std::cout << "ERROR" << std::endl;
  return 0;
}