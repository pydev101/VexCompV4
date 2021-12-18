#ifndef __ROBOTODOM_H__
#define __ROBOTODOM_H__
#include "robotmath.h"
#include <iostream>

//Describes positon, direction, size, and PID characteritics; PID control should be handled seperatly
//Assume everything except angles which is stored in radiqans is stored in the same units given
class OdomGrid{
  private:
    Point pos = Point(0, 0); //Units given by user
    double currentHeading = 0; //Radians

    Vector vel = Vector(0, 0);
    Vector accel = Vector(0, 0);

    double angularVelocity = 0;
    double angularAcceleration = 0;

    Point targetPos = Point(0, 0); //Units given by user
    double targetHeading = 0; //Radians
    Vector targetVec = Vector(0, 0);

  public:
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
    void updateTargetHead(){
      setTargetHeadAbs(Vector(1, 0).getAngle(targetVec));
    }

    //+ if in front; - if in back
    double getLinearError(){
      //Make realitive to the front of the robot
      Vector robotBasis = Vector(1, currentHeading, false);
      Vector realitiveTargetVector = targetVec.project(robotBasis); //dy is fwd/bck and dx is right/lft
      return realitiveTargetVector.getY();
    }

    double getThetaError(){
      return shortestArcToTarget(currentHeading, targetHeading);
    }
    
    Point getPos(){
      return pos;
    }
    double getCurrHead(){
      return currentHeading;
    }
    Vector getVel(){
      return vel;
    }
    Vector getAccel(){
      return accel;
    }
    double getAngularVel(){
      return angularVelocity;
    }
    double getAngularAccel(){
      return angularAcceleration;
    }
};
#endif