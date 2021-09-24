#include "robotmath.h"

//Defines in terms of units
typedef struct{
  double measurementWheelDiameter;
  int measurmentWheelEncodersPerRotation;
  double width;
  double length;
} RobotProfile;

class Robot{
  private:
    double widthInEncoders;
    double lengthInEncoders;
    double EncodersPerUnit;
    double EncodersPerRotation;
    PIDGains linearGains;
    PIDGains rotationalGains;
    RobotProfile profile;

    Point pos = Point(0, 0);
    double head = 0;

    Point targetPos = Point(0, 0);
    double targetHeading = 0;


  public:
    //Constructors
    Robot (Point Pos, bool posGivenInUnits, double currentHeading, bool headingGivenInDegrees, PIDGains linearPIDGains, PIDGains rotationalPIDGains, RobotProfile robotProfile){
      if(headingGivenInDegrees){
        currentHeading = degToRad(currentHeading);
      }
      if(posGivenInUnits){
        Pos = Point(Pos.x*EncodersPerUnit, Pos.y*EncodersPerUnit);
      }
      head = currentHeading;
      pos = Pos;
      targetPos = Pos;

      profile = robotProfile;
      EncodersPerRotation = profile.measurmentWheelEncodersPerRotation;
      EncodersPerUnit = profile.measurmentWheelEncodersPerRotation / (PI * profile.measurementWheelDiameter);

      linearGains = linearPIDGains;
      rotationalGains = rotationalPIDGains;
    }

    //Setter/Getters
    //TODO Define motion in terms of units not encoders
    void setTargetHead(double theta, bool inDeg){
      if(inDeg){
        theta = degToRad(theta);
      }
      targetHeading = theta;
    }
    void setTargetAbs(double x, double y, bool inUnits=true){
      if(inUnits){
        x = x*EncodersPerUnit;
        y = y*EncodersPerUnit;
      }
      targetPos = Point(x, y);
      targetHeading = Vector(pos, targetPos).getTheta();
    }
    void setTargetAbs(Point p, bool inUnits=true){
      if(inUnits){
        p = Point(p.x*EncodersPerUnit, p.y*EncodersPerUnit);
      }
      targetPos = p;
      targetHeading = Vector(pos, targetPos).getTheta();
    }
    void setTarget(double deltaX, double deltaY, bool inUnits=true){
      Vector t = Vector(deltaX, deltaY);
      if(inUnits){
        t = t*EncodersPerUnit;
      }
      targetPos = targetPos + t;
      targetHeading = t.getTheta();
    }
    void setTarget(double magnitude, double theta, bool inDeg, bool inUnits){
      if(inUnits){
        magnitude = magnitude*EncodersPerUnit;
      }
      Vector t = Vector(magnitude, theta, inDeg);
      targetPos = targetPos + t;
      targetHeading = t.getTheta();
    }
    void setTarget(Vector t){
      t = t*EncodersPerUnit;
      targetPos = targetPos + t;
      targetHeading = t.getTheta();
    }

    //All new positions must be given in terms of encoders and radians
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