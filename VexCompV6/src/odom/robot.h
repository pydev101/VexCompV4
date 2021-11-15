#ifndef __ROBOTCON_H__
#define __ROBOTCON_H__
#include "odometry.h"

//Robot for a position give output of a speed to acheive target
class Robot{
  private:
    double linearP;
    double angularP;

  public:
    OdomGrid location = OdomGrid(Point(0,0), 90, true);

    Robot(Point Pos, double CurrentHeading, bool headingGivenInDegrees, double linearKp, double angularKp){
      location = OdomGrid(Pos, CurrentHeading, headingGivenInDegrees);
      linearP = linearKp;
      angularP = angularKp;
    } 

    double * getRotationalSpeed(bool shortestArcToLineV=false){
      double o = location.getThetaError(shortestArcToLineV)*angularP;
      double static output[2] = {-o, o};
      return output;
    }

    double * getLinearSpeed(){
      double o = location.getLinearError()*linearP;
      double static output[2] = {o, o};
      return output;
    }



  //PID, tracking, and speed control (Max accel, Max speed, Constant Velocity, self-adj of PID in order to get ideal settings)
};

#endif