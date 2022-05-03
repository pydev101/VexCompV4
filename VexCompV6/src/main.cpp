// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         11              
// rightA               motor         3               
// rightB               motor         4               
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         20              
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      15              
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

#define COMPETITION 0

// A global instance of competition
#if COMPETITION
  competition Competition;
#endif


//Pre Auto function sets up motor and tracking functions before compeition starts; Also starts GUI program to allow selection of program to be used during that round
void pre_auton(void) {
  vexcodeInit();
  frontPne.set(false);
  backPne.set(true);
  arm.setStopping(hold);

  leftA.setStopping(brakeType::coast);
  leftB.setStopping(brakeType::coast);
  leftC.setStopping(brakeType::coast);
  rightA.setStopping(brakeType::coast);
  rightB.setStopping(brakeType::coast);
  rightC.setStopping(brakeType::coast);

  if(Brain.SDcard.isInserted()){
    Brain.Screen.clearScreen();
    Brain.Screen.drawImageFromFile("photoK.png", 0, 0);
  }

  Inertial.startCalibration();
  wait(1000, msec);
  while(Inertial.isCalibrating()){wait(20, msec);}
  wait(1000, msec);

  Brain.Screen.clearScreen();

  #if COMPETITION
    BrainGUIProgram();
  #else
    Inertial.setHeading(360-startingHead, rotationUnits::deg); //90 deg CCW but inertial sensor only measures in CW
    resetEncoders();
    task traker = task(trakerFunction);
    wait(updateTime+1, msec);
  #endif
}


//Main functon
int main() {
  pre_auton();

  #if COMPETITION
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);
  #endif

//-------TODO
/*
  Change units of length from inches to tiles
    --Tune PI after
    
  Changes grid from based on starting location to absolute location

  Move sensor away from floor and torward the edge of the robot to minimize small shifts that occur in the center

  Have PID rotation while in linear motion if greator then X degrees to avoid thew wiggles

  Test and tune reverse rot PID;
*/

blueLeft(0);

/*
  moveAbs(0, 25);
  moveAbs(25, 25);
  moveAbs(25, 0);
  moveAbs(0, 0);

  moveAbs(25, 0, false);
  moveAbs(25, 25, false);
  moveAbs(0, 25, false);
  moveAbs(0, 0, false);*/

/*turnTo(0);
turnTo(180);
turnTo(0);
turnTo(180);
turnTo(365-45);
turnTo(30);
turnTo(35);
turnTo(90);*/

 //moveCV(36*20, 0, 60);


  while(true){
    wait(200, msec);
  }
}