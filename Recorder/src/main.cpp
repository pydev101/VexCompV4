// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// frontLeft            motor         13              
// frontRight           motor         12              
// backLeft             motor         11              
// backRight            motor         14              
// intakeLeft           motor         1               
// intakeRight          motor         5               
// liftT                motor         3               
// liftB                motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Khaled Zuaiter                                   */
/*    Created:      Sat Nov 21 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// frontLeft            motor         13              
// frontRight           motor         12              
// backLeft             motor         11              
// backRight            motor         14              
// intakeLeft           motor         1               
// intakeRight          motor         2               
// liftT                motor         3               
// liftB                motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

char buffer[50000];

void writeCommand(double cmd, double arg, bool isStart=false){
    if(isStart){
       sprintf(buffer, "%g,%g;", cmd, arg);
       return;
    }
    sprintf(buffer, "%s%g,%g;", buffer, cmd, arg);
}

double abs(double x){
  if(x<0){x=x*-1;}
  return x;
}

void stopMotors(){
  frontLeft.setVelocity(0, percentUnits::pct);
  frontRight.setVelocity(0, percentUnits::pct);
  backRight.setVelocity(0, percentUnits::pct);
  backLeft.setVelocity(0, percentUnits::pct);
  frontLeft.stop();
  frontRight.stop();
  backRight.stop();
  backLeft.stop();
}

void startSpin(){
  frontLeft.spin(forward);
  frontRight.spin(forward);
  backRight.spin(forward);
  backLeft.spin(forward);
}

void intake(int d=0){
  intakeRight.setVelocity(100*d, pct);
  intakeLeft.setVelocity(100*d, pct);
  
  if(d==0){
    intakeLeft.stop();
    intakeRight.stop();
  }else{
    intakeRight.spin(fwd);
    intakeLeft.spin(fwd);
  }
}

void usercontrol(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  liftT.setBrake(brakeType::hold);
  liftB.setBrake(brakeType::hold);

  double lV = 0;
  double lH = 0;
  double rV = 0;
  double rH = 0;
  int threshold = 5;
  int intakeToggle = 0;
  int modeLiftA = 0;
  int modeLiftB = 0;

  while (true) {
    lV = Controller1.Axis3.position();
    lH = Controller1.Axis4.position();
    rV = Controller1.Axis2.position();
    rH = Controller1.Axis1.position();

    if((abs(lV)<threshold)&&(abs(lH)<threshold)&&(abs(rV)<threshold)&&(abs(rH)<threshold)){
      stopMotors();
      writeCommand(0, 0);
    }else{
      startSpin();
      writeCommand(1, 0);
    }

    frontLeft.setVelocity(lV+lH, velocityUnits::pct);
    frontRight.setVelocity(rV-rH, velocityUnits::pct);
    backLeft.setVelocity(lV-lH, velocityUnits::pct);
    backRight.setVelocity(rV+rH, velocityUnits::pct);

    writeCommand(2, lV+lH);
    writeCommand(3, rV-rH);
    writeCommand(4, lV-lH);
    writeCommand(5, rV+rH);

    if(Controller1.ButtonR1.pressing()){
      if(!(modeLiftA == 1)){
        liftT.spin(fwd);
        liftT.setVelocity(100, pct);
        writeCommand(6, 1);
        modeLiftA = 1;
      }
    }else if(Controller1.ButtonR2.pressing()){
      if(!(modeLiftA == 2)){
        liftT.spin(fwd);
        liftT.setVelocity(-100, pct);
        writeCommand(6, 2);
        modeLiftA = 2;
      }
    }else{
      if(!(modeLiftA == 0)){
        liftT.setVelocity(0, pct);
        liftT.stop();
        writeCommand(6, 0);
        modeLiftA = 0;
      }
    }

    if(Controller1.ButtonL1.pressing()){
      if(!(modeLiftB == 1)){
        liftB.spin(fwd);
        liftB.setVelocity(100, pct);
        writeCommand(7, 1);
        modeLiftB = 1;
      }
    }else if(Controller1.ButtonL2.pressing()){
      if(!(modeLiftB == 2)){
        liftB.spin(fwd);
        liftB.setVelocity(-100, pct);
        writeCommand(7, 2);
        modeLiftB = 2;
      }
    }else{
      if(!(modeLiftB == 0)){
        liftB.setVelocity(0, pct);
        liftB.stop();
        writeCommand(7, 0);
        modeLiftB = 0;
      }
    }

    if(Controller1.ButtonUp.pressing()){
      if(intakeToggle == 1){
        intake(0);
        intakeToggle = 0;
        writeCommand(8, 0);
      }else{
        intake(1);
        intakeToggle = 1;
        writeCommand(8, 1);
      }
      writeCommand(9, 250);
      wait(250, msec);
    }else if(Controller1.ButtonDown.pressing()){
      if(intakeToggle == -1){
        intake(0);
        intakeToggle = 0;
        writeCommand(8, 0);
      }else{
        intakeToggle = -1;
        intake(-1);
        writeCommand(8, 2);
      }
      writeCommand(9, 250);
      wait(250, msec);
    }

    if(Controller1.ButtonB.pressing()){
      break;
    }

    writeCommand(9, 20);
    wait(20, msec);
  }
}

void save(){
  int size = sizeof(char)*(strlen(buffer)+1);
  char* charToPrint = (char*)malloc(size);
  strcpy (charToPrint, buffer);
  Brain.SDcard.savefile("recorded.csv", reinterpret_cast<unsigned char*>(charToPrint), size);
  free(charToPrint);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  writeCommand(0,0,true);
  usercontrol();
  save();
}

/*
0: Stop           | Arg: 0
1: Spin           | Arg: 0 
2: Set frontLeft  | Arg: Speed (PCT)
3: Set frontRight | Arg: Speed (PCT)
4: Set backLeft   | Arg: Speed (PCT)
5: Set backRight  | Arg: Speed (PCT)
6: Set liftT      | Arg: 0-2 Stop, Forward, Reverse
7: Set liftB      | Arg: 0-2 Stop, Forward, Reverse
8: Set Intake     | Arg: 0-2 Stop, Forward, Reverse
9: Wait           | Arg: Time (Msec)
*/
