// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontLeft            motor         13              
// frontRight           motor         12              
// backLeft             motor         11              
// backRight            motor         14              
// rightEncoder         encoder       C, D            
// leftEncoder          encoder       E, F            
// horEncoder           encoder       A, B            
// ISensor              inertial      20              
// Controller1          controller                    
// intakeLeft           motor         1               
// intakeRight          motor         2               
// liftT                motor         3               
// liftB                motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----
                 
/*----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                               
//    Author:       Carson E                                 
//    Created:      Thu Sep 10 2020                                           
//    Description:  Primary file that implements controls and movement                                           
//                                                                            
//--------------------------------------------------------------------------*/

#include "AutoPrograms.h"

//competition Competition;

void pre_auton(void) {
  vexcodeInit();
  preSettings();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  
  setDPS(0);

  resetHeading();
  resetEncoders();
  task updateTask(trackingTask);

  BrainGUIProgram();
}


void autonomous(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  entries[indexAuto].function(entries[indexAuto].mod);
}

void usercontrol(void) {
  frontLeft.setBrake(brakeType::coast);
  frontRight.setBrake(brakeType::coast);
  backLeft.setBrake(brakeType::coast);
  backRight.setBrake(brakeType::coast);

  double lV = 0;
  double lH = 0;
  double rV = 0;
  double rH = 0;
  int threshold = 5;
  int intakeToggle = 0;

  while (true) {
    lV = Controller1.Axis3.position();
    lH = Controller1.Axis4.position();
    rV = Controller1.Axis2.position();
    rH = Controller1.Axis1.position();

    if((abs(lV)<threshold)&&(abs(lH)<threshold)&&(abs(rV)<threshold)&&(abs(rH)<threshold)){
      stopMotors();
    }else{
      startSpin();
    }

    frontLeft.setVelocity(lV+lH, velocityUnits::pct);
    frontRight.setVelocity(rV-rH, velocityUnits::pct);
    backLeft.setVelocity(lV-lH, velocityUnits::pct);
    backRight.setVelocity(rV+rH, velocityUnits::pct);

    if(Controller1.ButtonR1.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(100, pct);
    }else if(Controller1.ButtonR2.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(-100, pct);
    }else{
      liftT.setVelocity(0, pct);
      liftT.stop();
    }

    if(Controller1.ButtonL1.pressing()){
      liftB.spin(fwd);
      liftB.setVelocity(100, pct);
    }else if(Controller1.ButtonL2.pressing()){
      liftB.spin(fwd);
      liftB.setVelocity(-100, pct);
    }else{
      liftB.setVelocity(0, pct);
      liftB.stop();
    }

    if(Controller1.ButtonUp.pressing()){
      if(intakeToggle == 1){
        intake(0);
        intakeToggle = 0;
      }else{
        intake(1);
        intakeToggle = 1;
      }
      wait(250, msec);
    }else if(Controller1.ButtonDown.pressing()){
      if(intakeToggle == -1){
        intake(0);
        intakeToggle = 0;
      }else{
        intakeToggle = -1;
        intake(-1);
      }
      wait(250, msec);
    }

    wait(20, msec);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  entries[indexAuto].function(entries[indexAuto].mod);
  
  //std::cout << "Heading | X | Y" << std::endl;
  while (true) {
    /*testVarMutex.lock();
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print("Heading | X | Y");

    Controller1.Screen.newLine();
    Controller1.Screen.print(Heading*(180/PI));
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(X/measureWheelDegsOverInches);
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(Y/measureWheelDegsOverInches);

    Controller1.Screen.newLine();
    Controller1.Screen.print(tHead*(180/PI));
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(Xt/measureWheelDegsOverInches);
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(Yt/measureWheelDegsOverInches);

    //std::cout << (Heading*(180/PI)) << ", " << X/measureWheelDegsOverInches << ", " << Y/measureWheelDegsOverInches << std::endl;
    testVarMutex.unlock();*/
    wait(1000, msec);
  }
}