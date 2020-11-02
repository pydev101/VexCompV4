/*----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                               
//    Author:       Carson E                                 
//    Created:      Thu Oct 1 2020                                           
//    Description:  File that contains all the default control varibles                                           
//                                                                            
//--------------------------------------------------------------------------*/

//Imports
#include "vex.h"
using namespace vex;

//Module: Basemotor.h
const double minDPSSpeed = 14; //Minimum velocity set to the motors that the robot is able to move at
const double maxDPSSpeed = 1200; //18:1 cartiage has 200 RPM; 200 RPM = 1200 DPS; Max speed of motors

//Module: AdvMotor.h
//Physical
const double widthOfBaseMeters = 13; //Inches; Length between left center of wheel and right center of wheel
const double heightOfBaseMeters = 11; //Inches; Length between front wheel axle and back wheel axle
const double diameterOfTravelWheel = 4; //Inches; Diameter of main moving wheels
const double diameterOfMeasureWheel = 3; //Inches; Diameter of measurement wheels
const double centerToMeasureWheelRadius = 7.3; //Inches; Distance between center of rotation to center of vertical measurement wheel
//Arbitary
const double timeUnit = 10; //msec; Delay time between updates of PID function
const double defaultStopDelay = 500; //Msec; Wait time after a move function has been called
const double defaultGainLinear = 1.35; //Default K value for linear movements
const double defaultGainRotational = 1.53; //Default K value for rotational movements
const double defaultGainRotationalWSensor = 250; //Default K value for values regarding sensor rotation
const double linearMotionThresholdBoost = 0; //Value added to the default threshold value to decease accuracy for moving on
const double rotationalMotionThresholdBoost = 0; //Value added to the default rotation threshold value to decease accuracy for moving on
const double rotationalSensorThresholdBoost = 0; //Value added to the default sensor threshold value to decease accuracy for moving on