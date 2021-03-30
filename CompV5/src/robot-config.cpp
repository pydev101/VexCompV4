#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor motorLF = motor(PORT8, ratio18_1, true);
motor motorLB = motor(PORT7, ratio18_1, false);
motor motorRF = motor(PORT5, ratio18_1, true);
motor motorRB = motor(PORT10, ratio18_1, true);
inertial ISensor = inertial(PORT1);
controller Controller1 = controller(primary);
motor topIntake = motor(PORT19, ratio18_1, false);
motor bottomIntake = motor(PORT18, ratio18_1, false);
motor rightIntake = motor(PORT12, ratio18_1, true);
motor leftIntake = motor(PORT13, ratio18_1, false);
/*vex-vision-config:begin*/
vision VisionS = vision (PORT6, 50);
/*vex-vision-config:end*/

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