#ifndef __ROBOTMATH_H__
#define __ROBOTMATH_H__
#include "math.h"

const double PI = 3.14159265;
class Vector;
class Point;

//Basic

int abs(int x){
  if (x < 0){
    return -x;
  }
  return x;
}

double abs(double x){
  if (x < 0){
    return -x;
  }
  return x;
}

int sign(int x){
  if(x < 1){
    return -1;
  }
  return 1;
}

double sign(double x){
  if(x < 1){
    return -1;
  }
  return 1;
}

int floorInt(double x){
  return (int)floor(x);
}

//Algorithms
typedef struct{
  double p; //% change in output over % change in error
  double i;
  double d; //Recommendation Have D term be change in position not change in error so that a change in error is not considered a change in position; Or could just reset PID algoritm each use
} PIDGains;

typedef struct{
  double output;
  double reset;
  double lastError;
} PIDOutput;

//Initalize
PIDOutput initPID(double error, PIDGains gains){
  return {0, 0, error};
}

//Recursive
PIDOutput PID(double error, PIDGains gains, PIDOutput previous){
  double p = gains.p*error;
  double i = previous.reset + gains.i*error;
  double d = gains.d*(error - previous.lastError);
  return {p+i+d, i, error};
}

//Angle

double degToRad(double degrees){
  return degrees*PI/180;
}

double radToDeg(double radians){
  return radians*180/PI;
}

//Returns angle theta normalized within 0 <= theta < 2pi
double normalizeAngle(double theta, bool inRadians=true){
  double toss;
  if(inRadians){
    double r = (2*PI)*modf(theta/(2*PI), &toss);
    if(r<0){r+=(2*PI);}
    return r;
  }else{
    double r = (360)*modf(theta/(360), &toss);
    if(r<0){r+=(360);}
    return r;
  }
}


//Get shortest delta theta to heading
//TODO Ensure this function actually works
double shortestArcToTarget(double currentHeading, double targetHeading, bool inDeg=false){
  if(inDeg){
    currentHeading = degToRad(currentHeading);
    targetHeading = degToRad(targetHeading);
  }
  currentHeading = normalizeAngle(currentHeading);
  targetHeading = normalizeAngle(targetHeading);
  double deltaTheta = targetHeading - currentHeading;
  if(abs(deltaTheta) > PI){
    if(deltaTheta < 0){
      //If E is CW then faster path is CCW
      deltaTheta = (2*PI)+deltaTheta;
    }else{
      //If E is CCW; faster path is CW
      deltaTheta = -((2*PI)-deltaTheta);
    }
  }
  return deltaTheta;
}


//Get shortest delta theta to heading line (secent line between target heading and opposite end of circle)
//TODO Ensure function actually works
double shortestArcToLine(double currentHeading, double targetHeading, bool inDeg=false){
  if(inDeg){
    currentHeading = degToRad(currentHeading);
    targetHeading = degToRad(targetHeading);
  }
  currentHeading = normalizeAngle(currentHeading);
  targetHeading = normalizeAngle(targetHeading);
  double deltaTheta = targetHeading - currentHeading;
  if(abs(deltaTheta) <= PI/2){
    return shortestArcToTarget(currentHeading, targetHeading);
  }else{
    return shortestArcToTarget(currentHeading, targetHeading+PI);
  }
}

//Vector Math
class Point{
  public:
    double x;
    double y;
    Point(double X, double Y){
      x = X;
      y = Y;
    }

    //TODO Test collision
    bool collision(double otherX, double otherY, double r){
      return (((x-otherX)*(x-otherX))+((y-otherY)*(y-otherY))) <= (r*r);
    }
    bool collision(Point other, double r){
      return (((x-other.x)*(x-other.x))+((y-other.y)*(y-other.y))) <= (r*r);
    }
};


class Vector{
  private:
    double deltaX;
    double deltaY;
    
  public:
    //Define vector
    Vector(double delta_x, double delta_y){
      deltaX = delta_x;
      deltaY = delta_y;
    }
    Vector(Point start, Point end){
      deltaX = end.x - start.x;
      deltaY = end.y - start.y;
    }
    Vector(double magnitude, double theta, bool inDegrees){
      if(inDegrees){
        theta = degToRad(theta);
      }
      deltaX = magnitude * cos(theta);
      deltaY = magnitude * sin(theta);
    }

    //Get vector values
    double getX(){
      return deltaX;
    }
    double getY(){
      return deltaY;
    }
    double getMagnitude(){
      return sqrt(deltaX*deltaX + deltaY*deltaY);
    }
    double dot(Vector otherV){
      return deltaX*otherV.deltaX + deltaY*otherV.deltaY;
    }
    //TODO Test (Maybe worked in JS?)
    double getAngle(Vector vecB) {
      if((getMagnitude() == 0) || (vecB.getMagnitude() == 0)){
        return 2*PI; // Its bound between 0-PI so 2PI is error indication
      }
      double theta = acos(dot(vecB) / (getMagnitude() * vecB.getMagnitude()));
      double dotZ = deltaX * -vecB.deltaY + deltaY * vecB.deltaX;
      if (dotZ > 0) {
        //B right of A
        return -theta;
      } else if (dotZ < 0) {
        //B left of A
        return theta;
      } else {
        //Paraell
        return theta;
      }
    }
    Vector scale(double s){
      return Vector(deltaX*s, deltaY*s);
    }
    Vector getUnitVector(){
      if(getMagnitude() == 0){
        return Vector(0, 0);
      }
      return Vector(deltaX/getMagnitude(), deltaY/getMagnitude());
    }
    //TODO Test Math (Works in JS)
    Vector getRotatedVector(double theta, bool inDegrees=false){
      if(inDegrees){
        theta = degToRad(theta);
      }
      return Vector(deltaX * cos(theta) - deltaY * sin(theta), deltaX * sin(theta) + deltaY * cos(theta));
    }

    Vector project(Vector vec){
      //Projects this on to vec
      return vec.scale(dot(vec) / (vec.getMagnitude() * vec.getMagnitude()));
    }
    
    //Magic operators
    Vector operator + (const Vector& other) {
      return Vector(deltaX + other.deltaX, deltaY + other.deltaY);
    }

    Vector operator - (const Vector& other) {
      return Vector(deltaX - other.deltaX, deltaY - other.deltaY);
    }

    Vector operator * (int scalar) {
      return Vector(deltaX*scalar, deltaY*scalar);
    }
    Vector operator * (double scalar) {
      return Vector(deltaX*scalar, deltaY*scalar);
    }
    Point operator + (Point &p){
      return Point(p.x + getX(), p.y + getY());
    }
    Point operator - (Point &p){
      return Point(p.x - getX(), p.y - getY());
    }
};

#endif

