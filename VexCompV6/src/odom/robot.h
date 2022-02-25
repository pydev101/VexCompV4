#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

/*
Name: robot.h
Written By: Carson Easterling

Implementation of PID, tracking, and motion algorithms; After inital setup using a given position the robot will return a left and right motor output in order to decrease error
*/


class Robot{
  public:
    //Position infomation
    OdomGrid location = OdomGrid(Point(0,0), 90, true);
    double RobotRadius;
    double UnitsPerRevolution; //Units of measure per one full rotation of wheel base

    //PID varibles
    PIDGains linearGains;
    PIDGains rotGains;
    PIDGains linearGainsReverse; 

    Point lastStopPosition = Point(0,0);
    PIDOutput rotPID = initPID(0);

    //Varibles related to if the robot is currently stopped
    double linearThreshold; //How close the robot should be from target before it is allowed to quit
    double rotationalThreshold;
    double roatationStopTimer = 0; //How long has the robot spent within the threashold range
    double motionStopTimer = 0;
    bool stoppedRotating = true; //If true robot is has been in threashold long enough to be considered stop and allowed to continue
    bool stoppedMoving = true;

    //Last target velocity and maximum allowed set outputs
    double targetLinearVelocity = 0;
    double targetAngularVelocity = 0;
    double maxLinearVel = 0;
    double maxAngularVel = 0;

    //Controls to how the PID system should update
    bool usingLinearPIDControls = true; //If PID should update
    bool usingRotPIDControls = true;
    bool forward = true; //Desired robot direction of travel
    bool updateTargetHeadingWhileInMotion = true; //Determines if target heading updates to turn torwards target position while travelling
    bool blockLinearMotionIfThetaErrorTooHigh = true; //Determines if it will stop and turn if the theta error is too high
    double updateTargetHeadingMinThreashold = 0; //Will not update target heading if within X units of target to prevent overshoot, turning back, and repeating
    double maxThetaErrorForMotion = 0; //Determines acceptable theta error for linear motion to occur

    //Relates to robot following a defined path using a list of points
    bool traceModeOn = false;
    double traceVelocity = 0;
    smartPointPointer pathToTrace;
    int pathTraceIndex = 0;

    //Internal function that updates the output of the class using the PID algorithm if allowed
    void updatePID(double deltaT){
      if(usingLinearPIDControls){
        double baseError = getLinearErrorForPID();
        Vector reverseError = Vector(lastStopPosition, location.getPos());
        double newVel = 0;

        if(abs(baseError) > reverseError.getMagnitude()){
          //Follow Reverse
          newVel = reverseError.getMagnitude()*linearGainsReverse.p + linearGainsReverse.i;
          newVel = newVel * sign(baseError);
        }else{
          //Follow Standard
          newVel = linearGains.p*baseError + sign(baseError)*linearGains.i;
        }

        if(abs(newVel) > maxLinearVel){
          newVel = maxLinearVel*sign(newVel);
        }

        targetLinearVelocity = newVel;
      }

      if(usingRotPIDControls){
        double eTheta = getThetaError();
        rotPID = PID(eTheta, deltaT, rotGains, rotPID);
        double newOmega = rotPID.output;

        if(abs(newOmega) > maxAngularVel){
          newOmega = maxAngularVel*sign(newOmega);
        }

        targetAngularVelocity = newOmega;
      }
    }

    //Intenral function that sets target position of class based on current location, speed, and given path
    void updateTrace(double deltaT){
      if(traceModeOn){
        if(abs(location.getRobotBasisVector().dot(Vector(location.getPos(), pathToTrace[pathTraceIndex]))) < linearThreshold){
          pathTraceIndex++;
        }

        Point p = pathToTrace[pathTraceIndex];
        setAbsTarget(p);
        Vector t = Vector(location.getPos(), p).getUnitVector();
        Vector v = location.getRobotBasisVector().scale(traceVelocity);

        if(pathTraceIndex == (pathToTrace.size - 1)){
          setAbsTarget(pathToTrace[pathToTrace.size - 1]);
          setLineMode(true);
        }else{
          setHeadTargetAbs(Vector(1 ,0).getAngle(t), false);
          targetLinearVelocity = v.dot(t);
        }
      }
    }

  //public:
    //Constructor to save all required user defined varibles in safe manner
    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, 
          PIDGains linPID, PIDGains rotPID,
          PIDGains linPIDR,
          double linearThres, double rotationalThresInRadians, double maxLinearVelocity, double maxAngularVelocity,
          double updateTargetHeadingMinThreasholdX, double maxThetaErrorForMotionX, bool lastArgInDeg){
      location = OdomGrid(Pos, CurrentHeading, headingGivenInDegrees);

      lastStopPosition = Pos;

      linearGains = linPID;
      rotGains = rotPID;
      linearGainsReverse = linPIDR;

      rotationalThreshold = rotationalThresInRadians;
      linearThreshold = linearThres;
      maxLinearVel = maxLinearVelocity;
      maxAngularVel = maxAngularVelocity;
      updateTargetHeadingMinThreashold = updateTargetHeadingMinThreasholdX;
      if(lastArgInDeg){
        maxThetaErrorForMotion = degToRad(maxThetaErrorForMotionX);
      }else{
        maxThetaErrorForMotion = maxThetaErrorForMotionX;
      }
    } 


    //Set class paramters in a safe manner using these functions
    void setMaxLinearVel(double x){
      maxLinearVel = abs(x);
    }
    void setMaxRotationalVel(double x){
      maxAngularVel = abs(x);
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


    //Functions that define how PID output should be handled; Useful for differnt functions for example camera has its own PID so this should be disabled to prevent confusion within the system
    void usePIDControls(bool active){
      usingLinearPIDControls = active;
      usingRotPIDControls = active;
    }
    void usePIDControls(bool linActive, bool rotActive){
      usingLinearPIDControls = linActive;
      usingRotPIDControls = rotActive;
    }
    //Sets up PID controls to move in a straight line
    void setLineMode(bool fwd, bool blockLineMotionIfThetaHigh=true){
      traceModeOn = false;
      usePIDControls(true);
      forward = fwd;
      updateTargetHeadingWhileInMotion = true;
      blockLinearMotionIfThetaErrorTooHigh = blockLineMotionIfThetaHigh;
    }
    //Sets up PID controls to rotate in the same spot

    void setRotateMode(){
      traceModeOn = false;
      usePIDControls(true);
      forward = true;
      updateTargetHeadingWhileInMotion = false;
      blockLinearMotionIfThetaErrorTooHigh = true;
    }
    //Sets up PID controls to accept a constantly changing target
    void setIndependentTargetMode(){
      traceModeOn = false;
      usePIDControls(true);
      forward = true;
      updateTargetHeadingWhileInMotion = false;
      blockLinearMotionIfThetaErrorTooHigh = false;
    }
    //Setup robot output to follow a path at contant velocity
    void traceMode(smartPointPointer &path, double vel){
      if(path.size >= 2){
        usePIDControls(false, true);
        forward = true;
        updateTargetHeadingWhileInMotion = false;
        blockLinearMotionIfThetaErrorTooHigh = false;
        traceVelocity = vel;
        setAbsTarget(path[path.size - 1]);
        pathToTrace = path;
        pathTraceIndex = 0;
        traceModeOn = true;
      }
    }

//Target selection------------------------------------------------------------------------------------------------------------
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


  //Position Setting/Updates--------------------------------------------------------------------------------------------------------------

  //Updates robot postion info and should be called first in update loop
  void updatePos(double deltaT, Vector deltaPos, double deltaHead, bool deltaHeadInDegrees=false){
    location.updatePosition(deltaPos, deltaHead, deltaT, deltaHeadInDegrees);
  }
  void setPos(double x, double y){
    location.setPos(x, y);
  }
  void setHead(double head, bool headingGivenInDegrees=false){
    location.setHead(head, headingGivenInDegrees);
  }

  //Check to see if robot has met the criteria to be considered stopped and ready to move to the next positon; Should be called last in update loop
  void updateStopStatus(double deltaT){
    if(abs(getThetaError()) < rotationalThreshold){
      roatationStopTimer = roatationStopTimer + deltaT;
      if(roatationStopTimer > 0.05){
        stoppedRotating = true;
      }
    }else{
      roatationStopTimer = 0;
      stoppedRotating = false;
    }

    if(abs(getLinearErrorForPID()) < linearThreshold){
      motionStopTimer = motionStopTimer + deltaT;
      if(motionStopTimer > 0.05){
        stoppedMoving = true;
        lastStopPosition = location.getPos();
      }
    }else{
      motionStopTimer = 0;
      stoppedMoving = false;
    }
  }

  //Updates class output and should be called second in update loop
  void updateVelocity(double deltaT){
    updateTrace(deltaT);
    updatePID(deltaT);
  }

  //Calculate error values used in PID calculations
  double getThetaError(){
    return shortestArcToTarget(location.getCurrHead(), location.getTargetHead()); //Returns the shortest angle in radians from position to target
  }
  double getLinearErrorForPID(){
    Vector targetVector = location.getTargetVector();
    Vector basis = location.getRobotBasisVector();
    double linErr = basis.dot(targetVector); //Linear error at current heading

    if(abs(linErr) < linearThreshold){
      return 0;
    }

    //Updates target heading if alllowed
    if(targetVector.getMagnitude() > updateTargetHeadingMinThreashold){
      if(updateTargetHeadingWhileInMotion){
        int d = 1;
        if(!forward){
          d = -1;
        }
        location.setTargetHeadAbs(Vector(1, 0).getAngle(targetVector.scale(d)));
      }
    }

    //Block motion if thetaError too high
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

  //Output---------------------------------------------------------------------------------------------------------------------------------------------------------------
  double getLinearSpeedTarget(){
    return targetLinearVelocity;
  }
  double getRotationalSpeedTarget(){
    return targetAngularVelocity;
  }
  bool isLinMoving(){
    return !stoppedMoving;
  }
  bool isRotating(){
    return !stoppedRotating;
  }
  bool isMoving(){
    return !(stoppedMoving && stoppedRotating);
  }

};

#endif