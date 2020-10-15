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
double tHead = 90;

void debug(){
  std::cout << "X: " << X << " Y: " << Y << " Head: " << ISensor.heading() << std::endl;
  std::cout << "Xt: " << Xt << " Yt: " << Yt << " Headt: " << tHead << std::endl;
  std::cout << "--------------------------" << std::endl;
}

double getTargetHeading(double deltaX, double deltaY){
  double theta = atan(abs(deltaY/deltaX))*(180/PI);
  if(deltaX > 0){
    if(deltaY > 0){
      return theta;
    }else{
      return 360-theta;
    }
  }else{
    if(deltaY > 0){
      return 180-theta;
    }else{
      return 180+theta;
    }
  }
}

double getTheta(){
  if(deltaX > 0){
    if(deltaY > 0){
      return theta;
    }else{
      return 360-theta;
    }
  }else{
    if(deltaY > 0){
      return 180-theta;
    }else{
      return 180+theta;
    }
  }
}

void move(double deltaX, double deltaY, bool rotate=true){
  Xt += deltaX;
  Yt += deltaY;
  deltaX = Xt-X;
  deltaY = Yt-Y;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY))*measureWheelDegsOverInches;
  //TODO Test Ht and see if it spins right
  double Ht = getTargetHeading(deltaX, deltaY); //Returns in degrees; PID requires radians; see if deltaX and deltaY need to be special

  resetEncoders();
  if(rotate){
    tHead = Ht;
    rotatePID((tHead-getHeading())*(PI/180));
    axisPID(0, travelDistance);
    X += cos((PI/180)*getHeading())*(getRightVertEnc()/measureWheelDegsOverInches);
    Y += sin((PI/180)*getHeading())*(getRightVertEnc()/measureWheelDegsOverInches);
  }else{
    vectorPID(Ht*(PI/180), travelDistance);
    X += getHorEnc()/measureWheelDegsOverInches;
    Y += getRightVertEnc()/measureWheelDegsOverInches;
  }

  debug();
}