#ifndef __ROBOTODOM_H__
#define __ROBOTODOM_H__
#include "logger.h"

/*
Name: odometry.h
Written By: Carson Easterling

All abstract functions and classes related to tracking the robot's current position on a grid and basic navigation to the specified target point and heading.
Angles are stored in radians with CCW being positive; All units of length depend on the units given by the user or other programs parts (Typically encoders, inches, or cms)
*/


class OdomGrid{
  public:
    Point pos = Point(0, 0); //Current robot position
    double currentHeading = 0; //Current robot heading (direction front of robot is facing)

    Vector vel = Vector(0, 0); //Current robot linear velocity
    Vector accel = Vector(0, 0); //Currrent robot linear acceleration

    double angularVelocity = 0; //Current robot angular velocity
    double angularAcceleration = 0; //Current robot angular acceleration

    Point targetPos = Point(0, 0); //Target navigation point
    double targetHeading = 0; //Target navigation heading
    Vector targetVec = Vector(0, 0); //Vector repersenting the direct path from current position to target position

  //public:
    //Constructors
    OdomGrid(Point Pos, double CurrentHeading, bool headingGivenInDegrees){
      if(headingGivenInDegrees){
        CurrentHeading = degToRad(CurrentHeading);
      }
      pos = Pos;
      currentHeading = CurrentHeading;
      targetPos = Pos;
      targetHeading = currentHeading;
    }

    //Setter/Getters
    //Set target heading and position
    void updatePosition(Vector deltaPos, double deltaHeading, double deltaT, bool headingInDegrees=false){
      if(headingInDegrees){
        deltaHeading = degToRad(deltaHeading);
      }

      pos = deltaPos + pos;
      Vector newVel = deltaPos.scale(1/deltaT);
      accel = (newVel + vel.scale(-1)).scale(1/deltaT);
      vel = newVel;

      currentHeading = currentHeading + deltaHeading;
      double newAngularVelocity = deltaHeading / deltaT;
      angularAcceleration = (newAngularVelocity - angularVelocity) / deltaT;
      angularVelocity = newAngularVelocity;

      targetVec = Vector(pos, targetPos);
    }
    void setHead(double newHead, bool headingInDegrees){
      if(headingInDegrees){
        newHead = degToRad(newHead);
      }
      currentHeading = newHead;
    }
    void setPos(double x, double y){
      pos = Point(x, y);
      targetVec = Vector(pos, targetPos);
    }


    void setTarget(Vector v){
      targetPos = v + targetPos;
      targetVec = Vector(pos, targetPos);
    }
    void setTargetRealitiveToTargetOrientation(Vector v){
      Vector fwd = Vector(1, targetHeading, false).scale(v.getY());
      Vector hor = Vector(1, targetHeading - (PI/2), false).scale(v.getX());
      setTarget(fwd + hor);
    }
    void setAbsTarget(double x, double y){
      targetPos = Point(x, y);
      targetVec = Vector(pos, targetPos);
    }
    void setAbsTarget(Point p){
      targetPos = p;
      targetVec = Vector(pos, targetPos);
    }
    void setTargetHeadAbs(double ang, bool headingInDegrees=false){
      if(headingInDegrees){
        ang = degToRad(ang);
      }
      targetHeading = normalizeAngle(ang);
    }
    void setTargetHead(double theta, bool inDegrees){
      if(inDegrees){
        theta = degToRad(theta);
      }
      setTargetHeadAbs(targetHeading + theta, false);
    }
    
    Point getPos(){
      return pos;
    }
    double getCurrHead(){
      return currentHeading;
    }
    Vector getRobotBasisVector(){
      return Vector(1, currentHeading, false);
    }
    Vector getVel(){
      return vel;
    }
    Vector getRealitiveVel(){
      return vel.project(getRobotBasisVector());
    }
    Vector getAccel(){
      return accel;
    }
    Vector getRealitiveAccel(){
      return accel.project(getRobotBasisVector());
    }
    double getAngularVel(){
      return angularVelocity;
    }
    double getAngularAccel(){
      return angularAcceleration;
    }

    Vector getTargetVector(){
      return targetVec;
    }
    Vector getRealitiveTargetVector(){
      return targetVec.project(getRobotBasisVector());
    }
    double getTargetHead(){
      return targetHeading;
    }

};
#endif