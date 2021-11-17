using namespace vex;

extern brain Brain;

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

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );