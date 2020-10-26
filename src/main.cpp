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
// liftL                motor         3               
// liftR                motor         4               
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

competition Competition;

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

      if(Controller1.ButtonR1.pressing()){
        intakeRight.spin(fwd);
        intakeLeft.spin(fwd);
        intakeRight.setVelocity(100, pct);
        intakeLeft.setVelocity(100, pct);
      }else if(Controller1.ButtonR2.pressing()){
        intakeRight.spin(fwd);
        intakeLeft.spin(fwd);
        intakeRight.setVelocity(-100, pct);
        intakeLeft.setVelocity(-100, pct);
      }else{
        intakeLeft.stop();
        intakeRight.stop();
      }

      if(Controller1.ButtonRight.pressing()){
        liftR.spin(fwd);
        liftL.spin(fwd);
        liftR.setVelocity(50, pct);
        liftL.setVelocity(50, pct);
      }else if(Controller1.ButtonLeft.pressing()){
        liftR.spin(fwd);
        liftL.spin(fwd);
        liftR.setVelocity(-50, pct);
        liftL.setVelocity(-50, pct);
      }else{
        liftL.stop();
        liftR.stop();
      }

      wait(20, msec);
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  move(24, 36*3, false);
  //move(-12, 15);
  //move(-X, -Y, false);

  //rotateSensor(PI/2);

  task updateTask(trackingTask);

  std::cout << "Heading | X | Y" << std::endl;
  while (true) {
    testVarMutex.lock();
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print("Heading | X | Y");
    Controller1.Screen.newLine();
    Controller1.Screen.print(Heading*(180/PI));
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(X/measureWheelDegsOverInches);
    Controller1.Screen.print(" | ");
    Controller1.Screen.print(Y/measureWheelDegsOverInches);

    std::cout << (Heading*(180/PI)) << ", " << X/measureWheelDegsOverInches << ", " << Y/measureWheelDegsOverInches << std::endl;
    testVarMutex.unlock();
    wait(500, msec);
  }
}