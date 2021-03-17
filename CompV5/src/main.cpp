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
  //Activate Vex components to run auto and driver during compeitionAAAAAAAAAAAAA
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  //autonomous();
  setIntake(1);
  setDPS(600,600);
  wait(1700, msec);
  setDPS(-400,400);
  wait(1200, msec);

  setDPS(400,400);
  wait(2700, msec);
  setDPS(0,0);

  setBottom(1);
  setTop(1);
  wait(1600, msec);
  setTop(0);
  setBottom(0);
  setIntake(0);

  setDPS(-800,-800);
  wait(500, msec);
  setBottom(-1);
  setIntake(-1);
  wait(500, msec);
  setDPS(0,0);
  wait(500, msec);
  setBottom(0);
  setIntake(0);

  setDPS(400,-400);
  wait(400, msec);
  setDPS(-1000, -1000);
  wait(2000, msec);
  for(int i=-1000; i<-400; i+=20){
    setDPS(i, i);  
    wait(5,msec);
  }
  setDPS(400, -400);
  wait(700, msec);
  setDPS(0,0);
  setDPS(600, 600);
  wait(3200, msec);
  setDPS(0,0);

  setDPS(400,-400);
  wait(600, msec);
  setDPS(400,400);
  wait(2700, msec);
  setDPS(0,0);

  keepRecordThreadRunning = false;
  std::cout << "Right Enc,Left Enc,Head,X,Y" << std::endl;
  for(int i=0; i<indexOfGraph; i++){
    std::cout << graphR[i] << "," << graphL[i] << "," << graphH[i] << "," << graphX[i] << "," << graphY[i] << std::endl;
    wait(30, msec);
  }

  return 0;
}