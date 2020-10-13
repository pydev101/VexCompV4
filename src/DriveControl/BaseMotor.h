/*----------------------------------------------------------------------------
//                                                                            
//    Module:       BaseMotor.h                                               
//    Author:       Carson E                                  
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that contains functions that interface with motors and sensors directly                                               
//                                                                            
//--------------------------------------------------------------------------*/
#include "config.h"

//Wheel Bias
struct biasStruct{
  double fRB;
  double fLB;
  double bRB;
  double bLB;
};
biasStruct bias = {0,0,0,0};

const double PI = 3.141592653;
const double PIover180 = PI/180;

//Function Definitions
void setDPS(double targetVel){
  //Sets velocity of motors; directon depends on current axis; Units in degrees per second
  frontLeft.setVelocity(targetVel*bias.fLB, velocityUnits::dps);
  frontRight.setVelocity(targetVel*bias.fRB, velocityUnits::dps);
  backLeft.setVelocity(targetVel*bias.bLB, velocityUnits::dps);
  backRight.setVelocity(targetVel*bias.bRB, velocityUnits::dps);
}

void stopMotors(){
  bias = {0,0,0,0};
  setDPS(0);
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

//Linear Movement
void startSpin(){
  frontRight.spin(directionType::fwd);
  frontLeft.spin(directionType::fwd);
  backRight.spin(directionType::fwd);
  backLeft.spin(directionType::fwd);
}

void setAxis(int axis){
  //Sets/Starts wheels spining in direction needed for axis
  if(axis == 0){
    //Vertical - Positive is fwd
    bias = {1,1,1,1};
  }else if(axis == 1){
    //Horizontal - Positive is right
    bias = {-1,1,1,-1};
  }else if(axis == 2){
    //Diagnal - Positive is fwd-right
    bias = {0,1,1,0};
  }
  else{
    //Diagnal - Positive is fwd-left
    bias = {1,0,0,1};
  }
  startSpin();
}

void setVector(double rads){
  //Sets Angle of Velocity
  double a = (sqrt(2)*(sin(rads) + cos(rads)))/4;
  double b = (sqrt(2)*(sin(rads) - cos(rads)))/4;
  bias = {b, a, a, b};
  startSpin();
}

//Rotational Movement
void rotate(){
  //Starts the robot moving counterclockwise around the center of the base
  bias = {1,-1,1,-1};
  startSpin();
}

bool isSpining(){
  if(frontLeft.isSpinning() || frontRight.isSpinning() || backLeft.isSpinning() || backRight.isSpinning()){
    return true;
  }else{
    return false;
  }
}

//Encoders
void resetEncoders(){
  horEncoder.resetRotation();
  rightEncoder.resetRotation();
  leftEncoder.resetRotation();
}

void resetHeading(){
  ISensor.resetHeading();
  ISensor.setHeading(90, rotationUnits::deg);
}

double getRightVertEnc(){
  return rightEncoder.position(rotationUnits::deg);
}

double getLeftVertEnc(){
  return -1*leftEncoder.position(rotationUnits::deg);
}

double getHorEnc(){
  return -1*horEncoder.position(rotationUnits::deg);
}

double getHeading(){
  return ISensor.heading();
}

double getAxisEncoder(int axis){
  double ver = getRightVertEnc();
  double hor = getHorEnc();
  if(axis == 0){
    //Vertical - Positive is fwd
    return ver;
  }else if(axis == 1){
    //Horizontal - Positive is right
    return hor;
  }else if(axis == 2){
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
  }else{
    //Absolute motion
    return sqrt((ver*ver)+(hor*hor));
  }
  
  //std::cout << "ERROR AXIS ENCODER: " << ver << ", " << hor << std::endl;
  return 0;
}