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

void transfer(){
  Point p;
  while(robot.driving()){
    //p = robot.getPos();
    setDPS(robot.move());
    wait(20, msec);
  }
  setDPS(0,0);
  wait(1000, msec);
  //std::cout << "(" << p.x << ", " << p.y << ", " << p.head << ", " << 0 << ")" << std::endl;
}


void threadTask(){
  while(true){
    updatePosition();
    wait(15, msec);
  }
}

