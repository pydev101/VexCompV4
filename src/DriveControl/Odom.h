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
double Heading = PI/2;
double tHead = PI/2;

void stats(){
  std::cout << "Current (" << X <<", " << Y << ") |";
  std::cout << "Target (" << Xt <<", " << Yt << ") |";
  std::cout << "Heading: " << Heading <<" | Target: " << tHead << std::endl;
}

double turnTo(double x, bool inDeg = true, bool useSenor = true){
  if(inDeg){
    x = x * (PI/180);
  }
  tHead = x;
  resetHeading();
  if(useSenor){
    rotateSensor(tHead-Heading);
  }else{
    rotatePID(tHead-Heading);
  }
  double throwawayInt;
  Heading -= (2*PI)*modf(getHeading()/(2*PI), &throwawayInt);
  return -getHeading();
}

void move(double deltaX, double deltaY, double maxSpeed, bool rotate=true){
  Xt += deltaX;
  Yt += deltaY;
  deltaX = Xt-X;
  deltaY = Yt-Y;
  
  std::cout << "Start: " << deltaX << ", " << deltaY <<std::endl;
  //stats();

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

  X += (getRightVertEnc()*cos(Heading) + getHorEnc()*cos(Heading-(PI/2)))/measureWheelDegsOverInches; //should be delta x
  Y += (getRightVertEnc()*sin(Heading) + getHorEnc()*sin(Heading-(PI/2)))/measureWheelDegsOverInches; //should b delta y

  std::cout << "End: " << deltaX << ", " << deltaY <<std::endl;
  stats();
  std::cout << "---------------------------------------" <<std::endl;
}

void move(double deltaX, double deltaY, bool rotate=true){
  move(deltaX, deltaY, 0, rotate);
}