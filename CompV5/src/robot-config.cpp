#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor motorLF = motor(PORT1, ratio18_1, false);
motor motorLB = motor(PORT2, ratio18_1, false);
motor motorRF = motor(PORT3, ratio18_1, false);
motor motorRB = motor(PORT4, ratio18_1, false);
inertial ISensor = inertial(PORT5);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}