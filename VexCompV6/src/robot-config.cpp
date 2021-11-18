#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftA = motor(PORT1, ratio18_1, true);
motor rightA = motor(PORT3, ratio18_1, false);
motor rightB = motor(PORT11, ratio18_1, true);
motor arm = motor(PORT12, ratio18_1, false);
motor intakeM = motor(PORT7, ratio18_1, true);
motor leftC = motor(PORT8, ratio18_1, true);
motor rightC = motor(PORT9, ratio18_1, false);
motor leftB = motor(PORT21, ratio18_1, false);
controller Controller1 = controller(primary);
inertial Inertial = inertial(PORT19);
digital_out backPne = digital_out(Brain.ThreeWirePort.A);
digital_out frontPne = digital_out(Brain.ThreeWirePort.H);

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