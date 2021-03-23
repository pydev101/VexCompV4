#include "math.h"
#include <iostream>
const double PI = 3.14159;

//Absolute value for double type varibles
double abs(double x){
  if(x < 0){x=x*-1;}
  return x;
}

//Returns 1 or -1 depending on the sign of input number
int getSign(double x){
  if(x < 0){return -1;}
  return 1;
}

//Returns an angle bounded by 0<X<360
double getStandardAngle(double ang, bool isRad=true){
  double toss;
  if(isRad){
    double r = (2*PI)*modf(ang/(2*PI), &toss);
    if(r<0){r+=(2*PI);}
    return r;
  }else{
    double r = (360)*modf(ang/(360), &toss);
    if(r<0){r+=(360);}
    return r;
  }
}

//Converts radians and degrees back and forth
double convert(double head, bool toRad=true){
  if(toRad){
    return head*(PI/180);
  }else{
    return head*(180/PI);
  }
}

//Point structure to store target and position data
typedef struct{
  double x;
  double y;
  double head;
} Point;

typedef struct{
  double primaryGain;
  double adjGain;
  double stopThreshold;
  double slowThreshold;
  double stopPCTofMin;
  double maxAccel;
} PIDVarible;

//Type of value getError(dirT) returns
enum MeasureType {X, Y, HEAD, SHORTANGLE, GRID, POLAR} dirT;
enum GearRatio {GR18To1, GR36to1, GR6to1} motorRatios;
//Primary control of robot
class Robot{
private:
  //Units of radians for angles or same units of length as the measurment of the wheel diameter
  Point pos; //Current X, Y, and Bearing of Robot
  Point tPos; //Target X, Target Y, Target Heading
  double speedTargets[2] = {0.0, 0.0}; //Left and Right target speed values
  double maxLinMoveSpeed; //Maximum allowed travel speed
  double maxLinMoveSpeedDefault; //Maximum speed able to be traveled 
  double minLinMoveSpeed; //Minimum target speed the robot starts moving at
  double minRotSpeed; //Minimum rotation speed in rad
  double maxRotSpeed; //Maximum rotation speed in rad

  PIDVarible linearPID;
  PIDVarible anglePID;

  double desiredTHead;
  double unitsToEncoders; // (Degrees/Units) used for conversion of distances
  double robotRadius; //Distance from center of robot to drive wheel base in units of motor degrees

  bool breakModeLin = false;
  bool breakModeRot = false;
  bool isLinStopped = true;
  bool isRotStopped = true;

  //Limited Proportinal Only control
  double calcLinearSpeed(){
    static double lastSpeed = 0;
    double e = getError(POLAR);

    double min = minLinMoveSpeed*linearPID.stopPCTofMin;
    double adjSpeed = (linearPID.adjGain*linearPID.slowThreshold)+min;
    double changeSpeed;

    if(abs(e) <= linearPID.slowThreshold){
      changeSpeed = (e*linearPID.adjGain) + (min*getSign(e)) - lastSpeed;
    }else{
      changeSpeed = ((((abs(e)-linearPID.slowThreshold)*linearPID.primaryGain) + adjSpeed)*getSign(e)) - lastSpeed;
    }

    if(breakModeLin){
      changeSpeed = -lastSpeed;
    }

    if(abs(changeSpeed) > linearPID.maxAccel){
      changeSpeed = linearPID.maxAccel*getSign(changeSpeed);
    }
    lastSpeed = lastSpeed + changeSpeed; //NewSpeed to set

    //Handle max speed
    if(abs(lastSpeed) > maxLinMoveSpeed){
      if(abs(abs(lastSpeed) - maxLinMoveSpeed) < linearPID.maxAccel){
        lastSpeed = maxLinMoveSpeed*getSign(lastSpeed);
      }else{
        lastSpeed = abs(abs(lastSpeed)-linearPID.maxAccel)*getSign(lastSpeed);
      }
    }

    if(abs(lastSpeed) < minLinMoveSpeed){
      lastSpeed = 0;
      isLinStopped = true;
    }else{
      isLinStopped = false;
    }

    return lastSpeed*unitsToEncoders; //Encoders
  }

  double calcRotationalSpeed(){
    double e = getError(SHORTANGLE);
    static double lastError = e;
    double min = minRotSpeed*anglePID.stopPCTofMin;
    double adjSpeed = (anglePID.adjGain*anglePID.slowThreshold)+min;
    double changeSpeed;

    static double lastSpeed = 0;
    if(abs(e) <= anglePID.slowThreshold){
      changeSpeed = (e*anglePID.adjGain) + (min*getSign(e)) - lastSpeed;
    }else{
      changeSpeed = ((((abs(e)-anglePID.slowThreshold)*anglePID.primaryGain) + adjSpeed)*getSign(e)) - lastSpeed;
    }

    if(breakModeRot){
      changeSpeed = -lastSpeed;
    }

    if(abs(changeSpeed) > anglePID.maxAccel){
      changeSpeed = anglePID.maxAccel*getSign(changeSpeed);
    }
    lastSpeed = lastSpeed + changeSpeed; //NewSpeed to set

    //Handle max speed
    if(abs(lastSpeed) > maxRotSpeed){
      if(abs(abs(lastSpeed) - maxRotSpeed) < anglePID.maxAccel){
        lastSpeed = maxRotSpeed*getSign(lastSpeed);
      }else{
        lastSpeed = abs(abs(lastSpeed)-anglePID.maxAccel)*getSign(lastSpeed);
      }
    }


    if(abs(lastSpeed) < minRotSpeed){
      lastSpeed = 0;
      isRotStopped = true;
    }else{
      isRotStopped = false;
    }

    std::cout << lastSpeed << ", " << getError(SHORTANGLE) << ", " << abs(abs(e) - abs(lastError)) << std::endl;
    lastError = e;
    return lastSpeed; //Rad
  }


public:
  //X in units, y in units, Head in Rads, minSpeeds in deg/s, maxSpeeds in deg/s, wheelDiamter in units, robot drive base width in units
  Robot(double x, double y, double Head, double minSpeeds, double maxSpeed, double wheelDiameter, double width,
        PIDVarible rotPIDVars, PIDVarible linPIDVars){
    pos = {x,y, Head};
    desiredTHead = Head;
    unitsToEncoders = 360/(PI*wheelDiameter);
    minLinMoveSpeed = minSpeeds/unitsToEncoders;
    maxLinMoveSpeed = maxSpeed/unitsToEncoders;
    maxLinMoveSpeedDefault = maxLinMoveSpeed;
    robotRadius = width*0.5*unitsToEncoders;
    minRotSpeed = (2*minLinMoveSpeed)/(wheelDiameter/2); //Might be better if set directly by programmer TODO
    maxRotSpeed = (2*maxLinMoveSpeed)/(wheelDiameter/2);

    linearPID = linPIDVars;
    anglePID = rotPIDVars;
  }

  double getError(MeasureType d){
    if(d == X){
      return tPos.x-pos.x;
    }else if(d == Y){
      return tPos.y-pos.y;
    }else if(d == HEAD){
      return getStandardAngle(tPos.head)-getStandardAngle(pos.head);
      //Or using the motor encoders (rightEnc-leftEnc)/(2*radius) or the difference between the encoders divided by the distance between the drive wheels
    }else if(d == SHORTANGLE){
      double e = getError(HEAD);
      if(abs(e) > PI){
        //Other path is shorter travel
        if(e < 0){
          //If E is CW then faster path is CCW
          e = (2*PI)+e;
        }else{
          //If E is CCW; faster path is CW
          e = -((2*PI)-e);
        }
      }
      return e;
    }else if(d == GRID){
      return sqrt((getError(X)*getError(X)) + (getError(Y)*getError(Y)));
    }else{
      double r = getError(GRID);
      //If robot is facing target point return pos r, else return -r

      double tar = atan2(getError(Y), getError(X));
      if(tar < 0){
        tar += 2*PI;
      }

      double lowT = tar - (PI/2);
      double highT = tar + (PI/2);
      double curr = getStandardAngle(pos.head);

      //Move the angles to the left side of teh circle in order to avoid the strange 0-360 problems that plague me
      if(getSign(cos(tar)) == 1){
        lowT = getStandardAngle(lowT+PI);
        highT = getStandardAngle(highT+PI);
        curr = getStandardAngle(curr+PI);

        //High is bounded by [180, 360]
        if((0 <= highT) && (highT < PI)){
          highT = 2*PI;
          lowT = PI;
        }
      }

      if((lowT < curr) && (curr < highT)){
        return r;
      }else{
        return -r;
      }
    }
  }

  void updatePos(double Xd, double Yd, double h){
    pos = {(Xd/unitsToEncoders)+pos.x, (Yd/unitsToEncoders)+pos.y, h};
  }

  void resetPos(double x, double y, double h){
    pos = {x, y, h};
  }

  void setTRealitive(double fwd, double hor){
    tPos.x = tPos.x + fwd*cos(desiredTHead) + hor*cos(desiredTHead-(PI/2));
    tPos.y = tPos.y + fwd*sin(desiredTHead) + hor*sin(desiredTHead-(PI/2));
    double ang = atan2((tPos.y - pos.y), (tPos.x-pos.x));
    if(ang<0){ang+=(2*PI);}
    tPos.head = ang;
    desiredTHead = ang;
  }

  void setTAbsolute(double x, double y){
    tPos.x = x;
    tPos.y = y;
    double ang = atan2((tPos.y - pos.y), (tPos.x-pos.x));
    if(ang<0){ang+=(2*PI);}
    tPos.head = ang;
    desiredTHead = ang;
  }

  void setTHead(double head, bool inDegs=false){
    if(inDegs){
      head = (head*PI)/180;
    }
    head = getStandardAngle(head);
    tPos.head = head;
    desiredTHead = head;
  }

  void setMaxSpeed(double pct){
    maxLinMoveSpeed = abs(pct)*maxLinMoveSpeedDefault;
  }
  void setMaxAcceleration(double a){
    linearPID.maxAccel = abs(a);
  }

  double getRadiusInEncoders(){
    return robotRadius;
  }

  //Turns to exact head value
  double* turnToHead(){
    double s = calcRotationalSpeed();
    speedTargets[0] = s*robotRadius*0.5*-1;
    speedTargets[1] = s*robotRadius*0.5;

    return speedTargets;
  }

  //Turns shortest angle to be inline with target; Returns 1 or -1 depending on robot move direction to reach target
  int setToShortestVector(){
    if(abs(getError(HEAD)) <= (PI/2)){
      return 1;
    }else{
      tPos.head = getStandardAngle(tPos.head+PI);
      return -1;
    }
  }

  //Moves in line until target is reached; dir is used to determine if should move straight or in reverse
  double* moveLin(bool angleAdj=true){
    double r = calcRotationalSpeed();
    double s = calcLinearSpeed();
    speedTargets[0] = s;
    speedTargets[1] = s;
    if(angleAdj){
      speedTargets[0] += -r*robotRadius*0.5;
      speedTargets[1] +=  r*robotRadius*0.5;
    }
    return speedTargets;
  }

  //Moves to target pos in quickest direction
  double* move(bool setShortV=true){
    if(getError(GRID) > linearPID.slowThreshold){
      double ang = atan2((tPos.y - pos.y), (tPos.x-pos.x));
      if(ang<0){ang+=(2*PI);}
      tPos.head = ang;
    }
    if(setShortV){
      setToShortestVector();
    }
    return moveLin();
  }

  bool driving(){
     if((getError(GRID) > linearPID.stopThreshold) || (!isLinStopped) || (!isRotStopped)){
      return true;
    }else{
      return false;
    }
  }

  bool turning(){
    //Use for debugging if turn holds too long; likely the adjGain is too low and the speed is zero before theashold is met
    //std::cout << abs(getError(HEAD)) << ", " << speedTargets[0] << ", " << ((abs(getError(HEAD)) > anglePID.stopThreshold) || (!isRotStopped)) << std::endl;
    if((abs(getError(HEAD)) > anglePID.stopThreshold) || (!isRotStopped)){
      return true;
    }else{
      return false;
    }
  }

  Point getPos(){
    return pos;
  }

  Point getTPOS(){
    return tPos;
  }
};