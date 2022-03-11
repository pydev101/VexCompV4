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
  backPne.set(false);
  arm.setStopping(hold);

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

  Work on Beizer curve functions
    
  Changes grid from based on starting location to absolute location

  Move sensor away from floor and torward the edge of the robot to minimize small shifts that occur in the center

  Have PID rotation while in linear motion if greator then X degrees to avoid thew wiggles

  Test and tune reverse rot PID; P for rotation should be about 16 and it should use an I of about 15; 
*/

/*
turnTo(0);
turnTo(180);
turnTo(365-45);
turnTo(30);
turnTo(35);*/


Point start = robot.location.getPos();
smartPointPointer result;
result.append(start);
Vector shifts[] = {Vector(-4.617962500000006, 6.0121541666666625), Vector(-7.908349999999989, 11.867283333333305), Vector(-9.871162500000002, 17.56538750000001), Vector(-10.506400000000006, 23.106466666666666), Vector(-9.8140625, 28.490520833333335), Vector(-7.7941499999999975, 33.71755), Vector(-4.4466624999999995, 38.78755416666666), Vector(0.22840000000000865, 43.70053333333332), Vector(6.231037499999999, 48.45648749999999), Vector(13.56125, 53.055416666666666), Vector(20.61310000000001, 57.756649999999986), Vector(25.780650000000016, 62.81951666666666), Vector(29.063900000000007, 68.24401666666667), Vector(30.462850000000003, 74.03014999999999), Vector(29.9775, 80.17791666666666), Vector(27.607850000000006, 86.68731666666667), Vector(23.353899999999992, 93.55835), Vector(17.215650000000004, 100.79101666666666), Vector(9.193099999999996, 108.38531666666667), Vector(-0.71375, 116.34125)};
moveAbs(start);
for(int i=0; i<20; i++){
     result.append(shifts[i] + start);
}
tracePath(result, 80);

  while(true){
    wait(200, msec);
  }
}