using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor frontLeft;
extern motor frontRight;
extern motor backLeft;
extern motor backRight;
extern encoder rightEncoder;
extern encoder leftEncoder;
extern encoder horEncoder;
extern inertial ISensor;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );