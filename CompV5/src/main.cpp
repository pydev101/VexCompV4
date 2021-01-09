// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         1               
// motorLB              motor         2               
// motorRF              motor         3               
// motorRB              motor         4               
// ISensor              inertial      5               
// ---- END VEXCODE CONFIGURED DEVICES ----

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\carson.easterling                                */
/*    Created:      Fri Dec 04 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "programs.h"
#include "driver.h"

using namespace vex;

void pre_auton(void) {
  vexcodeInit();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}
  
  initMotors();
  setDPS(0,0);

  resetHeading();
  resetEncoders();
}

competition Competition;
int main() {
  //AAA
  pre_auton();
  /*Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  while(true){
    wait(1000, msec);
  }*/

  return 0;
}