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


/*turnTo(0);
turnTo(180);
turnTo(0);
turnTo(180);
turnTo(365-45);
turnTo(30);
turnTo(35);
turnTo(90);*/

//moveCV(36*2, -50, 60);

Point start = robot.location.getPos();
smartPointPointer result;
result.append(start);
Vector shifts[] = {Vector(-2.341099999999994, 13.800356249999986), Vector(-4.796399999999966, 26.175591666666648), Vector(-7.365900000000009, 37.12570625), Vector(-10.049600000000002, 46.65070000000001), Vector(-12.8475, 54.75057291666667), Vector(-15.759600000000002, 61.425325), Vector(-18.78590000000001, 66.67495625), Vector(-21.926399999999994, 70.49946666666666), Vector(-25.181100000000008, 72.89885625000001), Vector(-28.55, 73.873125), Vector(-31.684552083333323, 73.92784583333334), Vector(-34.236208333333316, 73.56859166666666), Vector(-36.20496875, 72.79536250000001), Vector(-37.59083333333334, 71.60815833333334), Vector(-38.393802083333334, 70.00697916666667), Vector(-38.613875, 67.99182499999999), Vector(-38.25105208333333, 65.56269583333334), Vector(-37.30533333333332, 62.719591666666666), Vector(-35.77671875, 59.46251249999999), Vector(-33.66520833333333, 55.79145833333333), Vector(-31.29198958333333, 52.32144374999998), Vector(-28.978249999999974, 49.66748333333333), Vector(-26.72398958333333, 47.82957708333334), Vector(-24.529208333333326, 46.807725000000005), Vector(-22.39390625, 46.60192708333334), Vector(-20.318083333333327, 47.212183333333336), Vector(-18.301739583333323, 48.63849375), Vector(-16.344874999999984, 50.880858333333336), Vector(-14.447489583333338, 53.93927708333334), Vector(-12.609583333333333, 57.81375), Vector(-11.2558375, 61.83811041666666), Vector(-10.810933333333306, 65.34619166666666), Vector(-11.274870833333344, 68.33799375000001), Vector(-12.647650000000006, 70.81351666666666), Vector(-14.929270833333334, 72.77276041666667), Vector(-18.119733333333325, 74.21572499999999), Vector(-22.21903750000001, 75.14241041666668), Vector(-27.227183333333333, 75.55281666666667), Vector(-33.144170833333334, 75.44694375), Vector(-39.97, 74.82479166666667), Vector(-46.682818749999996, 73.80293958333333), Vector(-52.26077499999998, 72.49796666666664), Vector(-56.703868750000005, 70.90987291666667), Vector(-60.0121, 69.03865833333333), Vector(-62.18546875, 66.88432291666666), Vector(-63.223975, 64.44686666666666), Vector(-63.127618749999996, 61.72628958333333), Vector(-61.89639999999999, 58.72259166666665), Vector(-59.53031875, 55.435772916666664), Vector(-56.029375, 51.865833333333335), Vector(-52.13111041666666, 48.617081250000005), Vector(-48.57306666666666, 46.293824999999984), Vector(-45.355243750000014, 44.89606458333333), Vector(-42.47764166666667, 44.42380000000001), Vector(-39.94026041666667, 44.87703125), Vector(-37.7431, 46.25575833333334), Vector(-35.88616041666668, 48.55998125000001), Vector(-34.36944166666666, 51.7897), Vector(-33.19294375, 55.94491458333333), Vector(-32.35666666666667, 61.025625), Vector(-31.85109375, 65.77325208333335), Vector(-31.666708333333325, 68.92921666666666), Vector(-31.80351041666667, 70.49351875), Vector(-32.261500000000005, 70.46615833333333), Vector(-33.040677083333335, 68.84713541666666), Vector(-34.141041666666666, 65.63645), Vector(-35.562593750000005, 60.834102083333335), Vector(-37.30533333333332, 54.44009166666667), Vector(-39.369260416666656, 46.45441875), Vector(-41.754375, 36.87708333333333), Vector(-43.82900833333333, 27.576920833333325), Vector(-44.96149166666666, 20.422766666666647), Vector(-45.15182500000001, 15.414620833333338), Vector(-44.40000833333333, 12.552483333333331), Vector(-42.706041666666664, 11.836354166666666), Vector(-40.06992499999999, 13.26623333333333), Vector(-36.49165833333333, 16.84212083333335), Vector(-31.971241666666664, 22.564016666666674), Vector(-26.508675000000004, 30.431920833333326), Vector(-20.103958333333335, 40.44583333333333), Vector(-14.644960416666654, 50.521604166666656), Vector(-12.019549999999983, 58.57508333333332), Vector(-12.22772708333333, 64.60627083333334), Vector(-15.269491666666681, 68.61516666666667), Vector(-21.14484375, 70.60177083333333), Vector(-29.853783333333336, 70.56608333333334), Vector(-41.396310416666665, 68.50810416666667), Vector(-55.772425000000005, 64.42783333333333), Vector(-72.98212708333334, 58.32527083333333), Vector(-93.02541666666667, 50.20041666666667)};
moveAbs(start);
for(int i=0; i<90; i++){
     result.append(shifts[i] + start);
}
tracePath(result, 20);


  while(true){
    wait(200, msec);
  }
}