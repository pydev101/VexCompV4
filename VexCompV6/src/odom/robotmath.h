#ifndef __ROBOTMATH_H__
#define __ROBOTMATH_H__
#include "math.h"
#include <stdlib.h>

/*
Name: robotmath.h
Written By: Carson Easterling

Defines abstract mathmatical concepts and algorithms relevent to robot control. Includes functionality for angles, PID algorithm, beizer curves, and vectors

*/

//Define constants and types
const double PI = 3.14159265;
class Vector;
class Point;

//Basic

//abs - returns absolute value of input
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

//sign - returns 1 if input >= 0; -1 if input < 0
int sign(int x){
  if(x < 0){
    return -1;
  }
  return 1;
}

double sign(double x){
  if(x < 0){
    return -1;
  }
  return 1;
}

//Takes a decimal input and returns the lowest closest interger
int floorInt(double x){
  return (int)floor(x);
}

//--------------------------------------------------------------------------------------------------

//PID Algorithm

//PID Gains - stores PID gain values
typedef struct{
  double p; //% change in output over % change in error
  double i; 
  double d; //Recommendation Have D term be change in position not change in error so that a change in error is not considered a change in position; Or could just reset PID algoritm each use
} PIDGains;

//PID Output - Stores relevent infomation for the next PID cycle
typedef struct{
  double output;
  double reset;
  double lastError;
} PIDOutput;

//Initlizes a PID Output assuming a stationary robot with a new error value
PIDOutput initPID(double error){
  return {0, 0, error};
}

//PID Algorithm
/*
Returns an output (ex speed) that tries to minimize the error value (ex distance from target); Uses 3 tunable constants to get the output to decrease the error

Error: SetValue - CurrentValue; Larger error is farther than where it is trying to be
deltaT: Length of time since the last cycle
gains: The constants used to determine the output
previous: The results from the previous cycle
*/
PIDOutput PID(double error, double deltaT, PIDGains gains, PIDOutput previous){
  double p = gains.p*error; //Proportinal - Larger error creates a larger output (Robot starts off fast and slows down as error decreases and it gets closer to target)
  double i = previous.reset + gains.i*error*deltaT; //Intergral - The longer error persits the output will increases and causes error to eventually approch zero
  double d = gains.d*(error - previous.lastError)/deltaT; //Derivitive - The larger the last change in error the more output will decrease to prevent overshooting of targt
  return {p+i+d, i, error}; //Output is P+I+D, the intergral is saved for the next cycle so it can increase if needed, error is saves for the next cycle for the derivitive can be calculated
}
//For more reading: https://pidexplained.com/how-to-tune-a-pid-controller/

//--------------------------------------------------------------------------------------------------

//Angle

//Converts degrees to radians
double degToRad(double degrees){
  return degrees*PI/180;
}

//Converts radians to degrees
double radToDeg(double radians){
  return radians*180/PI;
}

//Returns any angle between 0 - (360 || 2PI depending if input in in degrees or radians)
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


//Takes two headings and gives you the smallest angle between them in radians; + if the target heading is counter clockwise of the current heading and - if clockwise
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

//Takes two headings and returns the shortest angle between the current heading and a secent line through the circle that passes through the target heading
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

//--------------------------------------------------------------------------------------------------


//Point Class (Stores X and Y coordnates)
class Point{
  public:
    double x;
    double y;
    Point(double X, double Y){
      x = X;
      y = Y;
    }
};

//Vector Class (Repersents a Vector and allows vector operations to be performed)
class Vector{
  private:
    double deltaX; //Change in X repersented by the vector
    double deltaY; //Change in Y repersented of the vector
    
  public:
    //Define Vector by using dX and dY
    Vector(double delta_x, double delta_y){
      deltaX = delta_x;
      deltaY = delta_y;
    }
    //Define Vector using start and end points
    Vector(Point start, Point end){
      deltaX = end.x - start.x;
      deltaY = end.y - start.y;
    }
    //Define Vector by using a magnitiude at a given heading
    Vector(double magnitude, double theta, bool inDegrees){
      if(inDegrees){
        theta = degToRad(theta);
      }
      deltaX = magnitude * cos(theta);
      deltaY = magnitude * sin(theta);
    }

    //Returns dX
    double getX(){
      return deltaX;
    }
    //Returns dY
    double getY(){
      return deltaY;
    }
    //Returns the magnitude of the vector
    double getMagnitude(){
      return sqrt(deltaX*deltaX + deltaY*deltaY);
    }
    //Returns the dot product of this vector and another (The product of both vector's magnitude, + if in same direction, 0 if perendicular, - if facing opposite directions)
    double dot(Vector otherV){
      return deltaX*otherV.deltaX + deltaY*otherV.deltaY;
    }
    //Returns the angle between this vector and another realitive to this vector in radians (+ is CCW; - is CW)
    double getAngle(Vector vecB) {
      if((getMagnitude() == 0) || (vecB.getMagnitude() == 0)){
        return 0; // Its bound between 0-PI so 2PI is error indication
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
    //Returns a new Vector based on a scaling factor of this vector
    Vector scale(double s){
      return Vector(deltaX*s, deltaY*s);
    }
    //Returns a new vector repersenting the unit vector of this vector
    Vector getUnitVector(){
      if(getMagnitude() == 0){
        return Vector(1, 0);
      }
      return Vector(deltaX/getMagnitude(), deltaY/getMagnitude());
    }
    //Returns a new vector that repersents this vector rotated by theta (+ is CCW; - is CW)
    Vector getRotatedVector(double theta, bool inDegrees=false){
      if(inDegrees){
        theta = degToRad(theta);
      }
      return Vector(deltaX * cos(theta) - deltaY * sin(theta), deltaX * sin(theta) + deltaY * cos(theta));
    }
    //Returns a new vector that repersents this vector projected onto another vector (The component of this vector in the same direction as the other vector)
    Vector project(Vector vec){
      //Projects this on to vec
      if(vec.getMagnitude() == 0){
        return Vector(0, 0);
      }
      return vec.scale(dot(vec) / (vec.getMagnitude() * vec.getMagnitude()));
    }
    
    //Adds two vectors using + (this + otherVector)
    Vector operator + (const Vector& other) {
      return Vector(deltaX + other.deltaX, deltaY + other.deltaY);
    }

    //Subtracts two vectors using - (this - otherVector)
    Vector operator - (const Vector& other) {
      return Vector(deltaX - other.deltaX, deltaY - other.deltaY);
    }

    //Returns a new vector scaled by factor (this * constInt)
    Vector operator * (int scalar) {
      return Vector(deltaX*scalar, deltaY*scalar);
    }
    //Returns a new vector scaled by factor (this * constDouble)
    Vector operator * (double scalar) {
      return Vector(deltaX*scalar, deltaY*scalar);
    }
    //Returns a new point that results from adding a vector from a point (this + point)
    Point operator + (Point &p){
      return Point(p.x + getX(), p.y + getY());
    }
    Point operator - (Point &p){
      return Point(p.x - getX(), p.y - getY());
    }
};

//--------------------------------------------------------------------------------------------------

//A dynamic arry points; Allows for any ammount of points to be stored safely in a list
class smartPointPointer{
  public:
  Point* data = (Point*)malloc(0); //Array that stores points
  int size = 0; //Size of the array

  //Frees data when pointer is no longer used; Ensures safe memory usage
  ~smartPointPointer(){
    free(data);
  }

  //Clears all data from the list
  void clear(){
    size = 0;
    free(data);
    data = (Point*)malloc(0);
  }

  //Adds a point to the end of the list
  void append(Point p){
    size++;
    data = (Point*)realloc(data, sizeof(Point) * size);
    data[size - 1] = p;
  }

  //Returns a point at an index in the list
  Point &operator[](size_t index) {
    return data[index];
  }
};

//--------------------------------------------------------------------------------------------------

//Returns a point along a defined bezier curve at t (0-1)
Point bezierFormula(Point initPoint, Point finalPoint, Point C1, Point C2, double t){
  double inverseT = (1-t);
  double x = inverseT*inverseT*inverseT*initPoint.x + 3*inverseT*inverseT*t*C1.x + 3*inverseT*t*t*C2.x + t*t*t*finalPoint.x;
  double y = inverseT*inverseT*inverseT*initPoint.y + 3*inverseT*inverseT*t*C1.y + 3*inverseT*t*t*C2.y + t*t*t*finalPoint.y;
  return Point(x, y);
}

//Returns a point list that contains (steps) points along a defined bezier curve using 2 end points and 2 control points
smartPointPointer generatePath(Point initPoint, Point finalPoint, Point C1, Point C2, const int steps=10){
  smartPointPointer result = smartPointPointer();
  for(double t=0; t<=1; t=t+(1.0/steps)){
    result.append(bezierFormula(initPoint, finalPoint, C1, C2, t));
  }
  return result;
}

//Returns a point list that contains (steps) points along a defined bezier curve using 2 end points and 2 control points defined realitive to the endpoints
smartPointPointer generatePath(Point initPoint, Point finalPoint, Vector V1, Vector V2, const int steps=10){
  smartPointPointer result = smartPointPointer();
  for(double t=0; t<=1; t=t+(1.0/steps)){
    result.append(bezierFormula(initPoint, finalPoint, V1+initPoint, V2+finalPoint, t));
  }
  return result;
}
#endif