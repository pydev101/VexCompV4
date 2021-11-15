// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// leftA                motor         1               
// leftB                motor         2               
// rightA               motor         3               
// rightB               motor         4               
// arm3M                motor         5               
// arm4M                motor         6               
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// Inertial             inertial      10              
// pne                  digital_out   A               
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
void set3Bar(int speed){
  setM(arm3M, speed);
}
void set4Bar(int speed){
  setM(arm4M, speed);
}
void setIntake(int speed){
  setM(intakeM, speed);
}



int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  //TODO NEED TO CALIBRATE INERTIAL SENSOR

  arm4M.setStopping(hold);
  arm3M.setStopping(hold);

  //pne.set(true);

  while(true){
    setLeft(Controller1.Axis3.position(percent));
    setRight(Controller1.Axis2.position(percent));

    if(Controller1.ButtonR1.pressing()){
      set4Bar(100);
    }else if(Controller1.ButtonR2.pressing()){
      set4Bar(-60);
    }else{
      set4Bar(0);
    }

    if(Controller1.ButtonL1.pressing()){
      set3Bar(100);
    }else if(Controller1.ButtonL2.pressing()){
      set3Bar(-60);
    }else{
      set3Bar(0);
    }

    if(Controller1.ButtonX.pressing()){
      setIntake(100);
    }else if(Controller1.ButtonA.pressing()){
      setIntake(-60);
    }else{
      setIntake(0);
    }
  }
}
