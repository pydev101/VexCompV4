#include "OdomControls/Grid.h"
#include "OdomControls/MotionControl.h"
#include "Learning.h"

Robot robot = Robot(0, 0, PI/2, minSpeed, maxSpeed, 4.25, 12.5, 7, 5000, 1.5, 5000, 1.5, PI/2);

void updatePosition(){
  double E = getRight();
  double L = getLeft();
  double H = getHor();
  static double Head = getHeading();
  resetEncoders();
  
  double D = (getHeading()-Head)*robot.getRadiusInEncoders(); //Calculate the change in motor encoder based on change of rotation

  double deltaF = E-D; //Delta F is the change in the direction the robot is facing; rotation isn't fwd movement so we attempt to subtract it out
  double deltaH = H; //If H is important then we may need to subtract the circumfrance it travels from the delta H

  Head = getHeading();
  robot.updatePos(deltaF*cos(Head)+deltaH*cos(Head-(PI/2)), deltaF*sin(Head)+deltaH*sin(Head-(PI/2)), Head);
}
void threadTask(){
  while(true){
    updatePosition();
    wait(15, msec);
  }
}

/*
TODO:
-Intergrate learning behavior
--Likely will need to write a function that sets the double roationalGainA, double maxRotAccelerationA, double linearGainA, double maxLinAccelerationA only; 
--Leave the other contructors varibles alone because tehy are constants or handled by other parts of the program
--Timing and learning components can be intergated into the high level move functions should be defined here and not in the class itself
*/

void turnHelp(){
  while(robot.turning()){
    robot.turnToHead();
    wait(20, msec);
  }
  setDPS(0,0);
  wait(200, msec);
}

void moveHelp(bool useShortestVector){
  while(robot.driving()){
    setDPS(robot.move(useShortestVector));
    wait(20, msec);
  }
  setDPS(0,0);
  wait(200, msec);
}

void turnToHead(double head, bool inDeg=true){
  if(inDeg){
    head = head*(PI/180);
  }
  robot.setTHead(head);
  turnHelp();
}

void move(double fwd, double hor, bool useShortestVector=true){
  robot.setTRealitive(fwd, hor);
  if(useShortestVector){
    robot.setToShortestVector();
  }
  turnHelp();
  moveHelp(useShortestVector);
}

void moveAbs(double X, double Y, bool useShortestVector=true){
  robot.setTAbsolute(X, Y);
  if(useShortestVector){
    robot.setToShortestVector();
  }
  turnHelp();
  moveHelp(useShortestVector);
}
