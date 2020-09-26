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
const double timeUnit = 250; //msec; Delay time between updates of PID function
const double gain = 10;
const double widthOfBaseMeters = 0; //In Meters
const double heightOfBaseMeters = 0; //In Meters
const double diameterOfWheelMeters = 0; //In Meters

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

  if(abs(error) < minDegreesPerTimeUnit){
    //Robot unable to move distance in one time unit using minVelocity
    //setDPS(0);
    return results{0,0,0};
  }
  
  reset = reset + (tau * error);
  double p = gain * error;
  double d = tau * (error - lastError);

  return results{p+reset+d, error, reset};
}


void axisPID(int axis, double degrees){
  resetEncoders();
  setAxis(axis);
  results r = PID(degrees, getRightVertEnc());

  while(r.speed > 0){
    std::cout << "Encoder: " << getRightVertEnc() << " Speed: " << r.speed << " Error: " << r.lastError << std::endl;

    setDPS(r.speed);
    wait(timeUnit, timeUnits::msec);
    r = PID(degrees, getRightVertEnc(), r.lastError, r.reset);
  }
}