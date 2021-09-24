#include "robotmath.h"

class Robot{
  private:
    double widthInUnit;
    double lengthInUnit;
    double measurementWheelDiameterInUnit;
    double motionWheelDiameterInUnit;

    Point pos;
    double head;

    Point targetPos;
    double targetHeading;

  public:
    //TODO Make constructor to implement all relevent real world values and PID
    //Constructors

    //Setter/Getters
    void setTargetHead(double theta, bool inDeg){
      if(inDeg){
        theta = degToRad(theta);
      }
      targetHeading = theta;
    }
    void setTargetAbs(double x, double y){
      targetPos = Point(x, y);
    }
    void setTargetAbs(Point p){
      targetPos = p;
    }
    void setTarget(double deltaX, double deltaY){
      Vector t = Vector(deltaX, deltaY);
      targetPos = targetPos + t;
    }
    void setTarget(double magnitude, double theta, bool inDeg){
      Vector t = Vector(magnitude, theta, inDeg);
      targetPos = targetPos + t;
    }
    void setTarget(Vector t){
      targetPos = targetPos + t;
    }

    void setPos(double x, double y, double newHead){
      pos = Point(x, y);
      head = newHead;
    }
    void setPos(Point p, double newHead){
      pos = p;
      head = newHead;
    }
    void updatePos(double deltaX, double deltaY, double newHead){
      Vector t = Vector(deltaX, deltaY);
      pos = targetPos + t;
      head = newHead;
    }
    void updatePos(double magnitude, double theta, bool inDeg, double newHead){
      Vector t = Vector(magnitude, theta, inDeg);
      pos = targetPos + t;
      head = newHead;
    }
    void updatePos(Vector t, double newHead){
      pos = targetPos + t;
      head = newHead;
    }

    //Helper
    Vector getTargetVector(){
      return Vector(pos, targetPos);
    }
    double linearError(){
      Vector v = getTargetVector();
      return v.getMagnitude() * v.sameDirection(head);
    }
    double thetaError(bool alignWithSecent=true){
      if(alignWithSecent){
        return shortestArcToLine(head, targetHeading);
      }else{
        return shortestArcToTarget(head, targetHeading);
      }
    }

    //Motion
    //Give linear speed and radial speed

};
































/*
int test(){
    return 42;
}

int main(){
    int (*p)() = test;
    std::cout << test() << std::endl;
    std::cout << p() << std::endl;
    return 0;
}*/