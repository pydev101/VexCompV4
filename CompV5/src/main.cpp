// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         5               
// motorRB              motor         10              
// ISensor              inertial      1               
// Controller1          controller                    
// topIntake            motor         19              
// bottomIntake         motor         18              
// rightIntake          motor         12              
// leftIntake           motor         13              
// VisionS              vision        6               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         5               
// motorRB              motor         10              
// ISensor              inertial      1               
// Controller1          controller                    
// topIntake            motor         19              
// bottomIntake         motor         18              
// rightIntake          motor         12              
// leftIntake           motor         13              
// VisionS              vision        6               
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
  //Activate Vex components to run auto and driver during compeitionAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  std::cout << "Cycle, Error, P, I, D, DeltaE"  << std::endl;
  turnToHead(270);

  /*moveAbs(0, 20, false);
  moveAbs(20, 20, false);
  moveAbs(20, 0, false);
  moveAbs(0, 0, false);
  moveAbs(20, 20, false);
  moveAbs(0, 36, false);
  turnToHead(90);*/

  return 0;
}