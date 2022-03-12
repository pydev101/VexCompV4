// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftA                motor         1               
// rightA               motor         3               
// rightB               motor         4               
// arm                  motor         12              
// intakeM              motor         7               
// leftC                motor         8               
// rightC               motor         9               
// leftB                motor         21              
// Controller1          controller                    
// Inertial             inertial      15              
// backPne              digital_out   A               
// frontPne             digital_out   H               
// BackCam              vision        5               
// LimitBack            limit         E               
// ---- END VEXCODE CONFIGURED DEVICES ----


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\mainuser                                         */
/*    Created:      Fri Nov 05 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "driver.h"
#include "programs.h"

#define COMPETITION 0

// A global instance of competition
#if COMPETITION
  competition Competition;
#endif


//Pre Auto function sets up motor and tracking functions before compeition starts; Also starts GUI program to allow selection of program to be used during that round
void pre_auton(void) {
  vexcodeInit();
  frontPne.set(false);
  backPne.set(false);
  arm.setStopping(hold);

  if(Brain.SDcard.isInserted()){
    Brain.Screen.clearScreen();
    Brain.Screen.drawImageFromFile("photoK.png", 0, 0);
  }

  Inertial.startCalibration();
  wait(1000, msec);
  while(Inertial.isCalibrating()){wait(20, msec);}
  wait(1000, msec);

  Brain.Screen.clearScreen();

  #if COMPETITION
    BrainGUIProgram();
  #else
    Inertial.setHeading(360-startingHead, rotationUnits::deg); //90 deg CCW but inertial sensor only measures in CW
    resetEncoders();
    task traker = task(trakerFunction);
    wait(updateTime+1, msec);
  #endif
}


//Main functon
int main() {
  pre_auton();

  #if COMPETITION
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);
  #endif

//-------TODO
/*
  Change units of length from inches to tiles
    --Tune PI after
    
  Changes grid from based on starting location to absolute location

  Move sensor away from floor and torward the edge of the robot to minimize small shifts that occur in the center

  Have PID rotation while in linear motion if greator then X degrees to avoid thew wiggles

  Test and tune reverse rot PID;
*/

/*
turnTo(0);
turnTo(180);
turnTo(365-45);
turnTo(30);
turnTo(35);*/


Point start = robot.location.getPos();
smartPointPointer result;
result.append(start);
Vector shifts[] = {Vector(-8.863585416666666, 0.010706249999990263), Vector(-16.80167499999999, 0.4234916666666331), Vector(-23.814268749999993, 1.2383562499999963), Vector(-29.901366666666668, 2.4552999999999847), Vector(-35.06296875, 4.074322916666667), Vector(-39.299075, 6.095425000000001), Vector(-42.60968541666667, 8.518606250000015), Vector(-44.9948, 11.343866666666655), Vector(-46.45441875, 14.571206250000001), Vector(-46.98854166666666, 18.200625), Vector(-46.71017916666666, 21.98587916666666), Vector(-45.732341666666656, 25.680724999999985), Vector(-44.05502916666667, 29.28516250000001), Vector(-41.67824166666666, 32.79919166666667), Vector(-38.601979166666666, 36.222812499999996), Vector(-34.82624166666667, 39.556025), Vector(-30.351029166666663, 42.798829166666664), Vector(-25.17634166666665, 45.951224999999994), Vector(-19.302179166666665, 49.013212499999995), Vector(-12.728541666666667, 51.984791666666666), Vector(-6.855568749999986, 55.062243749999986), Vector(-3.083399999999968, 58.44184999999998), Vector(-1.412035416666667, 62.123610416666665), Vector(-1.8414750000000022, 66.10752500000001), Vector(-4.37171875, 70.39359375), Vector(-9.00276666666666, 74.98181666666666), Vector(-15.734618750000012, 79.87219375000001), Vector(-24.567275, 85.06472500000001), Vector(-35.50073541666666, 90.55941041666667), Vector(-48.535, 96.35625)};
moveAbs(start);
for(int i=0; i<30; i++){
     result.append(shifts[i] + start);
}
tracePath(result);

  while(true){
    wait(200, msec);
  }
}