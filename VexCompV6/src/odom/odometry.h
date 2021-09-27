#ifndef __ROBOTODOM_H__
#define __ROBOTODOM_H__
#include "robotmath.h"

//Describes positon, direction, size, and PID characteritics; PID control should be handled seperatly
//Assume everything except angles which is stored in radiqans is stored in the same units given
class OdomGrid{
  private:
    Point pos = Point(0, 0); //Units given by user
    double head = 0; //Radians

    Point targetPos = Point(0, 0); //Units given by user
    double targetHeading = 0; //Radians

  public:
    //Constructors
    OdomGrid (Point Pos, double currentHeading, bool headingGivenInDegrees){
      if(headingGivenInDegrees){
        currentHeading = degToRad(currentHeading);
      }
      head = currentHeading;
      pos = Pos;
      targetPos = Pos;
    }

    //Setter/Getters
    //Set target heading and position
      Vector updateTargetVector(){
        Vector t = getTargetVector();
        setTargetHead(t.getTheta(), false);
        return t;
      }
      void setTargetHead(double theta, bool inDeg){
        if(inDeg){
          theta = degToRad(theta);
        }
        targetHeading = theta;
      }


      void setTarget(Point t){
        targetPos = t;
        updateTargetVector();
      }
      void setTarget(double deltaX, double deltaY){
        setTarget(Vector(deltaX, deltaY) + pos);
      }
      void setTarget(double magnitude, double theta, bool inDeg){
        setTarget(Vector(magnitude, theta, inDeg) + pos);
      }
      void setTargetAbs(double x, double y){
        setTarget(Point(x, y));
      }
      Point getTarget(){
        return pos;
      }
      double getTargetHeading(bool inDeg){
        if(inDeg){
          return radToDeg(targetHeading);
        }
        return targetHeading;
      }



      //Set current heading and position
      void setCurrentHead(double currentHeading, bool inDeg){
        if(inDeg){
          currentHeading = degToRad(currentHeading);
        }
        head = currentHeading;
      }
      void shiftCurrentHead(double theta, bool inDeg){
        if(inDeg){
          theta = degToRad(theta);
        }
        head += theta;
      }
      double getHeading(bool inDeg){
        if(inDeg){
          return radToDeg(head);
        }
        return head;
      }


      void setPos(Point t){
          pos = t;
      }
      void shiftPos(double deltaX, double deltaY){
        setPos(Vector(deltaX, deltaY) + pos);
      }
      void shiftPos(double magnitude, double theta, bool inDeg){
        setPos(Vector(magnitude, theta, inDeg) + pos);
      }
      void setPosAbs(double x, double y){
        setPos(Point(x, y));
      }
      Point getPos(){
        return pos;
      }


    //Helper
    Vector getTargetVector(){
      return Vector(pos, targetPos);
    }
    double linearError(){
      Vector v = getTargetVector();
      return v.getMagnitude() * v.sameDirection(head);
    }
    double thetaError(bool alignWithSecent){
      if(alignWithSecent){
        return shortestArcToLine(head, targetHeading);
      }else{
        return shortestArcToTarget(head, targetHeading);
      }
    }
};
#endif