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
    PIDOutput rotationalOutput;
    PIDOutput linearOutput;
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
      void setTargetHead(){
        Vector t = Vector(pos, targetPos);
        setTargetHead(t.getTheta(), false);
      }
      void setTargetHead(double theta, bool inDeg){
        if(inDeg){
          theta = degToRad(theta);
        }
        targetHeading = theta;
        rotationalOutput = initPID(thetaError(), rotationalGains);
      }
      void setTarget(Point t){
        targetPos = t;
        setTargetHead();
        linearOutput = initPID(linearError(), linearGains);
      }
      void setTarget(double deltaX, double deltaY){
        //TODO
        //Make call to setTarget(Point)
      }
      void setTarget(double magnitude, double theta, bool inDeg){
        //TODO
        //Make call to setTarget(Point)
      }
      void setTargetAbs(double x, double y, bool inUnits=true){
        if(inUnits){
          x = x*EncodersPerUnit;
          y = y*EncodersPerUnit;
        }
        setTarget(Point(x, y));
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