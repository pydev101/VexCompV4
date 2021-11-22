#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

//Robot for a position give output of a speed to acheive target
class Robot{
  private:
    double linearP;
    double angularP;

  public:
    OdomGrid location = OdomGrid(Point(0,0), 90, true);

    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, double linearKp, double angularKp){
      location = OdomGrid(Pos, CurrentHeading, headingGivenInDegrees);
      linearP = linearKp;
      angularP = angularKp;
    } 

    double getRotationalSpeed(bool shortestArcToLineV=false){
      double o = location.getThetaError(shortestArcToLineV)*angularP;
      return o;
    }

    double getLinearSpeed(){
      double o = location.getLinearError()*linearP;
      return o;
    }

    //Sets Target using XY basis grid
    void resetPID(){

    }

    void setHeadTarget(double theta, bool inDeg){
      resetPID();
      location.setTargetHead(theta, inDeg);
    }

    void setHeadTargetAbs(double head, bool inDeg){
      resetPID();
      location.setTargetHeadAbs(head, inDeg);
    }

    void setTarget(Vector v){
      resetPID();
      location.setTarget(v);
      location.updateTargetHead();
    }

    //Sets realitive to current position and robot orientation
    //dX is horizontal, dY is fwd
    void setTargetRealitiveToRobotOrientation(Vector v){
      resetPID();
      location.setTargetRealitiveToTargetOrientation(v);
      location.updateTargetHead();
    }

  //PID, tracking, and speed control (Max accel, Max speed, Constant Velocity, self-adj of PID in order to get ideal settings)
};

#endif