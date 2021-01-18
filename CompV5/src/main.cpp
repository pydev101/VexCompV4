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
/*    Author:       C:\Users\carson                                           */
/*    Created:      Fri Dec 04 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

//Include Math libaries
#include "programs.h"
#include "driver.h"

using namespace vex;

//Setup robot in preparation to driver motion
void pre_auton(void) {
  //Run Vex setup
  vexcodeInit();

  //Wait while sensor calibrates; This is needed in order to keep a sense of direction
  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}
  
  //Setup motors; Break type and activate breaks
  initMotors();
  setDPS(0,0); //Setting speed to 0 turns the breaks on

  //Encoders start at 0 because we are starting at 0 displacement
  resetEncoders();

  //Start tracking thread to log any movement of the robot onto the virtual grid
  thread t = thread(threadTask);
}

competition Competition;
int main() {
  //Run our setup
  pre_auton();
  //Activate Vex components to run auto and driver during compeition
  /*Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  */

  robot.setTRealitive(100, 0);

  //setDPS(50, 50);

  double* speed = robot.moveLin(1);
  while(true){
    speed = robot.moveLin(1);
    std::cout << robot.getError(GRID) << "," << speed[0] << std::endl;
    setDPS(speed[0]*360/(2*PI*4.25), speed[1]*360/(2*PI*4.25));
    wait(10, msec);
  }

  return 0;
}