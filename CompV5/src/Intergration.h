#include "OdomControls/Grid.h"
#include "OdomControls/MotionControl.h"

Robot robot = Robot(0, 0, PI/2, minSpeed, maxSpeed, 4.25, 12.75); // NEED TO INIT

void updatePosition(){
  double E = getRight();
  double L = getLeft();
  double H = getHor();
  double Head = getHeading();
  double D = (E-L)/2;
  //Dl = E-L; //BUG? Dl might be unesscary; Encoders don't reset

  double deltaF = E-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
  double deltaH = H; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE

  resetEncoders();
  robot.updatePos(deltaF*cos(Head)+deltaH*cos(Head-(PI/2)), deltaF*sin(Head)+deltaH*sin(Head-(PI/2)), Head);
}

void threadTask(){
  while(true){
    updatePosition();
    wait(15, msec);
  }
}

