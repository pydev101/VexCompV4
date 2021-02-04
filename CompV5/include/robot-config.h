using namespace vex;

extern brain Brain;

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

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );