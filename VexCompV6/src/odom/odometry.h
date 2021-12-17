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

      pos = deltaPos + pos; //TODO Ensure this works
      Vector newVel = deltaPos.scale(1/deltaT);
      accel = (newVel + vel.scale(-1)).scale(1/deltaT);
      vel = newVel;

      currentHeading = currentHeading + deltaHeading; //TODO Examine this didn't seem to be working eariler
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
      targetPos = v + targetPos; //TODO Ensure the math/operator works here
      targetVec = Vector(pos, targetPos);
    }
    void setTargetRealitiveToTargetOrientation(Vector v){
      Vector fwd = Vector(1, targetHeading, false).scale(v.getY());
      Vector hor = Vector(1, targetHeading - (PI/2), false).scale(v.getX());
      setTarget(fwd + hor); //TODO Ensure Operator does its job
    }
    void setAbsTarget(double x, double y){
      targetPos = Point(x, y);
      targetVec = Vector(pos, targetPos);
    }
    void setTargetHeadAbs(double ang, bool headingInDegrees=false){
      if(headingInDegrees){
        ang = degToRad(ang);
      }
      targetHeading = normalizeAngle(targetHeading);
      targetHeading = ang;
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

    //TODO ENSURE THIS IS ACCUATE
    double getLinearError(bool shortestArcToLineV=true){
      targetVec = Vector(pos, targetPos);
      //std::cout << "(" << pos.x << ", " << pos.y << "), Target: (" << targetPos.x << ", " << targetPos.y << "), Vector: <" << targetVec.getX() << ", " << targetVec.getY() << ">" << std::endl;
      updateTargetHead(); //Overides how user last set target heading to turn in order to move robot linearly
      double a = getThetaError(shortestArcToLineV);
      if(cos(a) >= 0){
        return targetVec.getMagnitude();
      }else{
        return -targetVec.getMagnitude();
      }
    }
    //TODO Ensure works hyper important; should be -180 to 180
    double getThetaError(bool shortestArcToLineV=false){
      if(shortestArcToLineV){
        return shortestArcToLine(currentHeading, targetHeading);
      }
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