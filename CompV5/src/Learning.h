//#include "vex.h"
#include <stdio.h>
#include "math.h"
#include <iostream>

//using namespace vex;

typedef struct{
  double gain;
  double maxAcceleration;
} RobotProfile;

typedef struct{
  int ID; //Identifies which a unique base profile
  int index; //Identifies which variation of the base profile is being tested; 0 is the base profile itself
  bool isRotation; //Identifies if this is a linear or rotation adjustment trial
  double runs;
  RobotProfile settings;
  double sumTrialTime;
  double sumTrialError;
} Trial;

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

double calculateCost(Trial n){
  return ((n.sumTrialError/n.runs)*(n.sumTrialError/n.runs)*(n.sumTrialTime/n.runs));
}


void test(){
  //WORKS!
  vexFileMountSD();
  FIL* t = vexFileOpenCreate("Testing.txt");
  char b[] = {'H','e','l','l','o','\0'};
  vexFileWrite(b, sizeof(b), 1, t);
  vexFileClose(t);
}