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

void move(double deltaX, double deltaY, bool rotate=true){
  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY));
  double Ht = atan(deltaY/deltaX); //Returns in radians; PID requires radians; see if deltaX and deltaY need to be special

}