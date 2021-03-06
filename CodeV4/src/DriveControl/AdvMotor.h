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
const double minRotationalRadsPerTimeUnit = ((minDPSSpeed*(diameterOfTravelWheel/360))/(sqrt(widthOfBaseMeters*widthOfBaseMeters+heightOfBaseMeters*heightOfBaseMeters)/2))/timeUnitsPerSecond;

//Helper
double abs(double x){
  if(x<0){x=x*-1;}
  return x;
}

//Main Functions
struct results{double speed; double lastError; double reset;};

results PID(double encTarget, double encCurr, double gain, results r={0,0,0}, double maxSpeed = 0,double rGain=0, double dGain=0){
  //EncTarget is in degrees
  double error = encTarget - encCurr;
  
  double reset = r.reset + (rGain * error);
  double speed = (gain*error)+reset+(dGain*(error - r.lastError));

  if((maxSpeed != 0) && (abs(speed) > maxSpeed) && (speed != 0)){
    speed = (abs(speed)/speed)*maxSpeed;
  }
  return results{speed, error, reset};
}

//Keep a straight line in one of 6 vectors
double axisPID(int axis, double degrees, double maxSpeed=0, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  double startEnc = getAxisEncoder(axis);
  results r = PID(degrees, getAxisEncoder(axis)-startEnc, gain);
  
  if(abs(r.lastError) < (minDegreesPerTimeUnit+linearMotionThresholdBoost)){
    stopMotors();
    return 0;
  }
  setAxis(axis);

  int currTimeUnit = 0;
  while((abs(r.lastError) > (minDegreesPerTimeUnit+linearMotionThresholdBoost)) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(axis)-startEnc, gain, r, maxSpeed);
    setDPS(r.speed);
    //std::cout << currTimeUnit << ", " << getAxisEncoder(axis)-startEnc << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return getAxisEncoder(axis)-startEnc;
}

//Will travel in any 2d direction
double vectorPID(double rads, double degrees, double maxSpeed=0, int stopDelay=defaultStopDelay, double gain=defaultGainLinear){
  double startEnc = getAxisEncoder(4);

  results r = PID(degrees, getAxisEncoder(4)-startEnc, gain);
  
  if(abs(r.lastError) < (minDegreesPerTimeUnit+linearMotionThresholdBoost)){
    stopMotors();
    return 0;
  }
  setVector(rads);
  
  int currTimeUnit = 0;
  while((abs(r.lastError) > (minDegreesPerTimeUnit+linearMotionThresholdBoost)) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degrees, getAxisEncoder(4)-startEnc, gain, r, maxSpeed);
    setDPS(r.speed);
    std::cout << currTimeUnit << ", " << getAxisEncoder(4)-startEnc << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return getAxisEncoder(4)-startEnc;
}

//Robot rotation using robot motor encoders
double rotatePID(double radians, double maxSpeed=0, int stopDelay=defaultStopDelay, double gain=defaultGainRotational){
  double throwawayInt;
  double degreesTarget = (2*PI)*modf(radians/(2*PI), &throwawayInt); //Radians needed
  if(abs(radians) > PI){
    if(radians < 0){
      radians = (2*PI)-abs(radians);
    }else{
      radians = -(2*PI)+abs(radians);
    }
  }
  degreesTarget = degreesTarget*centerToMeasureWheelRadius*measureWheelDegsOverInches; //Travel distance in mDegs

  double startEnc = getRightVertEnc();
  results r = PID(degreesTarget, getRightVertEnc()-startEnc, gain);
  if(abs(r.lastError) < (minDegreesPerTimeUnit+rotationalMotionThresholdBoost)){
    stopMotors();
    return 0;
  }
  rotate();

  int currTimeUnit = 0;
  while((abs(r.lastError) > (minDegreesPerTimeUnit+rotationalMotionThresholdBoost)) && (abs(r.speed) >= minDPSSpeed)){
    r = PID(degreesTarget, getRightVertEnc()-startEnc, gain, r, maxSpeed);
    setDPS(r.speed);
    //std::cout << currTimeUnit << ", " << getRightVertEnc()-startEnc << ", " << r.lastError << ", " << r.speed << std::endl;
    wait(timeUnit, msec);
    currTimeUnit++;
  }
  stopMotors();
  wait(stopDelay, timeUnits::msec);
  return (getRightVertEnc()-startEnc)/(centerToMeasureWheelRadius*measureWheelDegsOverInches);
}
