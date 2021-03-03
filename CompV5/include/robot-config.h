using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor motorLF;
extern motor motorLB;
extern motor motorRF;
extern motor motorRB;
extern inertial ISensor;
extern controller Controller1;
extern motor topIntake;
extern motor bottomIntake;
extern motor rightIntake;
extern motor leftIntake;
extern signature VisionS__SIG_1;
extern signature VisionS__SIG_2;
extern signature VisionS__SIG_3;
extern signature VisionS__SIG_4;
extern signature VisionS__SIG_5;
extern signature VisionS__SIG_6;
extern signature VisionS__SIG_7;
extern vision VisionS;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );