#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

//Robot for a position give output of a speed to acheive target

class Robot{
  public:
    OdomGrid location = OdomGrid(Point(0,0), 90, true);
    double RobotRadius;
    double UnitsPerRevolution;

    PIDGains linearGains;
    PIDGains angularGains;
    PIDOutput linearPid = {0,0,0};
    PIDOutput rotationalPid = {0,0,0};

    double linearThreshold;
    double rotationalThreshold;
    double roatationStopTimer = 0;
    double motionStopTimer = 0;
    bool stoppedRotating = true;
    bool stoppedMoving = true;

    double targetLinearVelocity = 0;
    double targetAngularVelocity = 0;
    double maxLinearAccel = 0;
    double maxAngularAccel = 0;
    double maxLinearVel = 0;
    double maxAngularVel = 0;

    bool usingPIDControls = true;
    bool forward = true;
    bool updateTargetHeadingWhileInMotion = true;
    double updateTargetHeadingMinThreashold = 0;

  //public:
    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, PIDGains linearK, PIDGains angularK, double linearThres, double rotationalThresInRadians, double maxLinearVelocity, double maxAngularVelocity, double maxLinearAccelleration, double maxAngularAccelleration, double updateTargetHeadingMinThreasholdX){
      location = OdomGrid(Pos, CurrentHeading, headingGivenInDegrees);
      linearGains = linearK;
      angularGains = angularK;
      rotationalThreshold = rotationalThresInRadians;
      linearThreshold = linearThres;
      maxLinearAccel = maxLinearAccelleration;
      maxAngularAccel = maxAngularAccelleration;
      maxLinearVel = maxLinearVelocity;
      maxAngularVel = maxAngularVelocity;
      updateTargetHeadingMinThreashold = updateTargetHeadingMinThreasholdX;
    } 

    void setMaxLinearVel(double x){
      maxLinearVel = abs(x);
    }
    void setMaxRotationalVel(double x){
      maxAngularVel = abs(x);
    }
    void setMaxLinearAccel(double x){
      maxLinearAccel = abs(x);
    }
    void setMaxRotationalAccel(double x){
      maxAngularAccel = abs(x);
    }
    void setTargetHeadingMinThreashold(double x){
      updateTargetHeadingMinThreashold = abs(x);
    }

    void usePIDControls(bool active){
      usingPIDControls = active;
    }

//------------------
    //Sets Target using XY basis grid
    void setTargetHead(double theta, bool inDeg){
      location.setTargetHead(theta, inDeg);
    }
    void setHeadTargetAbs(double head, bool inDeg){
      location.setTargetHeadAbs(head, inDeg);
    }
    void setTarget(Vector v){
      location.setTarget(v);
    }
    void setAbsTarget(double x, double y){
      location.setAbsTarget(x, y);
    }
    //Sets realitive to last target position
    void setTargetRealitiveToRobotOrientation(Vector v){
      //dX is horizontal, dY is fwd
      location.setTargetRealitiveToTargetOrientation(v);
    }
//---------------

  void updatePos(double deltaT, Vector deltaPos, double deltaHead, bool deltaHeadInDegrees=false){
    location.updatePosition(deltaPos, deltaHead, deltaT, deltaHeadInDegrees);
  }
  void setPos(double x, double y){
    location.setPos(x, y);
  }
  void setHead(double head, bool headingGivenInDegrees=false){
    location.setHead(head, headingGivenInDegrees);
  }
  void updateStopStatus(double deltaT){
    if(abs(getThetaError()) < rotationalThreshold){
      roatationStopTimer = roatationStopTimer + deltaT;
      if(roatationStopTimer > 0.5){
        stoppedRotating = true;
      }
    }else{
      roatationStopTimer = 0;
      stoppedRotating = false;
    }

    if(abs(getLinearError()) < linearThreshold){
      motionStopTimer = motionStopTimer + deltaT;
      if(motionStopTimer > 0.5){
        stoppedMoving = true;
      }
    }else{
      motionStopTimer = 0;
      stoppedMoving = false;
    }
  }
  void updatePID(double deltaT){
    if(usingPIDControls){
      double e = getLinearError();
      linearPid = PID(e, deltaT, linearGains, linearPid);
      double newVel = linearPid.output;
      if(abs(newVel) > maxLinearVel){
        newVel = maxLinearVel*sign(newVel);
      }
      targetLinearVelocity = newVel;

      double eTheta = getThetaError();
      rotationalPid = PID(eTheta, deltaT, angularGains, rotationalPid);
      double newOmega = rotationalPid.output;
      if(abs(newOmega) > maxAngularVel){
        newOmega = maxAngularVel*sign(newOmega);
      }
      targetAngularVelocity = newOmega;
    }
  }

  double getThetaError(){
    return shortestArcToTarget(location.getCurrHead(), location.getTargetHead());
  }
  double getLinearError(){
    Vector targetVector = location.getTargetVector();
    Vector basis = location.getRobotBasisVector();
    Vector temp = targetVector.project(basis);
    double theta = basis.getAngle(Vector(0, 1));
    temp = temp.getRotatedVector(theta);
    double linErr = temp.getY();

    if(targetVector.getMagnitude() > updateTargetHeadingMinThreashold){
      if(updateTargetHeadingWhileInMotion){
        int d = 1;
        if(!forward){
          d = -1;
        }
        location.setTargetHeadAbs(Vector(1, 0).getAngle(targetVector.scale(d)));
      }
    }

    return linErr;
  }

  void setMoveMode(bool fwd){
    usingPIDControls = true;
    forward = fwd;
    updateTargetHeadingWhileInMotion = true;
  }
  void setRotateMode(){
    usingPIDControls = true;
    forward = true;
    updateTargetHeadingWhileInMotion = false;
  }

  double getLinearSpeedTarget(){
    usingPIDControls = true;
    return targetLinearVelocity;
  }
  double getRotationalSpeedTarget(){
    usingPIDControls = true;
    return targetAngularVelocity;
  }
  bool isRotating(){
    return !stoppedRotating;
  }
  bool isMoving(){
    return !(stoppedMoving && stoppedRotating);
  }

};

#endif