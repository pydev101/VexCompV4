#ifndef __ROBOTCONTROL_H__
#define __ROBOTCONTROL_H__
#include "vex.h"
#include "odometry.h"

//Defines in terms of units
typedef struct{
  double measurementWheelDiameter;
  int measurmentWheelEncodersPerRotation;
  double width;
  double length;
} RobotProfile;

typedef struct{
  double (*getLeftEncoder)();
  double (*getRightEncoder)();
  double (*getHeadingInRadians)();
  void (*resetLeftEncoder)();
  void (*resetRightEncoder)();
  void (*resetHeading)(double head, bool inDegrees);
  void (*setLeftMotor)(double speed);
  void (*setRightMotor)(double speed);
} MotorProfile;

class Robot{
  private:
    int mode = 0; //0 is tracking; 1 is linear motion; 2 is stationary rotation
    const int delayInMilliSec = 15;
    double EncodersPerUnit;
    double EncodersPerRotation;
    RobotProfile profile;
    MotorProfile motors;
    PIDOutput linearOutput;
    PIDOutput rotationalOutput;
    double linearVelocity = 0; //Units per sec
    double angularVelocity = 0; //Radians per sec
    
    void track(){
      static double lastTime = 0;
      static double lastLeftEncoder = 0;
      static double lastRightEncoder = 0;
      static double lastHead = 0; //Radians

      double L = motors.getLeftEncoder();
      double R = motors.getRightEncoder();
      double tempHead = motors.getHeadingInRadians();
      double t = Brain.Timer.value();

      double deltaT = t - lastTime; //TODO Ensure units are actually in msec
      double deltaL = L - lastLeftEncoder;
      double deltaR = R - lastRightEncoder;
      double deltaTheta = tempHead - lastHead;

      double deltaF = 0.5*(deltaL + deltaR)/EncodersPerUnit;
      deltaT = deltaT * 1000; //Convert msec to sec
      linearVelocity = deltaF/deltaT;
      angularVelocity = deltaTheta/deltaT;

      grid.setCurrentHead(tempHead, false);
      grid.shiftPos(deltaF, tempHead, false);

      lastTime = t;
      lastLeftEncoder = L;
      lastRightEncoder = R;
      lastHead = tempHead;
    }

    //TODO
    //Control motor speed based on error; finish with stopped motors and a return of true when complete
    bool straightMotionLoop(){
      //Find linear output, adjust rotational output based on drift
      grid.updateTargetVector();
      rotationalOutput = PID(grid.thetaError(true), rotationalGains, rotationalOutput);
      linearOutput = PID(grid.linearError(), linearGains, linearOutput);
      double left = linearOutput.output - rotationalOutput.output*profile.width*0.5;
      double right = linearOutput.output + rotationalOutput.output*profile.width*0.5;
      left = left*EncodersPerUnit; //TODO Convert to output wheel not based on measurement wheel
      right = right*EncodersPerUnit; 
      if(WITHINTARGET){
        if(WITHINSTOPPED){
          STOP
        }
      }
      //Calculate motor speed with account to theta drift
      //If within parameters set motors to 0 and return true; else set motor speed
    }

    bool rotateMotionLoop(){
      rotationalOutput = PID(grid.thetaError(false), rotationalGains, rotationalOutput);
      //If within parameters set motors to 0 and return true; else set motor speed
    }

  public:
    OdomGrid grid = OdomGrid(Point(0,0), PI/2, false);
    PIDGains linearGains;
    PIDGains rotationalGains;

    Robot(double startX, double startY, double startHeading, bool startHeadingInDegrees, RobotProfile robotProfile, MotorProfile motorProfile, PIDGains linearPIDGains, PIDGains rotationalPIDGains){
      profile = robotProfile;
      motors = motorProfile;
      grid = OdomGrid(Point(startX, startY), startHeading, startHeadingInDegrees);
      
      linearGains = linearPIDGains;
      rotationalGains = rotationalPIDGains;

      EncodersPerRotation = profile.measurmentWheelEncodersPerRotation;
      EncodersPerUnit = profile.measurmentWheelEncodersPerRotation / (PI * profile.measurementWheelDiameter);

      motors.resetLeftEncoder();
      motors.resetRightEncoder();
      motors.resetHeading(startHeading, startHeadingInDegrees);
    }

    //Needs to be on its own thread so it can constantly update position
    bool controlLoop(){
      bool result = false;
      track();
      if(mode == 1){
        result = straightMotionLoop();
      }else if(mode == 2){
        result = rotateMotionLoop();
      }
      if(result){
        mode = 0; //
      }
      wait(delayInMilliSec, msec);
      return result;
    }

    void move(){
      //Goes to grid target position
      linearOutput = initPID(grid.linearError(), linearGains);
      rotationalOutput = initPID(grid.thetaError(true), rotationalGains);
      mode = 1;
      while(mode == 1){wait(delayInMilliSec, msec);}
    }
    void rotate(){
      //Roates to grid target heading
      rotationalOutput = initPID(grid.thetaError(false), linearGains);
      mode = 2;
      while(mode == 2){wait(delayInMilliSec, msec);}
    }

    //Need continous tracking mode
    //Straight line motion mode
    //Stationary rotation mode
    //Public function sets target then runs correct mode until it stops then stops motors and swaps back to tracking mode
};
#endif

/*
int test(){
    return 42;
}

int main(){
    int (*p)() = test;
    std::cout << test() << std::endl;
    std::cout << p() << std::endl;
    return 0;
}*/