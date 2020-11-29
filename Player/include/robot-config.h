using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor frontLeft;
extern motor frontRight;
extern motor backLeft;
extern motor backRight;
extern motor intakeLeft;
extern motor intakeRight;
extern motor liftT;
extern motor liftB;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );