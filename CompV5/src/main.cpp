// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorLF              motor         13              
// motorLB              motor         14              
// motorRF              motor         11              
// motorRB              motor         12              
// ISensor              inertial      15              
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

  /*
  Checklist:
  DONE! 1) Motors: Wiring, Speed, and Direction
  DONE! 2) Encoders: getEncoder returns positive in the fwd direction; getHeading returns the Heading of the robot correctly in radians
  3) Grid: Encoders map correctly to the grid and positive is reflected accuratly; Error values should be verified as well
  4) Rotational Motion: Gain is set; Robot can turn in the shortest direction, both directions, and accuratly arrive at target; Close values should have a low speed value for use in grid movement
  5) Linear Motion: Gain is tuned and direction, speed, acceleration are double checked or set
  6) Grid Move: A target is set on the grid and the robot must arrive accuratly and turn to and stay straight

  Future Features:
  -Self adjusting gain and max acceleration through gradient decsent
  */

  //TODO: Set motor speed fwd and reverse; above and below max to test functions

  //Need to check encoders then heading
  //setDPS(minSpeed+3, minSpeed+3);AAAAAAAAAA

  motorLB.setBrake(brakeType::coast);
  motorLF.setBrake(brakeType::coast);
  motorRB.setBrake(brakeType::coast);
  motorRF.setBrake(brakeType::coast);
  setDPS(0,0);

  robot.setTRealitive(20, -20);

  //POLAR ERROR IS MESSING UP

  Point p;
  while(true){
    p = robot.getPos();
    std::cout << "(" << p.x << ", " << p.y << ", " << p.head << ")" << std::endl;
    robot.getError(POLAR);
    //std::cout << "Error" << robot.getError(POLAR) << std::endl;
    wait(500, msec);
  }

  return 0;
}