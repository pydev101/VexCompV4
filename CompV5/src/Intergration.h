#include "OdomControls/Grid.h"
#include "OdomControls/MotionControl.h"

Robot robot = Robot(0, 0, 0, minSpeed, 0); // NEED TO INIT

void updatePosition(){
  double E = getRight();
  double L = getLeft();
  double H = getHor();
  double Head = getHeading();
  static double El = 0;
  static double Ll = 0;
  static double Hl = 0;
  static double Dl = 0;
  double D = (E-L-Dl)/2;
  Dl = E-L; //BUG? Dl might be unesscary; Encoders don't reset

  double deltaF = E-El-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
  double deltaH = H - Hl; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE
  El = E;
  Hl = H;

  robot.updatePos(deltaF*cos(Head)+deltaH*cos(Head-(PI/2)), deltaF*sin(Head)+deltaH*sin(Head-(PI/2)), Head);
}


