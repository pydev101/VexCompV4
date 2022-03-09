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

  smartPointPointer path = generatePath(Point(0, 0), Point(0, 60), Point(20, 20), Point(-20, 40));
  tracePath(path);

  //TODO
    /*
    Change units of length from inches to tiles
    --Tune PI after

    Work on Beizer curve functions
    
    Changes grid from based on starting location to absolute location

    Move sensor away from floor and torward the edge of the robot to minimize small shifts that occur in the center

    Have PID rotation while in linear motion if greator then X degrees to avoid thew wiggles
    */

Point start = robot.location.getPos();
smartPointPointer result;
result.append(start);
Vector shifts[] = {Vector(6.511779, 6.511779), Vector(12.72378, 12.72378), Vector(18.63601, 18.63601), Vector(24.24847, 24.24847), Vector(29.56115, 29.56115), Vector(34.57405, 34.57405), Vector(39.28718, 39.28718), Vector(43.70053, 43.70053), Vector(47.81411, 47.81411), Vector(51.62792, 51.62792), Vector(55.28945, 55.28945), Vector(58.94623, 58.94623), Vector(62.59825, 62.59825), Vector(66.24552, 66.24552), Vector(69.88802, 69.88802), Vector(73.52577, 73.52577), Vector(77.15875, 77.15875), Vector(80.78698, 80.78698), Vector(84.41045, 84.41045), Vector(88.02917, 88.02917), Vector(91.80253, 91.80253), Vector(95.88993, 95.88993), Vector(100.2914, 100.2914), Vector(105.0069, 105.0069), Vector(110.0365, 110.0365), Vector(115.3801, 115.3801), Vector(121.0377, 121.0377), Vector(127.0094, 127.0094), Vector(133.2952, 133.2952), Vector(139.895, 139.895)};
for(int i=0; i<30; i++){
     result.append(shifts[i] + start);
}
tracePath(result);

  while(true){
    wait(200, msec);
  }
}



/*moveAbs(0, 20);
moveAbs(-20, 20);
moveAbs(-20, 0, false);
moveAbs(0, 20);
turnTo(90, true);
moveAbs(0, 0, false);*/
//smartPointPointer path = generatePath(Point(0,0), Point(0,24.8), Point(-35.4,3.85), Point(31.53, 12.35), 25);
//tracePath(path);
//std::cout << "Complete" << std::endl;

/*
TODO

-implement a path tracer which follows a path without worrying about slowing down to a stop each time
-Look at vex::serial_link as part of logger
--Cameras need to be done once above is done; Set the target poisition to the goal
*/