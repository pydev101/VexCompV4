#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

//Robot for a position give output of a speed to acheive target
class Robot{
  private:
    PIDGains linearGains;
    PIDGains angularGains;
    double RobotRadius;
    double UnitsPerRevolution;
    double linearThreshold;
    double rotationalThreshold;
    PIDOutput linearPid = {0,0,0};
    PIDOutput rotationalPid = {0,0,0};

  public:
    OdomGrid location = OdomGrid(Point(0,0), 90, true);

    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, PIDGains linearK, PIDGains angularK, double linearThres, double rotationalThresInRadians){
      location = OdomGrid(Pos, CurrentHeading, headingGivenInDegrees);
      linearGains = linearK;
      angularGains = angularK;
      rotationalThreshold = rotationalThresInRadians;
      linearThreshold = linearThres;
    } 

    //Sets Target using XY basis grid
    void setHeadTarget(double theta, bool inDeg){
      location.setTargetHead(theta, inDeg);
    }

    void setHeadTargetAbs(double head, bool inDeg){
      location.setTargetHeadAbs(head, inDeg);
    }

    void setTarget(Vector v){
      location.setTarget(v);
      location.updateTargetHead();
    }

    //Sets realitive to last target position
    void setTargetRealitiveToRobotOrientation(Vector v){
      //dX is horizontal, dY is fwd
      location.setTargetRealitiveToTargetOrientation(v);
      location.updateTargetHead();
    }
    void setAbsTarget(double x, double y){
      location.setAbsTarget(x, y);
      location.updateTargetHead();
    }

    double turnCV(double speed){
      double e = location.getThetaError();
      if(abs(e) < rotationalThreshold){
        return 0;
      }else{
        return sign(e)*speed;
      }
    }

    double moveCV(double speed){
      double e = location.getLinearError();
      if(abs(e) < linearThreshold){
        return 0;
      }else{
        return speed;
      }
    }

    double rotate(double deltaT, bool reset=false){
      //TODO implement max accel feature
      double e = location.getThetaError();
      if(reset){
        rotationalPid = initPID(e);
        return 0;
      }
      rotationalPid = PID(e, deltaT, angularGains, rotationalPid);
      return rotationalPid.output;
    }

    double linear(double deltaT, bool reset=false){
      //TODO implement max accel feature
      double e = location.getLinearError();
      if(reset){
        linearPid = initPID(e);
        return 0;
      }
      linearPid = PID(e, deltaT, linearGains, linearPid);
      return abs(linearPid.output); //Direction of velocity is determines by outer funtion
    }
  //PID, tracking, and speed control (Max accel, Max speed, Constant Velocity, self-adj of PID in order to get ideal settings)
};

#endif