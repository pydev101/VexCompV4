/*----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                               
//    Author:       Carson E                                 
//    Created:      Thu Sep 10 2020                                           
//    Description:  Primary file that implements controls and movement                                           
//                                                                            
//--------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontLeft            motor         13              
// frontRight           motor         12              
// backLeft             motor         11              
// backRight            motor         14              
// rightEncoder         encoder       C, D            
// leftEncoder          encoder       E, F            
// horEncoder           encoder       A, B            
// ISensor              inertial      15              
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "AutoPrograms.h"
//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  setDPS(0);

  double deg = 10;
  double a = ((sin(deg*(3.14159265358979/180)+cos(deg*(3.14159265358979/180))*sqrt(2))/4));
  double b = ((sin(deg*(3.14159265358979/180)-cos(deg*(3.14159265358979/180))*sqrt(2))/4));

  bias = {b,a,a,b};
  setDPS(400);
  startSpin();
  wait(2000, msec);
  stopMotors();

  //rotatePID(90, -1);
  std::cout << "END " << std::endl;
}
