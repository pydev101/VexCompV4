//#include "vex.h"
#include <fstream>
#include "math.h"

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
std::ofstream myfile;
void test(){
  myfile.open("/usd/example.txt", std::ios::out);
  myfile << "Hello World" << std::endl;
  myfile.close();
}