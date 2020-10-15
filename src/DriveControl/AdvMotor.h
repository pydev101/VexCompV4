/*----------------------------------------------------------------------------
//                                                                            
//    Module:       AdvMotor.h                                               
//    Author:       Carson E                                
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that contains functions that provide a layer of logic ontop of basic drive functions in order to more accuratly move                                            
//                                                                            
//--------------------------------------------------------------------------*/

#include "BaseMotor.h"

//Known constants
const double timeUnitsPerSecond = 1000/timeUnit; //How many time units per second (t/s)
const double minDegreesPerTimeUnit = minDPSSpeed/timeUnitsPerSecond; //The least ammount of degrees travelable in one time unit
const double measureWheelDegsOverInches = 360/(PI*diameterOfMeasureWheel);

//Helper
double abs(double x){
  if(x<0){x=x*-1;}
  return x;
}

//Main Functions
struct results{double speed; double lastError; double reset;};

results PID(double encTarget, double encCurr, double gain, results r={0,0,0}, double rGain=0, double dGain=0){
  //EncTarget is in degrees
  double error = encTarget - encCurr;
  
  double reset = r.reset + (rGain * error);
  double d = dGain * (error - r.lastError);
  double p = gain * error;

  return results{p+reset+d, error, reset};
}

//Keep a straight line in one of 6 vectors
double axisPID(int axis, double degrees, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  resetEncoders();

  results r = PID(degrees, getAxisEncoder(axis), gain);
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return 0;
  }
  setAxis(axis);

  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(axis), gain, r);
    setDPS(r.speed);
    //std::cout << currTimeUnit << ", " << getAxisEncoder(axis) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return getAxisEncoder(axis);
}

//Will travel in any 2d direction
double vectorPID(double rads, double degrees, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  resetEncoders();

  results r = PID(degrees, getAxisEncoder(4), gain);
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return 0;
  }
  setVector(rads);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(4), gain, r);
    setDPS(r.speed);
    //std::cout << currTimeUnit << ", " << getAxisEncoder(4) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return getAxisEncoder(4);
}

//Will rotate
double rotatePID(double radians, int stopDelay=defaultStopDelay, double gain=defaultGainRotational){
  double throwawayInt;
  double degreesTarget = (2*PI)*modf(radians/(2*PI), &throwawayInt); //Radians needed
  //std::cout << degreesTarget << std::endl;
  degreesTarget = degreesTarget*centerToMeasureWheelRadius*measureWheelDegsOverInches; //Travel distance in degs

  resetEncoders();
  results r = PID(degreesTarget, getRightVertEnc(), gain);
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return 0;
  }
  rotate();

  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degreesTarget, getRightVertEnc(), gain, r);
    setDPS(r.speed);
    //std::cout << currTimeUnit << ", " << getRightVertEnc() << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return getRightVertEnc()/(centerToMeasureWheelRadius*measureWheelDegsOverInches);
}