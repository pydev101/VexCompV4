// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         4               
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      10              
// backPne              digital_out   A               
// frontPne             digital_out   H               
// BackCam              vision        5               
// LimitBack            limit         E               
// ---- END VEXCODE CONFIGURED DEVICES ----


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\mainuser                                         */
/*    Created:      Fri Nov 05 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "driver.h"
#include "programs.h"

#define COMPETITION 1

// A global instance of competition
#if COMPETITION
  competition Competition;
#endif

//Pre Auto function sets up motor and tracking functions before compeition starts; Also starts GUI program to allow selection of program to be used during that round
void pre_auton(void) {
  vexcodeInit();
  frontPne.set(false);
  backPne.set(false);
  arm.setStopping(hold);

  Inertial.startCalibration();
  while(Inertial.isCalibrating()){wait(10, msec);}
  Inertial.setHeading(360-startingHead, rotationUnits::deg); //90 deg CCW but inertial sensor only measures in CW

  resetEncoders();

  task traker = task(trakerFunction);
  #if COMPETITION
    BrainGUIProgram();
  #endif
}

//Main functon
int main() {
  pre_auton();

  #if COMPETITION
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);
  #endif

  //TODO Increase P value of linear track i norder to boost speed

  //OneMinute(0);

  /*std::cout << robot.location.pos.x << ", " << robot.location.pos.y << std::endl;
  moveAbs(0, 20);
  std::cout << robot.location.pos.x << ", " << robot.location.pos.y << std::endl;
  wait(2000, msec);
  std::cout << robot.location.pos.x << ", " << robot.location.pos.y << std::endl;*/
  //blueLeft(0);

  while(true){
    wait(200, msec);
  }
}

/*moveAbs(0, 20);
moveAbs(-20, 20);
moveAbs(-20, 0, false);
moveAbs(0, 20);
turnTo(90, true);
moveAbs(0, 0, false);*/
//smartPointPointer path = generatePath(Point(0,0), Point(0,24.8), Point(-35.4,3.85), Point(31.53, 12.35), 25);
//tracePath(path);
//std::cout << "Complete" << std::endl;

/*
TODO

-implement a path tracer which follows a path without worrying about slowing down to a stop each time
-Look at vex::serial_link as part of logger
--Cameras need to be done once above is done; Set the target poisition to the goal
*/