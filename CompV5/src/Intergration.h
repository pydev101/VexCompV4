#include "OdomControls/Grid.h"
#include "OdomControls/MotionControl.h"
#include "Learning.h"

/*
typedef struct{
  double primaryGain;
  double adjGain;
  double stopThreshold;
  double slowThreshold;
  double maxAccel;
} PIDVarible;
*/

PIDVarible rotForTest = {7.7, 0.1, (1.5*PI)/180, 0.001, 5000};
PIDVarible linForTest = {1.4, 0, 1, 0.001, 5000};
Robot robot = Robot(0, 0, PI/2, minSpeed, maxSpeed, 4.25, 12.5, rotForTest, linForTest);
//Robot robot = Robot(0, 0, PI/2, minSpeed, maxSpeed, 4, 13.75, 7.7, 5000, 1.52, 5000, 1.5, (2.5*PI)/180);

const int DataLength = 0;
double graphR[DataLength];
double graphL[DataLength];
double graphH[DataLength];
double graphX[DataLength];
double graphY[DataLength];

int indexOfGraph = 0;

void updatePosition(){
  double E = getRight();
  double L = getLeft();
  double H = getHor();
  static double Head = getHeading();

  resetEncoders();
  
  //double D = (getHeading()-Head)*robot.getRadiusInEncoders(); //Calculate the change in motor encoder based on change of rotation

  double deltaF = (E+L)/2; //Delta F is the change in the direction the robot is facing; rotation isn't fwd movement so we attempt to remove it via the average
  double deltaH = H; //If H is important then we may need to subtract the circumfrance it travels from the delta H

  if(indexOfGraph<DataLength){
    graphR[indexOfGraph] = E;
    graphL[indexOfGraph] = L;
    graphH[indexOfGraph] = Head;
    graphX[indexOfGraph] = deltaF*cos(Head)+deltaH*cos(Head-(PI/2));
    graphY[indexOfGraph] = deltaF*sin(Head)+deltaH*sin(Head-(PI/2));
    indexOfGraph++;
  }

  Head = getHeading();
  robot.updatePos(deltaF*cos(Head)+deltaH*cos(Head-(PI/2)), deltaF*sin(Head)+deltaH*sin(Head-(PI/2)), Head);
}

bool keepRecordThreadRunning = true;
void threadTask(){
  while(keepRecordThreadRunning){
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

void turnHelp(int i=0){
  double ti = Brain.timer(timeUnits::sec);
  while(robot.turning() && ((Brain.timer(timeUnits::sec)- ti) < 20)){
    setDPS(robot.turnToHead());
    wait(20, msec);
  }
  setDPS(0,0);
  wait(200, msec);
  if(robot.turning()){
    if(i<1){
      turnHelp(i+1);
    }
  }
}

void moveHelp(bool useShortestVector){
  double ti = Brain.timer(timeUnits::sec);
  while(robot.driving() && ((Brain.timer(timeUnits::sec)- ti) < 5)){
    setDPS(robot.move(useShortestVector));
    wait(20, msec);
  }
  setDPS(0,0);
  wait(200, msec);
}

void turnToHead(double head, bool inDeg=true){
  robot.setTHead(head, inDeg);
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

void fullReset(double x, double y, double h){
  resetEncoders();
  getHeading(true, h*(180/PI));
  robot.resetPos(x, y, h);
}