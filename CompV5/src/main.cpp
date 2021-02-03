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
// bottomIntake         motor         20              
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         9               
// motorRB              motor         10              
// ISensor              inertial      1               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         8               
// motorLB              motor         7               
// motorRF              motor         9               
// motorRB              motor         10              
// ISensor              inertial      1               
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
}

  /*
  Checklist:
  DONE! 1) Motors: Wiring, Speed, and Direction
  DONE! 2) Encoders: getEncoder returns positive in the fwd direction; getHeading returns the Heading of the robot correctly in radians
  DONE! 3) Grid: Encoders map correctly to the grid and positive is reflected accuratly; Error values should be verified as well
  DONE! 4) Rotational Motion: Gain is set; Robot can turn in the shortest direction, both directions, and accuratly arrive at target; Close values should have a low speed value for use in grid movement
  DONE! 5) Linear Motion: Gain is tuned and direction, speed, acceleration are double checked or set
  DONE! 6) Grid Move: A target is set on the grid and the robot must arrive accuratly and turn to and stay straight

  Future Features:
  -Self adjusting gain and max acceleration through gradient decsent
  */

void transfer(){
  Point p;
  while(robot.driving()){
    p = robot.getPos();
    //std::cout << "(" << p.x << ", " << p.y << ", " << getStandardAngle(p.head) << ")" << std::endl;
    setDPS(robot.move());
    wait(20, msec);
  }
  setDPS(0,0);
  wait(1000, msec);
  std::cout << "(" << p.x << ", " << p.y << ", " << p.head << ", " << 0 << ")" << std::endl;
}

competition Competition;
int main() {
  //Run our setup
  pre_auton();
  //Activate Vex components to run auto and driver during compeition
  //Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  

  //TODO SET GAIN AND MAX ACCEL FOR TURNING!!!!
/*
  wait(1000, msec);

  robot.setTAbsolute(12*2, 0);
  transfer();
  robot.setTRealitive(0, -24);
  transfer();
  robot.setTAbsolute(0, 0);
  transfer();
//vexFileOpen();*/
  return 0;
}