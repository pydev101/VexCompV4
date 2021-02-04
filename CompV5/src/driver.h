#include "OdomControls/MotionControl.h"

void usercontrol(void) {
  setBreaks(brakeType::coast);
  setDPS(0,0);
  
  int threshold = 5;
  double leftSide;
  double rightSide;
  double driverMaxSpeed = maxSpeed;

  while (true) {
    leftSide = Controller1.Axis3.position();
    rightSide = Controller1.Axis2.position();

    if((_MOTORABS(leftSide)<threshold)&&(_MOTORABS(rightSide)<threshold)){
      setDPS(0,0);
    }else{
      setDPS((leftSide/100)*driverMaxSpeed, (rightSide/100)*driverMaxSpeed);
    }

    if(Controller1.ButtonR1.pressing()){
      setTop(1);
    }else if(Controller1.ButtonUp.pressing()){
      setTop(-1);
    }else{
      setTop(0);
    }

    if(Controller1.ButtonR2.pressing()){
      setBottom(1);
    }else if(Controller1.ButtonX.pressing()){
      setBottom(-1);
    }else{
      setBottom(0);
    }

    if(Controller1.ButtonL1.pressing()){
      setIntake(1);
    }else if(Controller1.ButtonDown.pressing()){
      setIntake(-1);
    }else{
      setIntake(0);
    }

    wait(20, msec);
  }
}