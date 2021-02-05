//#include "vex.h"
#include <stdio.h>
#include "math.h"
#include <iostream>

//using namespace vex;

typedef struct{
  int ID;
  double roationalGainA;
  double maxRotAccelerationA;
  double linearGainA;
  double maxLinAccelerationA;
} RobotProfile;

typedef struct{
  int ID;
  RobotProfile settings;
  double sumTrialTime;
  double sumTrialError;
  double runs;
} Trial;

double calculateCost(Trial n){
  return ((n.sumTrialError/n.runs)*(n.sumTrialError/n.runs)*(n.sumTrialTime/n.runs));
}

//char mode = 'w';
//FIL* x = vexFileOpen("Hello", &mode); test if other doesn't work
void test(){
  //WORKS!
  vexFileMountSD();
  FIL* t = vexFileOpenCreate("Testing.txt");
  char b[] = {'H','e','l','l','o','\0'};
  vexFileWrite(b, sizeof(b), 1, t);
  vexFileClose(t);
}