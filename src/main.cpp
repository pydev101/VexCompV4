// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontLeft            motor         13              
// frontRight           motor         12              
// backLeft             motor         11              
// backRight            motor         14              
// rightEncoder         encoder       C, D            
// leftEncoder          encoder       E, F            
// horEncoder           encoder       A, B            
// ISensor              inertial      20              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                               
//    Author:       Carson E                                 
//    Created:      Thu Sep 10 2020                                           
//    Description:  Primary file that implements controls and movement                                           
//                                                                            
//--------------------------------------------------------------------------*/

#include "AutoPrograms.h"

//competition Competition;

void pre_auton(void) {
  vexcodeInit();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  
  setDPS(0);

  resetHeading();
  resetEncoders();
}


void autonomous(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
}


bool tank = true;
void usercontrol(void) {
  frontLeft.setBrake(brakeType::coast);
  frontRight.setBrake(brakeType::coast);
  backLeft.setBrake(brakeType::coast);
  backRight.setBrake(brakeType::coast);

  double lV = 0;
  double lH = 0;
  double rV = 0;
  double rH = 0;
  int threshold = 5;

  while (true) {
    if(tank){
      lV = Controller1.Axis3.position();
      lH = Controller1.Axis4.position();
      rV = Controller1.Axis2.position();
      rH = Controller1.Axis1.position();

      if((abs(lV)<threshold)&&(abs(lH)<threshold)&&(abs(rV)<threshold)&&(abs(rH)<threshold)){
        stopMotors();
      }else{
        startSpin();
      }

      frontLeft.setVelocity(lV+lH, velocityUnits::pct);
      frontRight.setVelocity(rV-rH, velocityUnits::pct);
      backLeft.setVelocity(lV-lH, velocityUnits::pct);
      backRight.setVelocity(rV+rH, velocityUnits::pct);
      wait(20, msec);
    }
  }
}

mutex testVarMutex;
double testHead = PI/2;
double testX = 0;
double testY = 0;
int testTask(){
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

    D = (E-L-Dl)/2; //Change in difference on one side - The only thing that signals if there has been rotation
    Dl = E-L; //Last difference = current difference
    
    deltaF = E-El-D; //Delta X = change in position - change in position due to rotation divided by 2 (Rotation doesnt change XY because opposite sides cancel out)
    deltaH = H - Hl; //TODO!!!!! If there is a rotation then if the wheel is not in the exact center then it might spin its circumfrance around the center as it rotates!!!! POSSIBLE
    El = E; //Last forward encoder = current
    Hl = H; //Last horiztonal encoder = current

    testVarMutex.lock();
    testHead += D/centerToMeasureWheelRadius; //Change of rotation in radians              //Possible change: Could update with delta heading since last checked to allow for other sensores to set the heading; or could use (E-L)/2 to get the absolute rotation
    testX += deltaF*cos(testHead) + deltaH*cos(testHead-(PI/2)); //Update the current X
    testY += deltaF*sin(testHead) + deltaH*sin(testHead-(PI/2)); //Update the current Y
    testVarMutex.unlock();
    wait(10, msec);
  }
  return 0;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  //move(24, 36*3, false);
  //move(-12, 15);
  //move(-X, -Y, false);

  //rotateSensor(PI/2);

  task updateTask(testTask);

  std::cout << "Heading | X | Y" << std::endl;
  while (true) {
    testVarMutex.lock();
    std::cout << (testHead*(180/PI)) << ", " << testX/measureWheelDegsOverInches << ", " << testY/measureWheelDegsOverInches << std::endl;
    testVarMutex.unlock();
    wait(500, msec);
  }
}