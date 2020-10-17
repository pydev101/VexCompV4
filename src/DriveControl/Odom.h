/*----------------------------------------------------------------------------
//                                                                            
//    Module:       Odom.h                                               
//    Author:       Carson E                                   
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that creates a virtual grid in the robot and contains top level motor control functions                                     
//                                                                            
//--------------------------------------------------------------------------*/

#include "AdvMotor.h"

double X = 0;
double Y = 0;
double Xt = 0;
double Yt = 0;
double tHead = PI/2;

double turnTo(double x, bool inDeg = true){
  if(inDeg){
    x = x * (PI/180);
  }
  tHead = x;
  return rotatePID(tHead-getHeading());
}

void move(double deltaX, double deltaY, double maxSpeed, bool rotate=true){
  Xt += deltaX;
  Yt += deltaY;
  deltaX = Xt-X;
  deltaY = Yt-Y;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY))*measureWheelDegsOverInches;
  //TODO Test Ht and see if it spins right
  double Ht = atan2(deltaY, deltaX); //Returns in radians; PID requires radians; see if deltaX and deltaY need to be special

  resetEncoders();
  if(rotate){
    turnTo(Ht, false);
    axisPID(0, travelDistance, maxSpeed);
  }else{
    vectorPID(Ht, travelDistance, maxSpeed);
  }

  X += (getRightVertEnc()*cos(getHeading()) + getHorEnc()*cos(getHeading()-(PI/2)))/measureWheelDegsOverInches;
  Y += (getRightVertEnc()*sin(getHeading()) + getHorEnc()*sin(getHeading()-(PI/2)))/measureWheelDegsOverInches;
}

void move(double deltaX, double deltaY, bool rotate=true){
  move(deltaX, deltaY, 0, rotate);
}