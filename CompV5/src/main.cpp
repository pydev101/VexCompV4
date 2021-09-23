// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         5               
// motorRB              motor         10              
// ISensor              inertial      2               
// Controller1          controller                    
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
  while(ISensor.isCalibrating()){wait(10, msec);}
  
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
  //Activate Vex components to run auto and driver during compeitionAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);
  
  std::cout << "T, R, L, H, X, Y" << std::endl;
  while(indexOfGraph<DataLength){wait(5, msec);}

  for(int t=0; t<DataLength; t++){
    char out[500];
    sprintf(out, "%d, %f, %f, %f, %f, %f", t, graphR[t], graphL[t], graphH[t], graphX[t], graphY[t]);
    std::cout << out << std::endl;
    wait(100, msec);
  }

  return 0;
}