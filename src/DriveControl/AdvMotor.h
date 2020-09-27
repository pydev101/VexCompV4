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
const double timeUnit = 200; //msec; Delay time between updates of PID function
const double gain = 0.85;
const double widthOfBaseMeters = 0; //In Meters
const double heightOfBaseMeters = 0; //In Meters
const double diameterOfTravelWheel = 0; //In Meters
const double diameterOfMeasureWheel = 0; //In Meters

//Known constants
const double timeUnitsPerSecond = 1000/timeUnit; //How many time units per second (t/s)
const double minDegreesPerTimeUnit = minDPSSpeed/timeUnitsPerSecond; //The least ammount of degrees travelable in one time unit
const double tau = gain/timeUnitsPerSecond; //Seconds per repeat * gain
const double turnDiameter = sqrt((widthOfBaseMeters*widthOfBaseMeters) + (heightOfBaseMeters*heightOfBaseMeters)); //Multiple meters*convserionConstant if wanted radians in motor degrees

//Helper
double abs(double x){
  if(x<0){x=x*-1;}
  return x;
}

//Main Functions
struct results{double speed; double lastError; double reset;};

results PID(double encTarget, double encCurr, double lastError=0, double reset=0){
  //EncTarget is in degrees
  double error = encTarget - encCurr;
  
  reset = reset + (tau * error);
  double p = gain * error;
  double d = tau * (error - lastError);

  return results{p+reset+d, error, reset};
}


void axisPID(int axis, double degrees, bool isInit=true){
  if(isInit){
    resetEncoders();
  }

  results r = PID(degrees, getAxisEncoder(axis));
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return;
  }
  setAxis(axis);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(axis), r.lastError, r.reset);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getAxisEncoder(axis) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }

  stopMotors();
}