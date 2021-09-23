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

    wait(20, msec);
  }
}