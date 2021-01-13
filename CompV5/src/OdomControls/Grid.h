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


enum MeasureType {X, Y, HEAD, GRID} dirT;
class Robot{
private:
  Point pos;
  Point tPos;
  double speedTargets[2] = {0.0,0.0};

  //TODO HAVE MAX/MIN ROT AND LIN SPEEDS BE DETERMINED FROM CONSTRUCTUR ARGS
  double maxMoveSpeed = 100000;
  double minMoveSpeed = 0.1;
  double maxAcceleration = 50;
  double linThreshold = 1;
  double angleThreshold = PI/60;
  double stopSpeed;
  bool breakMode = false;
  bool isRotating = false;
  bool isStopped = true;
  double unitsToEncoders;
  double robotRadius;

  //Limited Proportinal Only control
  double calcLinearSpeed(int dir, double gain=1.386){
    static double lastSpeed = 0;
    double changeSpeed = (dir*getError(GRID)*gain) - lastSpeed;
    if(abs(getError(GRID)) < linThreshold){
      changeSpeed = -lastSpeed;
    }
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

    if(abs(lastSpeed) < minMoveSpeed){
      lastSpeed = 0;
    }
    return lastSpeed*unitsToEncoders; //Encoders
  }

  double calcRotationalSpeed(double minRotSpeed=0.1, double maxRotSpeed=100000, double maxRotAcceleration=50, double gain=1){
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
    if(abs(e) < angleThreshold){
      changeSpeed = -lastSpeed;
    }

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

    if(abs(lastSpeed) < minRotSpeed){
      lastSpeed = 0;
    }
    return lastSpeed; //Rad
  }


public:
  Robot(double x, double y, double Head, double stopSpeeds, double wheelRadius, double width){
    pos = {x,y, Head};
    unitsToEncoders = 360/(2*PI*wheelRadius);
    stopSpeed = stopSpeeds;
    robotRadius = width*0.5*unitsToEncoders;
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

  void setMaxSpeed(double s){
    maxMoveSpeed = s;
  }
  void setMaxAcceleration(double a){
    maxAcceleration = a;
  }

  void setAngleThres(double thres){
    angleThreshold = thres;
  }

  void setLinThres(double thres){
    linThreshold = thres;
  }

  //Turns to exact head value
  double* turnToHead(){
    speedTargets[0] = calcRotationalSpeed()*robotRadius*-0.5;
    speedTargets[1] = calcRotationalSpeed()*robotRadius*0.5;
    return speedTargets;
  }

  //Turns shortest angle to be inline with target; Returns 1 or -1 depending on robot move direction to reach target
  int setToShortestVector(){
    if(abs(getError(HEAD)) <= (PI/2)){
      return 1;
    }else{
      tPos.head = tPos.head+180;
      return -1;
    }
  }

  //Moves in line until target is reached; dir is used to determine if should move straight or in reverse
  double* moveLin(int dir){
    speedTargets[0] = calcLinearSpeed(dir)+dir*calcRotationalSpeed()*robotRadius*-0.5;
    speedTargets[1] = calcLinearSpeed(dir)+dir*calcRotationalSpeed()*robotRadius*0.5;
    return speedTargets;
  }

  //Moves to target pos in quickest direction
  double* move(){
    int dir = setToShortestVector();
    if(abs(getError(HEAD)) > angleThreshold){
      return turnToHead();
    }
    return moveLin(dir);
  }

};