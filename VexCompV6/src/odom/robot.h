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
    Vector getTargetVector(){
      return Vector(pos, targetPos);
    }
    double linearError(){
      Vector v = getTargetVector();
      return v.getMagnitude() * v.sameDirection(head);
    }

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