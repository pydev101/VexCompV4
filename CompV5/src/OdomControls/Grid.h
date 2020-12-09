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
private:
  Point pos;
  Point tPos;
  double speedTargets[2] = {0,0};

  double maxMoveSpeed = 100000;
  double maxAcceleration = 50;
  double stopSpeed;
  bool breakMode = false;
  bool isRotating = false;
  bool isStopped = true;

  double radius; //The length between the inertal sensor and the left drive base
  double unitsToEncoders;

  Robot(double x, double y, double Head, double stopSpeeds, double r, double unitToEnc){
    pos = {x,y, Head};
    radius = r*unitToEnc;
    unitsToEncoders = unitToEnc;
    stopSpeed = stopSpeeds;
  }

  //Helper
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

  //Limited Proportinal Only control
  double calcLinearSpeed(int dir, double gain=1.386){
    static double lastSpeed = 0;
    double changeSpeed = (dir*getError(GRID)*gain) - lastSpeed;
    if(abs(changeSpeed) > maxAcceleration){
      changeSpeed = maxAcceleration*getSign(changeSpeed);
    }
    lastSpeed = lastSpeed + changeSpeed; //NewSpeed to set

    //Handle max speed
    if(abs(lastSpeed) > maxMoveSpeed){
      if(abs(abs(lastSpeed) - maxMoveSpeed) < maxAcceleration){
        lastSpeed = maxMoveSpeed*getSign(lastSpeed);
      }else{
        lastSpeed = abs(abs(lastSpeed)-maxAcceleration)*getSign(lastSpeed);
      }
    }
    return lastSpeed*unitsToEncoders; //Encoders
  }

  double calcRotationalSpeed(double maxRotSpeed=100000, double maxRotAcceleration=50, double gain=1){
    double e = getError(HEAD);
    if(abs(e) > PI){
      if(e < 0){
        e = (2*PI)+e;
      }else{
        e = -((2*PI)-e);
      }
    }

    static double lastSpeed = 0;
    double changeSpeed = (e*gain) - lastSpeed;
    if(abs(changeSpeed) > maxRotAcceleration){
      changeSpeed = maxRotAcceleration*getSign(changeSpeed);
    }
    lastSpeed = lastSpeed + changeSpeed; //NewSpeed to set

    //Handle max speed
    if(abs(lastSpeed) > maxRotSpeed){
      if(abs(abs(lastSpeed) - maxRotSpeed) < maxRotAcceleration){
        lastSpeed = maxRotSpeed*getSign(lastSpeed);
      }else{
        lastSpeed = abs(abs(lastSpeed)-maxRotAcceleration)*getSign(lastSpeed);
      }
    }
    return lastSpeed; //Rad
  }


public:
  void setPos(double x, double y, double h){
    pos = {x, y, h};
  }



  double[] getSpeedVars(int dir=1, double breakGain = 1.3){
    if(!breakMode){
      speedTargets[1] = calcLinearSpeed(dir);
      speedTargets[0] = speedTargets[1]  - (calcRotationalSpeed()*radius);
    }else{

    }

    if((abs(speedTargets[0]) < stopSpeed) && (abs(speedTargets[0]) < stopSpeed)){
      isStopped = true;
    }else{
      isStopped = false;
    }

    return speedTargets;
  }

  //Odometry Controls
  void setRealitive(double fwd, double hor){
    tPos.x = tPos.x + fwd*cos(pos.head) + hor*cos(pos.head-(PI/2));
    tPos.y = tPos.y + fwd*sin(pos.head) + hor*sin(pos.head-(PI/2));
    tPos.head = atan2((tPos.y - pos.y), (tPos.x-pos.x));
  }

  void setAbsolute(double x, double y){
    tPos.x = x;
    tPos.y = y;
    tPos.head = atan2((tPos.y - pos.y), (tPos.x-pos.x));
  }
};