#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

//Robot for a position give output of a speed to acheive target

typedef struct{
  double lastOutput;
  double reset;
  double lastError;
} RobotPID;

class Robot{
  private:
    PIDGains linearGains;
    PIDGains angularGains;
    double RobotRadius;
    double UnitsPerRevolution;
    double linearThreshold;
    double rotationalThreshold;
    RobotPID linearPid;
    RobotPID rotationalPid;

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
    void resetRotationalPID(bool shortestArcToLineV){
      rotationalPid = {0,0,location.getThetaError(shortestArcToLineV)};
    }
    void resetLinearPID(bool shortestArcToLineV){
      resetRotationalPID(shortestArcToLineV);
      linearPid = {0,0,location.getLinearError()};
    }

    void setHeadTarget(double theta, bool inDeg){
      location.setTargetHead(theta, inDeg);
      resetRotationalPID(false);
    }

    void setHeadTargetAbs(double head, bool inDeg){
      location.setTargetHeadAbs(head, inDeg);
      resetRotationalPID(false);
    }

    void setTarget(Vector v, bool shortestArcToLineV=true){
      location.setTarget(v);
      location.updateTargetHead();
      resetLinearPID(shortestArcToLineV);
    }

    //Sets realitive to current position and robot orientation
    //dX is horizontal, dY is fwd
    void setTargetRealitiveToRobotOrientation(Vector v, bool shortestArcToLineV=true){
      location.setTargetRealitiveToTargetOrientation(v);
      location.updateTargetHead();
      resetLinearPID(shortestArcToLineV);
    }



    //Returns Rad/Sec
    double getRotationalSpeed(bool shortestArcToLineV=false){
      //Exit Condition; TODO IF angularspeed is signifigant don't just stop
      if(abs(location.getThetaError()) < rotationalThreshold){
        return 0;
      }
      double e = location.getThetaError(shortestArcToLineV);

      double p = e*angularGains.p;

      double r = rotationalPid.reset + e;
      double i = angularGains.i * r;

      double d = angularGains.d * (e - rotationalPid.lastError); //TODO maybe include velocity in some regard so that -180-180 split doesnt impact it in some cases

      double o = p + i + d;

      rotationalPid = {o, r, e};
      return o;
    }

    //Returns UnitsPerSec
    double getLinearSpeed(){
      //Exit Condition; TODO IF speed is signifigant don't just stop
      if(abs(location.getLinearError()) < linearThreshold){
        return 0;
      }
      double e = location.getLinearError();

      double p = e*linearGains.p;

      double r = linearPid.reset + e;
      double i = linearGains.i * r;

      double d = linearGains.d * (e - linearPid.lastError); //TODO maybe include velocity

      double o = p + i + d;

      linearPid = {o, r, e};
      return o;
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
        return sign(e)*speed;
      }
    }

  //PID, tracking, and speed control (Max accel, Max speed, Constant Velocity, self-adj of PID in order to get ideal settings)
};

#endif