/*----------------------------------------------------------------------------
//                                                                            
//    Module:       BaseMotor.h                                               
//    Author:       Carson E                                
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that contains functions that provide a layer of logic ontop of basic drive functions in order to more accuratly move                                            
//                                                                            
//--------------------------------------------------------------------------*/

#include "BaseMotor.h"
#include "math.h"

//Adj Constants
const double timeUnit = 10; //msec; Delay time between updates of PID function
const double widthOfBaseMeters = 13; //In Inches
const double heightOfBaseMeters = 11; //In Inches
const double diameterOfTravelWheel = 4; //In Inches
const double diameterOfMeasureWheel = 3; //In Inches

//Known constants
const double timeUnitsPerSecond = 1000/timeUnit; //How many time units per second (t/s)
const double minDegreesPerTimeUnit = minDPSSpeed/timeUnitsPerSecond; //The least ammount of degrees travelable in one time unit
const double turnDiameter = sqrt((widthOfBaseMeters*widthOfBaseMeters) + (heightOfBaseMeters*heightOfBaseMeters)); //Diagboal length across wheels in inches

//Helper
double abs(double x){
  if(x<0){x=x*-1;}
  return x;
}

//Main Functions
struct results{double speed; double lastError;};

results PID(double encTarget, double encCurr, double gain, results r={0,0}){
  //EncTarget is in degrees
  double error = encTarget - encCurr;
  
  //reset = r.reset + (0 * error);
  //double d = 0 * (error - r.lastError);
  double p = gain * error;

  return results{p, error};
}


void axisPID(int axis, double degrees, int stopDelay=500, double gain=1.25){
  resetEncoders();

  results r = PID(degrees, getAxisEncoder(axis), gain);
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return;
  }
  setAxis(axis);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(axis), gain);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getAxisEncoder(axis) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
}

void rotatePID(double degreesTarget, bool CCW=true, int stopDelay=500, double gain=1.25){
  resetHeading();
  results r = PID(degreesTarget, getHeading(), gain);
}