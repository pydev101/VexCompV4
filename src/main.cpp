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
// ISensor              inertial      20              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "AutoPrograms.h"

competition Competition;

void pre_auton(void) {
  vexcodeInit();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  setDPS(0);

  resetHeading();
  resetEncoders();
}


void autonomous(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
}


bool tank = true;
void usercontrol(void) {
  frontLeft.setBrake(brakeType::brake);
  frontRight.setBrake(brakeType::brake);
  backLeft.setBrake(brakeType::brake);
  backRight.setBrake(brakeType::brake);
  startSpin();
  
  double lV = 0;
  double lH = 0;
  double rV = 0;
  double rH = 0;

  while (true) {
    if(tank){
      lV = Controller1.Axis3.position();
      lH = Controller1.Axis4.position();
      rV = Controller1.Axis2.position();
      rH = Controller1.Axis1.position();

      frontLeft.setVelocity(lV+lH, velocityUnits::pct);
      frontRight.setVelocity(rV-rH, velocityUnits::pct);
      backLeft.setVelocity(lV-lH, velocityUnits::pct);
      backRight.setVelocity(rV+rH, velocityUnits::pct);
      wait(20, msec);
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAA

  //move(12, 12, true);
  //move(-36, 0, true);
  //move(0, 0, false);

  while (true) {
    wait(100, msec);
  }
}