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

//Absolute value for double type varibles
double absLEARNING(double x){
  if(x < 0){x=x*-1;}
  return x;
}

#ifndef __PIDVarible_H__
#define __PIDVarible_H__
typedef struct{
  double PGain; //Proportional coefficent
  double IGain; //Intergal coefficent
  double DGain; //Derivitive coefficent
  double stopThreshold; //Final threashold
  double slowThreshold; //Determiens if change in position is a stop
  double maxAccel; //Max Acceleration
} PIDVarible;
#endif

typedef struct{
  PIDVarible vars;
  int numberOfRuns;
  double sumOfCost;
} Trial;

double calcCost(double error, double t){
  return absLEARNING(error)*t;
}

double alpha = 0.1;

double* getDeltas(Trial currentTrial, Trial oldTrial){
  static double arr[2];
  double deltaCost = (currentTrial.sumOfCost/currentTrial.numberOfRuns)-(oldTrial.sumOfCost/oldTrial.numberOfRuns);
  arr[0] = (currentTrial.vars.PGain-oldTrial.vars.PGain)/deltaCost;
  arr[1] = (currentTrial.vars.IGain-oldTrial.vars.IGain)/deltaCost;
  return arr;
}

Trial optimize(Trial currentTrial, Trial oldTrial){
  double* deltas = getDeltas(currentTrial, oldTrial);
  currentTrial.vars.PGain = currentTrial.vars.PGain - (alpha * (*(deltas + 0)));
  currentTrial.vars.IGain = currentTrial.vars.IGain - (alpha * (*(deltas + 1)));
  return currentTrial;
}

Trial oldRotTrial;
Trial oldLinTrial;

Trial currRotTrial;
Trial currLinTrial;

PIDVarible train(double error, double t, bool isRot){
  if(isRot){
    currRotTrial.sumOfCost += calcCost(error, t);
    currRotTrial.numberOfRuns += 1;

    if(currRotTrial.numberOfRuns >= 3){
      Trial placeholder = optimize(currRotTrial, oldRotTrial);
      oldRotTrial = currRotTrial;
      currRotTrial = placeholder;
    }
    return currRotTrial.vars;
  }else{
    currLinTrial.sumOfCost += calcCost(error, t);
    currLinTrial.numberOfRuns += 1;

    if(currLinTrial.numberOfRuns >= 3){
      Trial placeholder = optimize(currLinTrial, oldLinTrial);
      oldLinTrial = currLinTrial;
      currLinTrial = placeholder;
    }
    return currLinTrial.vars;
  }
}