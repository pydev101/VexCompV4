#ifndef __ROBOTODOM_H__
#define __ROBOTODOM_H__
#include "robotmath.h"

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
      accel = newVel + vel.scale(-1);
      vel = newVel;

      currentHeading = currentHeading + deltaHeading;
      double newAngularVelocity = deltaHeading / deltaT;
      angularAcceleration = newAngularVelocity - angularVelocity;
      angularVelocity = newAngularVelocity;

      targetVec = Vector(pos, targetPos);
    }

    void setPos(double x, double y){
      pos = Point(x, y);
      targetVec = Vector(pos, targetPos);
    }


    void setTarget(Vector v){
      targetPos = v + targetPos; //TODO Ensure the math/operator works here
      targetVec = Vector(pos, targetPos);
    }
    void setAbsTarget(double x, double y){
      targetPos = Point(x, y);
      targetVec = Vector(pos, targetPos);
    }
    void setTargetHead(double ang, bool headingInDegrees=false){
      if(headingInDegrees){
        ang = degToRad(ang);
      }
      targetHeading = normalizeAngle(targetHeading);
      targetHeading = ang;
    }
    void updateTargetHead(){
      setTargetHead(Vector(1, 0).getAngle(targetVec));
    }

    double getLinearError(bool shortestArcToLineV=true){
      updateTargetHead();
      double a = getThetaError(shortestArcToLineV);
      if(cos(a) >= 0){
        return targetVec.getMagnitude();
      }else{
        return -targetVec.getMagnitude();
      }
    }
    double getThetaError(bool shortestArcToLineV=false){
      if(shortestArcToLineV){
        return shortestArcToLine(currentHeading, targetHeading);
      }
      return shortestArcToTarget(currentHeading, targetHeading);
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