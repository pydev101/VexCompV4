#include "vex.h"

typedef struct{
  double x;
  double y;
  double head;
} Point;

class Rect{
  public:
    Point topLeft;
    Point bottomRight;
    Point center;

    Rect(double x, double y, double width, double height){
      topLeft = {x, y};
      bottomRight = {x+width, y+height};
      center = {x+(0.5*width), y+(0.5*height)};
    }

    Rect(Point topLeft, Point bottomRight){
      this->topLeft = topLeft;
      this->bottomRight = bottomRight;
      center = {(topLeft.x+bottomRight.x)*0.5, (topLeft.y+bottomRight.y)*0.5};
    }

    bool checkCollide(Point p){
      if((p.x >= topLeft.x) && (p.x <= bottomRight.x)){
        if((p.y >= topLeft.y) && (p.y <= bottomRight.y)){
          return true;
        }else{
          return false;
        }
      }else{
        return false;
      }
  }
};

enum MeasureType {X, Y, HEAD, GRID} dirT;
class Robot{
  static Point pos;
  static Point tPos;

  Robot(double x, double y, double Head){
    pos = {x,y, Head};
  }

  double calcLinearSpeed(int dir, double maxSpeed=100000, double maxAcceleration=50, double gain=1.386){
    static double lastSpeed = 0;
    double changeSpeed = (dir*getError(GRID)*gain) - lastSpeed;
    if(abs(changeSpeed) > maxAcceleration){
      changeSpeed = maxAcceleration*getSign(changeSpeed);
    }
    lastSpeed = lastSpeed + changeSpeed; //NewSpeed to set

    //Handle max speed
    if(abs(lastSpeed) > maxSpeed){
      if(abs(abs(lastSpeed) - maxSpeed) < maxAcceleration){
        lastSpeed = maxSpeed*getSign(lastSpeed);
      }else{
        lastSpeed = abs(abs(lastSpeed)-maxAcceleration)*getSign(lastSpeed);
      }
    }
    return lastSpeed;
  }

  double calcRotationalSpeed(double maxSpeed=100000, double maxAcceleration=50, double gain=1.386){
    double static lastSpeed = 0;
    double e = getError(HEAD);
    if(abs(e) > 180){
      if(e < 0){
        e = 360+e; //Validate math
      }else{
        e = -(360-e); //Validate math
      }
    }

  }

  double getError(MeasureType d){
    if(d == X){
      return tPos.x-pos.x;
    }else if(d == Y){
      return tPos.y-pos.y;
    }else if(d == HEAD){
      return getStandardAngle(tPos.head)-getStandardAngle(pos.head);
    }else{
      return sqrt((getError(X)*getError(X)) + (getError(Y)*getError(Y)));
    }
  }
};