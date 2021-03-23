//#include "vex.h"
#include <stdio.h>
#include "math.h"
#include <iostream>
/*
void log(){
  //WORKS!
  vexFileMountSD();
  FIL* t = vexFileOpenCreate("Testing.txt");
  FIL* f = vexFileOpen("Log.txt", "a");

  char b[] = {'H','e','l','l','o','\0'};
  vexFileWrite(b, sizeof(b), 1, t);
  vexFileClose(t);
}*/

/*
coefficient = randomValue
cost = evaluate(f(coefficient))
delta = derivative(cost) = change of cost dived by change in coefficent
newCoefficient = coefficient — (alpha * delta)
*/

#ifndef __PIDVarible_H__
#define __PIDVarible_H__
typedef struct{
  double primaryGain;
  double adjGain;
  double stopThreshold;
  double slowThreshold;
  double stopPCTofMin;
  double maxAccel;
} PIDVarible;
#endif

typedef struct{
  PIDVarible vars;
  int numberOfRuns;
  double sumOfCost;
} Trial;

double calcCost(double error, double t){
  return (error*error)*t;
}

double alpha = 0.1;

double* getDeltas(Trial currentTrial, Trial oldTrial){
  static double arr[4];
  double deltaCost = (currentTrial.sumOfCost/currentTrial.numberOfRuns)-(oldTrial.sumOfCost/oldTrial.numberOfRuns);
  arr[0] = (currentTrial.vars.primaryGain-oldTrial.vars.primaryGain)/deltaCost;
  arr[1] = (currentTrial.vars.adjGain-oldTrial.vars.adjGain)/deltaCost;
  arr[2] = (currentTrial.vars.slowThreshold-oldTrial.vars.slowThreshold)/deltaCost;
  arr[3] = (currentTrial.vars.stopPCTofMin-oldTrial.vars.stopPCTofMin)/deltaCost;
  return arr;
}

Trial optimize(Trial currentTrial, Trial oldTrial){
  double* deltas = getDeltas(currentTrial, oldTrial);
  currentTrial.vars.primaryGain = currentTrial.vars.primaryGain - (alpha * (*(deltas + 0)));
  currentTrial.vars.adjGain = currentTrial.vars.adjGain - (alpha * (*(deltas + 1)));
  currentTrial.vars.slowThreshold = currentTrial.vars.slowThreshold - (alpha * (*(deltas + 2)));
  currentTrial.vars.stopPCTofMin = currentTrial.vars.stopPCTofMin - (alpha * (*(deltas + 3)));
  return currentTrial;
}

