using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftA;
extern motor leftB;
extern motor rightA;
extern motor rightB;
extern motor arm3M;
extern motor arm4M;
extern motor intakeM;
extern motor leftC;
extern motor rightC;
extern inertial Inertial;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );