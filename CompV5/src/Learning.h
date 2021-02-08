//#include "vex.h"
#include <stdio.h>
#include "math.h"
#include <iostream>

//using namespace vex;

typedef struct{
  double gain;
  double maxAcceleration;
} RobotProfile;

//Alogorithm: If cost is better than current best; set as new best and reset training varibles; else change the variations
//If particlar variations appear to have no effect multiple adj by 0.75 of its current value

typedef struct{
  int ID; //Identifies which a unique base profile
  int gainIndex; //Multiplied by learning value to get variation for current test
  int accelIndex; //Multiplied by learning value to get variation for current test
  bool isRotation; //Identifies if this is a linear or rotation adjustment trial
  double bestCost;
  RobotProfile bestSettings;
  RobotProfile testSettings;
  double sumTrialVelocity;
  double sumTrialError;
  double runs;
} Trial;

typedef struct{
  Trial currentLinTrial;
  Trial currentRotTrial;
  double linGainAdj;
  double linAccelAdj;
  double rotGainAdj;
  double rotAccelAdj;
} settingsType;
//Log each completed trial to "Logs.txt" (Used for graphing and external data analysis)
//Log the current trial set (All trials that share an ID) to "settings.txt" (This is used to load the most recent training set for faster load times)
/*Format (For both files):
ID, index, isRotation, runs, gain, maxAcceleration, sumTrialTime, sumTrialError\n
*/
/*Algoritm:
For each base profile start a new trial set with unique ID
For each variation of a base profile keep the ID but update the index to identify type of change; Once 3 runs are complete create and move to new variation
After all variations calculate cost and create new base profile based on trend; If a trend on an axis is increasing cost in both directions or if both are equal decrease the change of variation for that axis 
*/
/*File Loading:
Read settings.txt and for each new line create a Trial struct
Add linear trials to one vector list and rotational trials to a differnt vector list
*/

double absLearn(double x){
  if(x < 0){x=x*-1;}
  return x;
}

double calculateCost(Trial n){
  double e = n.sumTrialError / n.runs;
  double v = n.sumTrialVelocity / n.runs;
  return e/v;
}

void learn(double e, double d, double t, Trial currentTrial){
  e = absLearn(e); d = absLearn(d); t = absLearn(t); double v = d/t;
  currentTrial.sumTrialError += e;
  currentTrial.sumTrialVelocity += v;
  currentTrial.runs += 1;

  if(currentTrial.runs == 3){
    
  }

}

void test(){
  //WORKS!
  vexFileMountSD();
  FIL* t = vexFileOpenCreate("Testing.txt");
  char b[] = {'H','e','l','l','o','\0'};
  vexFileWrite(b, sizeof(b), 1, t);
  vexFileClose(t);
}