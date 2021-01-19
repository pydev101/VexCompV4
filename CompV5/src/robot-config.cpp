#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor motorLF = motor(PORT13, ratio18_1, true);
motor motorLB = motor(PORT14, ratio18_1, false);
motor motorRF = motor(PORT11, ratio18_1, false);
motor motorRB = motor(PORT12, ratio18_1, true);
inertial ISensor = inertial(PORT15);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}