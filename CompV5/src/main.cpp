// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         9               
// motorRB              motor         10              
// ISensor              inertial      1               
// Controller1          controller                    
// topIntake            motor         19              
// bottomIntake         motor         18              
// rightIntake          motor         12              
// leftIntake           motor         13              
// ---- END VEXCODE CONFIGURED DEVICES ----


/*----------------------------------------------------------------------------
    Module:       main.cpp                                                  
    Author:       Carson E                                                  
    Created:      Fri Dec 04 2020                                           
    Description:  V5 project                                                                                                                       
----------------------------------------------------------------------------*/

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
  setBreaks(brakeType::coast);
  setDPS(0,0);

  //Encoders start at 0 because we are starting at 0 displacement
  resetEncoders();

  //Start tracking thread to log any movement of the robot onto the virtual grid
  thread t = thread(threadTask);
  //BrainGUIProgram();
}

//competition Competition;
int main() {
  //Run our setup
  pre_auton();
  //Activate Vex components to run auto and driver during compeitionAAAAAAAAAAAAA
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  //autonomous();

  setBottom(1);
  setIntake(1);
  robot.setMaxSpeed(25);
  move(25,0);

  robot.setMaxSpeed(100);
  turnToHead(225);
  //move(12,0);
  
  setDPS(200,200);
  wait(2000, msec);
  setDPS(0,0);

  setTop(1);
  setBottom(1);
  wait(2000, msec);
  setTop(0);
  setBottom(0);
  move(-30,0);
  

  return 0;
}