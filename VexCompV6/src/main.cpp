// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         4               
// arm                  motor         5               
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         10              
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

#include "vex.h"

using namespace vex;

void setM(motor m, int speed){
  m.setVelocity(speed, percent);
  if(speed == 0){
    m.stop();
  }else{
    m.spin(forward);
  }
}
void setLeft(int speed){
  setM(leftA, speed);
  setM(leftB, speed);
  setM(leftC, speed);
}
void setRight(int speed){
  setM(rightA, speed);
  setM(rightB, speed);
  setM(rightC, speed);
}
void setArm(int speed){
  setM(arm, speed);
}
void setIntake(int speed){
  setM(intakeM, speed);
}
void toggleFront(bool currState){
  static bool active = false;
  static bool press = false;
  if(currState != press){
    press = currState;
    if(currState){
      active = !active;
      frontPne.set(active);
    }
  }
}
void toggleBack(bool currState){
  static bool active = false;
  static bool press = false;
  if(currState != press){
    press = currState;
    if(currState){
      active = !active;
      backPne.set(active);
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  //TODO NEED TO CALIBRATE INERTIAL SENSOR

  arm.setStopping(hold);

  int intakeMode = 0;
  bool intakePressing = false;

  while(true){
    setLeft(Controller1.Axis3.position(percent));
    setRight(Controller1.Axis2.position(percent));

    if(Controller1.ButtonR1.pressing()){
      setArm(100);
    }else if(Controller1.ButtonR2.pressing()){
      setArm(-60);
    }else{
      setArm(0);
    }

    toggleFront(Controller1.ButtonL1.pressing());
    toggleBack(Controller1.ButtonL2.pressing());

    if(Controller1.ButtonX.pressing() && !intakePressing){
      intakePressing = true;
      if(intakeMode != 1){
        intakeMode = 1;
        setIntake(60);
      }else{
        intakeMode = 0;
        setIntake(0);
      }
    }else if(Controller1.ButtonA.pressing() && !intakePressing){
      intakePressing = true;
      if(intakeMode != -1){
        intakeMode = -1;
        setIntake(-60);
      }else{
        intakeMode = 0;
        setIntake(0);
      }
    }else{
      intakePressing = false;
    }
  }
}
