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
  while(true){
    E = getRightVertEnc(); //Right Measurement
    L = getLeftVertEnc(); //Left measurement
    H = getHorEnc(); //Horizontal measurement
    //std::cout << E << ", " << L << ", " << H << std::endl;

    D = (E-L-Dl)/2; //Change in difference on one side - The only thing that signals if there has been rotation
    Dl = E-L; //Last difference = current difference
    
    deltaF = E-El-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
    deltaH = H - Hl; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE
    El = E; //Last forward encoder = current
    Hl = H; //Last horiztonal encoder = current

    testVarMutex.lock();
    Heading += (D/measureWheelDegsOverInches)/centerToMeasureWheelRadius; //Change of rotation in radians              //Possible change: Could update with delta heading since last checked to allow for other sensores to set the heading; or could use (E-L)/2 to get the absolute rotation
    X += deltaF*cos(Heading) + deltaH*cos(Heading-(PI/2)); //Update the current X
    Y += deltaF*sin(Heading) + deltaH*sin(Heading-(PI/2)); //Update the current Y
    testVarMutex.unlock();
    wait(10, msec);
  }
  return 0;
}

void turnTo(double x, bool inDeg = true, bool useSenor = true){
  if(inDeg){
    x = x * (PI/180);
  }
  tHead = x;
  if(useSenor){
    rotateSensor(tHead-Heading);
  }else{
    rotatePID(tHead-Heading);
  }
}

void move(double deltaX, double deltaY, double maxSpeed, bool rotate=true){
  Xt += deltaX;
  Yt += deltaY;
  deltaX = Xt-X;
  deltaY = Yt-Y;
  
  std::cout << "Start: " << deltaX << ", " << deltaY <<std::endl;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY))*measureWheelDegsOverInches;
  //TODO Test Ht and see if it spins right
  double Ht = atan2(deltaY, deltaX); //Returns in radians; PID requires radians; see if deltaX and deltaY need to be special

  if(rotate){
    turnTo(Ht, false);
    axisPID(0, travelDistance, maxSpeed);
  }else{
    vectorPID(Ht, travelDistance, maxSpeed);
  }
}

void move(double deltaX, double deltaY, bool rotate=true){
  move(deltaX, deltaY, 0, rotate);
}