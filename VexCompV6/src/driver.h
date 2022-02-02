#include "Intergration.h"

//Defines Driver Program.

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

void usercontrol(void) {
  int intakeMode = 0;
  bool intakePressing = false;

  while(true){
    setLeft(Controller1.Axis3.position(percent));
    setRight(Controller1.Axis2.position(percent));

    if(Controller1.ButtonR1.pressing()){
      setArm(100);
    }else if(Controller1.ButtonR2.pressing()){
      setArm(-80);
    }else{
      setArm(0);
    }

    toggleFront(Controller1.ButtonL1.pressing());
    toggleBack(Controller1.ButtonL2.pressing());

    if(Controller1.ButtonX.pressing()){
      if(intakePressing == false){
        intakePressing = true;
        if(intakeMode == 1){
          intakeMode = 0;
          setIntake(0);
        }else{
          intakeMode = 1;
          setIntake(50);
        }
      }
    }else if(Controller1.ButtonA.pressing()){
      if(intakePressing == false){
        intakePressing = true;
        if(intakeMode == -1){
          intakeMode = 0;
          setIntake(0);
        }else{
          intakeMode = -1;
          setIntake(-50);
        }
      }
    }else{
      intakePressing = false;
    }
    updateControllerDisplay();
    wait(20, timeUnits::msec);
  }
}