/*----------------------------------------------------------------------------
//                                                                            
//    Module:       Odom.h                                               
//    Author:       Carson E                                   
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that creates a virtual grid in the robot and contains top level motor control functions                                     
//                                                                            
//--------------------------------------------------------------------------*/

#include "AdvMotor.h"

double X = 0;
double Y = 0;
double Xt = 0;
double Yt = 0;
double Heading = PI/2;
double tHead = PI/2;

//Continous tracking
mutex testVarMutex;
mutex headlingLock;
int trackingTask(){
  resetEncoders();
  double E = getRightVertEnc(); //Right Measurement
  double L = getLeftVertEnc(); //Left measurement
  double H = getHorEnc(); //Horizontal measurement
  double D = 0; //Current difference
  double El = 0; //Last encoder measurement
  double Hl = 0; //Last horizontal measurement
  double Dl = 0; //Last differnce
  double deltaF = 0; //Change in forward direction
  double deltaH = 0; //Change in horizontal direction
  int toss = 0;
  while(true){
    E = getRightVertEnc(); //Right Measurement
    L = getLeftVertEnc(); //Left measurement
    H = getHorEnc(); //Horizontal measurement

    D = (E-L-Dl)/2; //Change in difference on one side - The only thing that signals if there has been rotation
    Dl = E-L; //Last difference = current difference
    
    deltaF = E-El-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
    deltaH = H - Hl; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE
    El = E; //Last forward encoder = current
    Hl = H; //Last horiztonal encoder = current

    testVarMutex.lock();
    Heading -=  getHeading(); //Change of rotation in radians              //Possible change: Could update with delta heading since last checked to allow for other sensores to set the heading; or could use (E-L)/2 to get the absolute rotation
    X += deltaF*cos(Heading) + deltaH*cos(Heading-(PI/2)); //Update the current X
    Y += deltaF*sin(Heading) + deltaH*sin(Heading-(PI/2)); //Update the current Y
    resetHeading();
    testVarMutex.unlock();
    wait(10, msec);
  }
  return 0;
}

void rotateHeading(double tTheta, double threshold){
  //Determine direction using a closed heading (0-359.99); rotate to target using full rotation (-inf, inf)
  int spinCCW = 1;
  if(tTheta<0){
    //Theta is CW
    spinCCW = -1;
  }

  
}

void turnTo(double x, bool inDeg = true, bool useSenor = true){
  if(inDeg){
    x = x * (PI/180);
  }
  tHead = x;
  testVarMutex.lock();
  double d = tHead-Heading;
  testVarMutex.unlock();

  if(useSenor){
    rotateHeading(d, 0.1);
  }else{

    rotatePID(d);
  }
}

void translatePID(double dX, double dY){

}

void move(double deltaFWD, double deltaRIGHT, double maxSpeed, bool rotate=true){
  deltaFWD = deltaFWD/measureWheelDegsOverInches;
  deltaRIGHT = deltaRIGHT/deltaRIGHT;

  testVarMutex.lock();
  Xt += deltaFWD*cos(Heading) + deltaRIGHT*cos(Heading-(PI/2));
  Yt += deltaFWD*sin(Heading) + deltaRIGHT*sin(Heading-(PI/2));
  double deltaX = Xt-X;
  double deltaY = Yt-Y;
  testVarMutex.unlock();
  
  std::cout << "Start: " << deltaX << ", " << deltaY <<std::endl;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY));
  double Ht = atan2(deltaY, deltaX);

  translatePID(deltaX, deltaY);
}

void move(double deltaX, double deltaY, bool rotate=true){
  move(deltaX, deltaY, 0, rotate);
}