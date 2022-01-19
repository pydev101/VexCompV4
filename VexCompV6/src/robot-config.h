using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor leftA;
extern motor rightA;
extern motor rightB;
extern motor arm;
extern motor intakeM;
extern motor leftC;
extern motor rightC;
extern motor leftB;
extern controller Controller1;
extern inertial Inertial;
extern digital_out backPne;
extern digital_out frontPne;
extern signature BackCam__YELLOWGOAL;
extern signature BackCam__BLUEGOAL;
extern signature BackCam__REDGOAL;
extern signature BackCam__SIG_4;
extern signature BackCam__SIG_5;
extern signature BackCam__SIG_6;
extern signature BackCam__SIG_7;
extern vision BackCam;
extern limit LimitBack;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );