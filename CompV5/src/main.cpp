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
  //Activate Vex components to run auto and driver during compeitionAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  
  //std::cout << "Cycle, Error, P, I, D, DeltaE" << std::endl;
  //move(24, 24);

  setIntake(1);
  setBottom(1);
  setDPS(800,800);
  wait(1000, msec);
  setIntake(0);
  turnToHead(223);

  setDPS(500,500);
  wait(2300, msec);
  setDPS(0,0);

  setBottom(0);
  setTop(1);
  wait(800, msec);
  setTop(0);

  setDPS(-800,-800);
  wait(300, msec);
  //setIntake(-1);
  setDPS(0,0);
  turnToHead(180);
  setBottom(0);
  setIntake(0);

  setDPS(-1000, -1000);
  wait(1400, msec);
  for(int i=-980; i<-30; i+=20){
    setDPS(i, i);  
    wait(5,msec);
  }
  setDPS(0, 0);


  turnToHead(270);
  setDPS(600, 600);
  wait(400, msec);
  setBottom(1);
  setTop(1);
  setDPS(500, 500);
  wait(200, msec);
  setDPS(0, 0);


/*
  robot.setMaxAcceleration(200);
  move(90, 0);
  move(-78,0);
  turnToHead(90);
*/
  /*moveAbs(0, 20, false);
  moveAbs(20, 20, false);
  moveAbs(20, 0, false);
  moveAbs(0, 0, false);
  moveAbs(20, 20, false);
  moveAbs(0, 36, false);
  turnToHead(90);*/

  return 0;
}