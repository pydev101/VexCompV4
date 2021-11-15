#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftA = motor(PORT1, ratio18_1, false);
motor leftB = motor(PORT2, ratio18_1, false);
motor rightA = motor(PORT3, ratio18_1, false);
motor rightB = motor(PORT4, ratio18_1, false);
motor arm3M = motor(PORT5, ratio18_1, false);
motor arm4M = motor(PORT6, ratio18_1, false);
motor intakeM = motor(PORT7, ratio18_1, false);
motor leftC = motor(PORT8, ratio18_1, false);
motor rightC = motor(PORT9, ratio18_1, false);
inertial Inertial = inertial(PORT10);
digital_out pne = digital_out(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}