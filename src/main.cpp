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

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  
  setDPS(0);

  resetHeading();
  resetEncoders();
}


void autonomous(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
}


bool tank = true;
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

  while (true) {
    if(tank){
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
      wait(20, msec);
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  //move(24, 36*3, false);
  //move(-12, 15);
  //move(-X, -Y, false);

  rotateSensor(PI/2);

  while (true) {
    wait(100, msec);
  }
}