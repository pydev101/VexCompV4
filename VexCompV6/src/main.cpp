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

  //smartPointPointer path = generatePath(Point(0, 0), Point(0, 60), Point(20, 20), Point(-20, 40));
  //tracePath(path);

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
Vector shifts[] = {Vector(14.30592916666668, 14.275000000000004), Vector(25.43805000000002, 28.45483333333334), Vector(33.3963625, 42.53950000000001), Vector(38.18086666666666, 56.529), Vector(39.7915625, 70.42333333333333), Vector(38.22844999999999, 84.22250000000001), Vector(33.49152916666666, 97.9265), Vector(25.580799999999996, 111.53533333333336), Vector(14.496262500000002, 125.049), Vector(0.23791666666666667, 138.4675)};
for(int i=0; i<10; i++){
     result.append(shifts[i] + start);
}
tracePath(result, 90);

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