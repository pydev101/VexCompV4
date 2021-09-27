#ifndef __ROBOTCONTROL_H__
#define __ROBOTCONTROL_H__
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
    double EncodersPerUnit;
    double EncodersPerRotation;
    RobotProfile profile;
    MotorProfile motors;
    double leftVelocity = 0; //Encoders per sec
    double rightVelocity = 0; //Encoders per sec
    double headingVelocity = 0; //Radians per sec
    
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
      deltaT = deltaT * 1000; //Convert msec to sec
      double deltaL = L - lastLeftEncoder;
      double deltaR = R - lastRightEncoder;
      double deltaTheta = tempHead - lastHead;

      leftVelocity = deltaL/deltaT;
      rightVelocity = deltaR/deltaT;
      headingVelocity = deltaTheta/deltaT;

      grid.setCurrentHead(tempHead, false);
      grid.shiftPos(0.5*(deltaL + deltaR), tempHead, false);

      lastTime = t;
      lastLeftEncoder = L;
      lastRightEncoder = R;
      lastHead = tempHead;
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

    bool controlLoop(){
      //Return true once a motion motion has finished
      bool result = false;
      if(mode == 0){
        track();
      }else if(mode == 1){
        result = straightMotion();
      }else if(mode == 2){
        result = rotateMotion();
      }
      if(result){
        mode = 0; //
      }
      return result;
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