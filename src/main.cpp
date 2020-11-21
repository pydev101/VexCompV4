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
// intakeLeft           motor         1               
// intakeRight          motor         2               
// liftT                motor         3               
// liftB                motor         4               
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

bool hasSD;
void pre_auton(void) {
  vexcodeInit();
  hasSD = Brain.SDcard.isInserted();
  preSettings();

  ISensor.startCalibration();
  while(ISensor.isCalibrating()){wait(5, msec);}

  //frontLeft.setBrake(brakeType::hold);
  //frontRight.setBrake(brakeType::hold);
  //backLeft.setBrake(brakeType::hold);
  //backRight.setBrake(brakeType::hold);
  
  setDPS(0);

  resetHeading();
  resetEncoders();
  task updateTask(trackingTask);

  BrainGUIProgram();
}

int AutoRecorder(){
  autoEntry ent = entries[indexAuto];
  if(Brain.SDcard.isInserted()){
    char buffer [6000];
    double workX = 0;
    double workY = 0;
    double workH = 0;
    double workXt = 0;
    double workYt = 0;
    double workHt = 0;

    sprintf(buffer, "X,Y,Heading,Xt,Yt,Headingt\n"); //29 chars
    std::cout << "Setup complete" << std::endl;
    while(programRunning){
      testVarMutex.lock();
      workX = X; 
      workY = Y;
      workH = Heading;
      workXt = Xt;
      workYt = Yt;
      workHt = tHead;
      testVarMutex.unlock();
      sprintf(buffer, "%s%.5g,%.5g,%.5g,%.5g,%.5g,%.5g\n", buffer, workX, workY, workH, workXt, workYt, workHt); //200 per loop
      std::cout << "Looping" << std::endl;
      wait(166, msec);
    }
    char* charToPrint = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy (charToPrint,buffer);

    Brain.SDcard.savefile("lastestAuto.csv", reinterpret_cast<unsigned char*>(charToPrint), (sizeof(char)*(strlen(buffer)+1)));
    //Brain.SDcard.appendfile("lastestAuto.csv", reinterpret_cast<unsigned char*>(buffer), (sizeof(char)*(strlen(buffer)+1)));
    free(charToPrint);
    std::cout << "SAFE" << std::endl;
  }
  return 0;
};

void autonomous(void) {
  frontLeft.setBrake(brakeType::hold);
  frontRight.setBrake(brakeType::hold);
  backLeft.setBrake(brakeType::hold);
  backRight.setBrake(brakeType::hold);
  programRunning = true;
  task fileTrackingTask (AutoRecorder);
  entries[indexAuto].function(entries[indexAuto].mod);
  programRunning = false;
}

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
  int intakeToggle = 0;

  while (true) {
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

    if(Controller1.ButtonR1.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(100, pct);
    }else if(Controller1.ButtonR2.pressing()){
      liftT.spin(fwd);
      liftT.setVelocity(-100, pct);
    }else{
      liftT.setVelocity(0, pct);
      liftT.stop();
    }

    if(Controller1.ButtonL1.pressing()){
      liftB.spin(fwd);
      liftB.setVelocity(100, pct);
    }else if(Controller1.ButtonL2.pressing()){
      liftB.spin(fwd);
      liftB.setVelocity(-100, pct);
    }else{
      liftB.setVelocity(0, pct);
      liftB.stop();
    }

    if(Controller1.ButtonUp.pressing()){
      if(intakeToggle == 1){
        intake(0);
        intakeToggle = 0;
      }else{
        intake(1);
        intakeToggle = 1;
      }
      wait(250, msec);
    }else if(Controller1.ButtonDown.pressing()){
      if(intakeToggle == -1){
        intake(0);
        intakeToggle = 0;
      }else{
        intakeToggle = -1;
        intake(-1);
      }
      wait(250, msec);
    }

    wait(20, msec);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  double workX = 0;
  double workY = 0;
  double workH = 0;

  double horScale = 1/10.7;
  double vertScale = horScale*0.5;;
  
  double tempX = 130+(workX*horScale);
  double tempY = 220-(workY*vertScale);
  Brain.Screen.clearScreen(black);
  Brain.Screen.setPenColor(yellow);
  Brain.Screen.setFillColor(green);

  while (true) {
    testVarMutex.lock();
    workX = X;
    workY = Y;
    workH = Heading;
    testVarMutex.unlock();

    tempX = 130+(workX*horScale);
    tempY = 220-(workY*vertScale);

    if(hasSD){
      Brain.Screen.drawImageFromFile("gameMap.png", 0, 0);
    }else{
      Brain.Screen.clearScreen(black);
    }

    Brain.Screen.setPenWidth(5);
    Brain.Screen.drawLine(tempX, tempY, tempX+(20*cos(workH)), tempY-(20*sin(workH)));

    Brain.Screen.setPenWidth(0);
    Brain.Screen.drawCircle(tempX, tempY, 10);

    Brain.Screen.render();
    wait(1000, msec);
  }
}