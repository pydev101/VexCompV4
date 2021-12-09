// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         11              
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      10              
// backPne              digital_out   A               
// frontPne             digital_out   H               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         11              
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      10              
// backPne              digital_out   A               
// frontPne             digital_out   H               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         11              
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      10              
// backPne              digital_out   A               
// frontPne             digital_out   H               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         11              
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      19              
// backPne              digital_out   A               
// frontPne             digital_out   H               
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

// A global instance of competition
//competition Competition;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  frontPne.set(false);
  backPne.set(true);
  arm.setStopping(hold);

  Inertial.startCalibration();
  while(Inertial.isCalibrating()){wait(10, msec);}
  Inertial.setHeading(270, rotationUnits::deg); //90 deg CCW but inertial sensor only measures in CW

  resetEncoders();

  task traker = task(trakerFunction);
  //BrainGUIProgram();
}

//
// Main will set up the competition functions and callbacks.
//

int main() {
  // Set up callbacks for autonomous and driver control periods.
  pre_auton();
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  wait(3, timeUnits::sec);
  autonomous();

  //move(36,0);
  //move(0,36);
  //move(-36,36);
  //move(-36,0);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
