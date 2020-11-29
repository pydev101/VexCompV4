/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Jarrrett                                         */
/*    Created:      Wed Nov 25 2020                                           */
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
#include <stdio.h>
#include <iostream>

using namespace vex;

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

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  int size = Brain.SDcard.size("recorded.csv");
  int length = size/sizeof(uint8_t);
  uint8_t* rawFileData = (uint8_t*)malloc(size);
  char* charList = (char*)malloc(sizeof(char)*length);

  Brain.SDcard.loadfile("recorded.csv", rawFileData, size);

  for(int i=0; i<length; i++){
      charList[i] = static_cast<char>(rawFileData[i]);
  }
  free(rawFileData);

  frontLeft.setBrake(brakeType::coast);
  frontRight.setBrake(brakeType::coast);
  backLeft.setBrake(brakeType::coast);
  backRight.setBrake(brakeType::coast);
  liftT.setBrake(brakeType::hold);
  liftB.setBrake(brakeType::hold);

  int lastIndex = 0;
  for(int i=0; i<length; i++){
    float cmd = 0;
    float arg = 0;

    if(charList[i] == ';'){
      sscanf(charList+lastIndex, "%g,%g", &cmd, &arg);
      lastIndex = i+1;
      std::cout << cmd << ": " << arg << std::endl;

      if(cmd == 0){
        stopMotors();
      }else if (cmd == 1) {
        startSpin();
      }else if(cmd == 2){
        frontLeft.setVelocity(arg, velocityUnits::pct);
      }else if(cmd == 3){
        frontRight.setVelocity(arg, velocityUnits::pct);
      }else if(cmd == 4){
        backLeft.setVelocity(arg, velocityUnits::pct);
      }else if(cmd == 5){
        backRight.setVelocity(arg, velocityUnits::pct);
      }else if(cmd == 6){
        if(arg==2){
          liftT.setVelocity(-100, pct);
          liftT.spin(fwd);
        }else if(arg == 1){
          liftT.setVelocity(100, pct);
          liftT.spin(fwd);
        }else{
          liftT.setVelocity(0, pct);
          liftT.stop();
        }
      }else if(cmd == 7){
        if(arg==2){
          liftB.setVelocity(-100, pct);
          liftB.spin(fwd);
        }else if(arg == 1){
          liftB.setVelocity(100, pct);
          liftB.spin(fwd);
        }else{
          liftB.setVelocity(0, pct);
          liftB.stop();
        }
      }else if(cmd == 8){
        if(arg==2){
          intake(-1);
        }else if(arg == 1){
          intake(1);
        }else{
          intake();
        }
      }else if(cmd == 9){
        wait(arg, msec);
      }
    }
  }

  free(charList);

  return 0;
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
