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
const double turnCircumfranceMajor = (PI*sqrt((widthOfBaseMeters*widthOfBaseMeters) + (heightOfBaseMeters*heightOfBaseMeters))); //Diagboal length across wheels in inches times pi
const double turnCircumfranceMinor = (2*centerToMeasureWheelRadius*PI);
const double measureWheelC = PI*diameterOfMeasureWheel;

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


void axisPID(int axis, double degrees, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  resetEncoders();

  results r = PID(degrees, getAxisEncoder(axis), gain);
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return;
  }
  setAxis(axis);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(axis), gain, r);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getAxisEncoder(axis) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
}

void vectorPID(double deg, double degrees, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  resetEncoders();

  results r = PID(degrees, getAxisEncoder(4), gain);
  
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return;
  }
  setVector(deg);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(4), gain, r);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getAxisEncoder(4) << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
}

void rotatePID(double degreesTarget, int CCW=1, int stopDelay=defaultStopDelay, double gain=defaultGainRotational){
  double throwawayInt;
  degreesTarget = CCW*(turnCircumfranceMinor*modf(degreesTarget/360, &throwawayInt)); //Length of circumfrance needed to be traveled
  degreesTarget = (degreesTarget / measureWheelC)*360; // Degres measure wheel needs to be turned

  resetEncoders();
  results r = PID(degreesTarget, getRightVertEnc(), gain);
  if(abs(r.lastError) < minDegreesPerTimeUnit){
    stopMotors();
    return;
  }
  rotate();
  int currTimeUnit = 0;
  while((abs(r.lastError) > minDegreesPerTimeUnit) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degreesTarget, getRightVertEnc(), gain, r);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getRightVertEnc() << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
}