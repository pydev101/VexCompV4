#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor frontLeft = motor(PORT13, ratio18_1, false);
motor frontRight = motor(PORT12, ratio18_1, true);
motor backLeft = motor(PORT11, ratio18_1, false);
motor backRight = motor(PORT14, ratio18_1, true);
encoder rightEncoder = encoder(Brain.ThreeWirePort.C);
encoder leftEncoder = encoder(Brain.ThreeWirePort.E);
encoder horEncoder = encoder(Brain.ThreeWirePort.A);
inertial ISensor = inertial(PORT15);
controller Controller1 = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}