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
double Head = 90;

void move(double deltaX, double deltaY, bool rotate=false){
  double theta;
  if(deltaX == 0){
    if(deltaY < 0){
      theta = 270;
    }else if(theta > 0){
      theta = 90;
    }else{
      return;
    }
  }else{
    theta = atan(deltaY/deltaX)/PIover180;
    if(deltaX<0){
      if(deltaY<0){
        theta = theta + 180;
      }else{
        theta = theta + 90;
      }
    }else{
      if(deltaY<0){
        theta = theta + 270;
      }
    }
  }

  if(rotate){
    resetHeading();
    double deltaRotation = rotatePID(theta-Head);
    Head = Head + deltaRotation;
  }else{
    vectorPID(theta, sqrt((deltaX*deltaX)+(deltaY*deltaY)));
  }
  X = X + getHorEnc();
  Y = Y + getRightVertEnc();
}
