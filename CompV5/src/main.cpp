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

#include "Intergration.h"

using namespace vex;

void pre_auton(void) {
  vexcodeInit();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}
  
  initMotors();
  setDPS(0,0);

  resetHeading();
  resetEncoders();
  task updateThread(updatePosThread);
}

int main() {
  //AAA
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  char buf[10000];
  vexFileDirectoryGet("", buf, sizeof(buf)-1);
  puts(buf);

  FIL* x = vexFileOpen("lastestAuto.csv", "r");
  std::cout << vexFileSize(x) << std::endl;
  vexFileRead(buf, sizeof(buf)-1, 1, x);
  vexFileClose(x);
  puts(buf);

  return 0;
}