#include "math.h"
const double PI = 3.14159;

double abs(double x){
  if(x < 0){x=x*-1;}
  return x;
}

int getSign(double x){
  if(x < 0){return -1;}
  return 1;
}

double getStandardAngle(double ang, bool isRad=true){
  double toss;
  if(isRad){
    return ((2*PI)*modf(ang/(2*PI), &toss));
  }else{
    return (360*modf(ang/360, &toss));
  }
}

double convert(double head, bool toRad=true){
  if(toRad){
    return head*(PI/180);
  }else{
    return head*(180/PI);
  }
}

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

class Area: Rect{
  void (*callback)(void);
  Area(double x, double y, double width, double height, void (*call)(void)):Rect(x, y, width, height){
    callback = call;
  }
};

enum MeasureType {X, Y, HEAD, GRID} dirT;
class Robot{
private:
  Point pos;
  Point tPos;
  double speedTargets[2] = {0.0,0.0};

  double maxMoveSpeed = 100000;
  double maxAcceleration = 50;
  double stopSpeed;
  bool breakMode = false;
  bool isRotating = false;
  bool isStopped = true;

  double radius; //The length between the inertal sensor and the left drive base
  double unitsToEncoders;

  //Helper

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
  Robot(double x, double y, double Head, double stopSpeeds, double wheelRadius){
    pos = {x,y, Head};
    unitsToEncoders = 360/(2*PI*wheelRadius);
    radius = wheelRadius*unitsToEncoders;
    stopSpeed = stopSpeeds;
  }

  double getError(MeasureType d){
    if(d == X){
      return tPos.x-pos.x;
    }else if(d == Y){
      return tPos.y-pos.y;
    }else if(d == HEAD){
      return getStandardAngle(tPos.head)-getStandardAngle(pos.head);
      //Or using the motor encoders (rightEnc-leftEnc)/(2*radius) or the difference between the encoders divided by the distance between the drive wheels
    }else{
      return sqrt((getError(X)*getError(X)) + (getError(Y)*getError(Y)));
    }
  }

  void updatePos(double Xd, double Yd, double h){
    pos = {Xd+pos.x, Yd+pos.y, h};
  }

  void setTRealitive(double fwd, double hor){
    tPos.x = tPos.x + fwd*cos(pos.head) + hor*cos(pos.head-(PI/2));
    tPos.y = tPos.y + fwd*sin(pos.head) + hor*sin(pos.head-(PI/2));
    tPos.head = atan2((tPos.y - pos.y), (tPos.x-pos.x));
  }

  void setTAbsolute(double x, double y){
    tPos.x = x;
    tPos.y = y;
    tPos.head = atan2((tPos.y - pos.y), (tPos.x-pos.x));
  }

  void setTHead(double head){
    tPos.x = pos.x;
    tPos.y = pos.y;
    tPos.head = head;
  }

  //TODO Set Speed to rotate if Heading is past threasehold
  double* getSpeedVars(int dir=1){
    speedTargets[1] = calcLinearSpeed(dir) + (dir*calcRotationalSpeed()*radius*0.5); //Right
    speedTargets[0] = calcLinearSpeed(dir) - (dir*calcRotationalSpeed()*radius*0.5); //Left

    if((abs(speedTargets[0]) < stopSpeed) && (abs(speedTargets[1]) < stopSpeed)){
      isStopped = true;
    }else{
      isStopped = false;
    }

    return speedTargets;
  }

  void setMaxSpeed(double s){
    maxMoveSpeed = s;
  }
  void setMaxAcceleration(double a){
    maxAcceleration = a;
  }

};