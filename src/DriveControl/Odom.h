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
double Head = 90;
double tHead = 90;

void move(double deltaX, double deltaY, bool rotate=true){
  Xt += deltaX;
  Yt += deltaY;
  deltaX = Xt-X;
  deltaY = Yt-Y;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY))*measureWheelDegsOverInches;
  //TODO Test Ht and see if it spins right
  double Ht = atan2(deltaY, deltaX); //Returns in radians; PID requires radians; see if deltaX and deltaY need to be special
  resetEncoders();
  if(rotate){
    tHead = Ht*(180/PI);
    Head += rotatePID((tHead-Head)/(180/PI))*(180/PI);
    axisPID(0, travelDistance);
  }else{
    vectorPID(Ht, travelDistance);
  }
  X += getHorEnc()/measureWheelDegsOverInches;
  Y += getRightVertEnc()/measureWheelDegsOverInches;
}