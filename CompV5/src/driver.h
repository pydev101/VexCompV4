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
/*
    if(Controller1.ButtonR2.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(100, pct);
    }else if(Controller1.ButtonX.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(-100, pct);
    }else{
      liftT.setVelocity(0, pct);
      liftT.stop();
    }*/

    wait(20, msec);
  }
}