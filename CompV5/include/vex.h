/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)

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