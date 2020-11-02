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
double E = 0; //Right Measurement
double L = 0; //Left measurement
double H = 0; //Horizontal measurement

//Continous tracking
mutex testVarMutex;
int trackingTask(){
  resetEncoders();
  E = getRightVertEnc(); //Right Measurement
  L = getLeftVertEnc(); //Left measurement
  H = getHorEnc(); //Horizontal measurement
  double D = 0; //Current difference
  double El = 0; //Last encoder measurement
  double Hl = 0; //Last horizontal measurement
  double Dl = 0; //Last differnce
  double deltaF = 0; //Change in forward direction
  double deltaH = 0; //Change in horizontal direction
  double toss = 0;
  while(true){
    testVarMutex.lock();
    E = getRightVertEnc(); //Right Measurement
    L = getLeftVertEnc(); //Left measurement
    H = getHorEnc(); //Horizontal measurement

    D = (E-L-Dl)/2; //Change in difference on one side - The only thing that signals if there has been rotation
    Dl = E-L; //Last difference = current difference
    
    deltaF = E-El-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
    deltaH = H - Hl; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE
    El = E; //Last forward encoder = current
    Hl = H; //Last horiztonal encoder = current

    Heading -=  getHeading(); //Change of rotation in radians              //Possible change: Could update with delta heading since last checked to allow for other sensores to set the heading; or could use (E-L)/2 to get the absolute rotation
    X += deltaF*cos(Heading) + deltaH*cos(Heading-(PI/2)); //Update the current X
    Y += deltaF*sin(Heading) + deltaH*sin(Heading-(PI/2)); //Update the current Y
    resetHeading();
    testVarMutex.unlock();
    wait(10, msec);
  }
  return 0;
}

void rotateUsingHeading(double tTheta, double threshold=0.08, double gain=300){
  //Determine direction using a closed heading (0-359.99); rotate to target using full rotation (-inf, inf)
  testVarMutex.lock();
  double localHead = Heading;
  testVarMutex.unlock();
  double startHead = localHead; //Used as 0 point for the full rotatin based heading

  results r = PID(tTheta+startHead, localHead, gain);

  if(abs(r.lastError) < threshold){
    return;
  }

  setDPS(r.speed);
  rotate();

  while(abs(r.lastError) > threshold){
    testVarMutex.lock();
    localHead = Heading;
    testVarMutex.unlock();
    r = PID(tTheta+startHead, localHead, gain, r);
    std::cout << r.lastError << ", " << r.speed << std::endl;
    setDPS(r.speed);
    if(abs(r.speed) < minDPSSpeed){
      break;
    }
    wait(10, msec);
  }
  setDPS(0);
  stopMotors();
}

void turnTo(double x, bool inDeg = true, bool useSenor = true){
  if(inDeg){
    x = x * (PI/180);
  }
  double toss;

  tHead = x;
  testVarMutex.lock();
  double d = tHead-((2*PI)*modf(Heading/(2*PI), &toss));
  testVarMutex.unlock();
  if(d<(-PI)){
    d += (2*PI);
  }else if (d>(PI)) {
    d -= (2*PI);
  }

  if(useSenor){
    rotateUsingHeading(d);
  }else{
    rotatePID(d);
  }
}

void translatePID(double head, double dis, double maxSpeed=0, bool rotate=true, double gain=defaultGainLinear){
  if(rotate){
    turnTo(head, false);
    double toss;
    testVarMutex.lock();
    double wE = E;
    double workHead = Heading;
    testVarMutex.unlock();
    double sE = E;
    dis += sE;

    double adjGain = 2;

    results r = PID(dis, wE, gain);
    if(abs(r.lastError) < minDegreesPerTimeUnit){
      return;
    }
    
    setAxis(0);
    while(abs(r.lastError) > minDegreesPerTimeUnit){
      testVarMutex.lock();
      wE = E;
      workHead = Heading;
      testVarMutex.unlock();

      r = PID(dis, wE, gain, r, maxSpeed);
      std::cout << r.lastError << ", " << r.speed << std::endl;

      setRightDPS((adjGain*(tHead-((2*PI)*modf(Heading/(2*PI), &toss))))+r.speed);
      setLeftDPS(r.speed);

      if(abs(r.speed) < minDPSSpeed){
        break;
      }
      
      wait(timeUnit, msec);
    }
    setDPS(0);
    stopMotors();
  }else{
    testVarMutex.lock();
    double deltaX = Xt-X;
    double deltaY = Yt-Y;
    testVarMutex.unlock();
    dis = sqrt((deltaX*deltaX) + (deltaY*deltaY));
    head = atan2(deltaY, deltaX);
    if(dis < 10){
      return;
    }

    while(dis > 10){
      testVarMutex.lock();
      deltaX = Xt-X;
      deltaY = Yt-Y;
      testVarMutex.unlock();
      dis = sqrt((deltaX*deltaX) + (deltaY*deltaY));
      head = atan2(deltaY, deltaX);

      std::cout << "Head: " << head << "; Dis: " << dis << "; Speed: " << dis*gain << std::endl;

      setVector(head);
      setDPS(dis*gain);
      if(dis*gain < minDPSSpeed){
        break;
      }
    }
    stopMotors();
  }
}

void move(double deltaFWD, double deltaRIGHT, double maxSpeed, bool rotate=true){
  deltaFWD = deltaFWD*measureWheelDegsOverInches;
  deltaRIGHT = deltaRIGHT*measureWheelDegsOverInches;

  testVarMutex.lock();
  Xt += deltaFWD*cos(Heading) + deltaRIGHT*cos(Heading-(PI/2));
  Yt += deltaFWD*sin(Heading) + deltaRIGHT*sin(Heading-(PI/2));
  double deltaX = Xt-X;
  double deltaY = Yt-Y;
  std::cout << "Fwd: " << deltaFWD << "; Right: " << deltaRIGHT << "; Heading: " << (Heading*(180/PI)) <<std::endl;
  testVarMutex.unlock();
  std::cout << "Xt: " << Xt << "; Yt: " << Yt <<std::endl;
  std::cout << "DeltaXY: " << deltaX << ", " << deltaY <<std::endl;

  double travelDistance = sqrt((deltaX*deltaX) + (deltaY*deltaY));
  double Ht = atan2(deltaY, deltaX);

  std::cout << "Travel: " << travelDistance << "; Heading Target: " << Ht <<std::endl;

  translatePID(Ht, travelDistance, maxSpeed, rotate);
}

void move(double deltaFWD, double deltaRIGHT, bool rotate=true){
  move(deltaFWD, deltaRIGHT, 0, rotate);
}