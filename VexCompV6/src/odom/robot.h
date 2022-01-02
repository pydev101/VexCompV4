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
    bool blockLinearMotionIfThetaErrorTooHigh = true;
    double updateTargetHeadingMinThreashold = 0;
    double maxThetaErrorForMotion = 0;

  //public:
    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, PIDGains linearK, PIDGains angularK, double linearThres, double rotationalThresInRadians, double maxLinearVelocity, double maxAngularVelocity, double maxLinearAccelleration, double maxAngularAccelleration, double updateTargetHeadingMinThreasholdX, double maxThetaErrorForMotionX, bool lastArgInDeg){
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
      if(lastArgInDeg){
        maxThetaErrorForMotion = degToRad(maxThetaErrorForMotionX);
      }else{
        maxThetaErrorForMotion = maxThetaErrorForMotionX;
      }
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
    void setMaxThetaErrorForMotion(double x, bool inDeg){
      x = abs(x);
      if(inDeg){
        maxThetaErrorForMotion = degToRad(x);
      }else{
        maxThetaErrorForMotion = x;
      }
    }

    void usePIDControls(bool active){
      usingPIDControls = active;
    }
    void setLinPID(PIDGains x){
      linearGains = x;
      linearPid = initPID(0);
    }
    void setRotPID(PIDGains x){
      angularGains = x;
      rotationalPid = initPID(0);
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
    void setAbsTarget(Point p){
      location.setAbsTarget(p);
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
    double linErr = basis.dot(targetVector);

    if(targetVector.getMagnitude() > updateTargetHeadingMinThreashold){
      if(updateTargetHeadingWhileInMotion){
        int d = 1;
        if(!forward){
          d = -1;
        }
        location.setTargetHeadAbs(Vector(1, 0).getAngle(targetVector.scale(d)));
      }
    }

    if(blockLinearMotionIfThetaErrorTooHigh){
      if(abs(getThetaError()) < maxThetaErrorForMotion){
        return linErr;
      }else{
        return 0;
      }
    }else{
      return linErr;
    }
  }

  void setLineMode(bool fwd){
    usingPIDControls = true;
    forward = fwd;
    updateTargetHeadingWhileInMotion = true;
    blockLinearMotionIfThetaErrorTooHigh = true;
  }
  void setRotateMode(){
    usingPIDControls = true;
    forward = true;
    updateTargetHeadingWhileInMotion = false;
    blockLinearMotionIfThetaErrorTooHigh = true;
  }
  //Use for tracing, camera and mechanium wheels; Doesnt allow theta target to change during linear motion so targets can be set indepenedently
  void setIndependentTargetMode(){
    usingPIDControls = true;
    forward = true;
    updateTargetHeadingWhileInMotion = false;
    blockLinearMotionIfThetaErrorTooHigh = false;
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