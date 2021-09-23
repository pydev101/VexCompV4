#ifndef __ROBOTMATH_H__
#define __ROBOTMATH_H__
#include "math.h"

const double PI = 3.14159265;

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

//Vector
typedef struct{
  double x;
  double y;
} Point;

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
      deltaX = end.y - start.y;
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
    double getTheta(bool inDegrees=false){
      double result = atan2(deltaY, deltaX);
      if(inDegrees){
        return radToDeg(result);
      }
      return result;
    }
    Vector getUnitVector(){
      return Vector(deltaX/getMagnitude(), deltaY/getMagnitude());
    }

    //Magic operators
    Point operator + (const Point& other) {
      Point result;
      result.x = other.x + deltaX;
      result.y = other.y + deltaY;
      return result;
    }
    Vector operator + (const Vector& other) {
      return Vector(other.deltaX + deltaX, other.deltaY + deltaY);
    }

    //TODO Make sure the vector is on the right of the operator
    Point operator - (const Point& other) {
      Point result;
      result.x = other.x - deltaX;
      result.y = other.y - deltaY;
      return result;
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
};
#endif

