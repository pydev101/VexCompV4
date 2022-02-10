#include "Intergration.h"

#define specialSkillsMode 0
#define armUpSpeed 100
#define armDownSpeed -100
#define fullUp 1520
#define holdDelayMsec 125

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
  #if specialSkillsMode
    double pressStartTime = Brain.timer(timeUnits::msec);
    int beingPressed = 0;
  #endif

  while(true){
    setLeft(Controller1.Axis3.position(percent));
    setRight(Controller1.Axis2.position(percent));

    #if specialSkillsMode
      double now = Brain.timer(timeUnits::msec);

      if(Controller1.ButtonR1.pressing()){
        if(beingPressed == 1){
          if(now-pressStartTime > holdDelayMsec){
            setArm(armUpSpeed);
          }
        }else{
          beingPressed = 1;
          pressStartTime = now;
        }
      }else{
        if(beingPressed == 1){
          if(now-pressStartTime > holdDelayMsec){
            setArm(0);
          }else{
            arm.startRotateTo(fullUp, rotationUnits::deg, 100, velocityUnits::pct);
          }
          beingPressed = 0;
        }
      }

      if(Controller1.ButtonR2.pressing()){
        if(beingPressed == -1){
          if(now-pressStartTime > holdDelayMsec){
            setArm(armDownSpeed);
          }
        }else{
          beingPressed = -1;
          pressStartTime = now;
        }
      }else{
        if(beingPressed == -1){
          if(now-pressStartTime > holdDelayMsec){
            setArm(0);
          }else{
            arm.startRotateTo(0, rotationUnits::deg, 100, velocityUnits::pct);
          }
          beingPressed = 0;
        }
      }
    #else
      if(Controller1.ButtonR1.pressing()){
        setArm(armUpSpeed);
      }else if(Controller1.ButtonR2.pressing()){
        setArm(armDownSpeed);
      }else{
        setArm(0);
      }
    #endif

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

    #if specialSkillsMode == 0
      updateControllerDisplay();
    #endif

    wait(20, timeUnits::msec);
  }
}