#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftA = motor(PORT1, ratio18_1, true);
motor rightA = motor(PORT3, ratio18_1, false);
motor rightB = motor(PORT4, ratio18_1, true);
motor arm = motor(PORT12, ratio18_1, true);
motor intakeM = motor(PORT7, ratio18_1, true);
motor leftC = motor(PORT8, ratio18_1, true);
motor rightC = motor(PORT9, ratio18_1, false);
motor leftB = motor(PORT21, ratio18_1, false);
controller Controller1 = controller(primary);
inertial Inertial = inertial(PORT15);
digital_out backPne = digital_out(Brain.ThreeWirePort.A);
digital_out frontPne = digital_out(Brain.ThreeWirePort.H);
/*vex-vision-config:begin*/
signature BackCam__YELLOWGOAL = signature (1, 321, 2443, 1382, -6959, -6351, -6654, 9.9, 0);
signature BackCam__BLUEGOAL = signature (2, -2595, -2049, -2322, 8159, 10031, 9094, 4.7, 0);
signature BackCam__REDGOAL = signature (3, 9153, 10063, 9608, -1199, -767, -982, 6.4, 0);
signature BackCam__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature BackCam__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature BackCam__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature BackCam__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision BackCam = vision (PORT5, 50, BackCam__YELLOWGOAL, BackCam__BLUEGOAL, BackCam__REDGOAL, BackCam__SIG_4, BackCam__SIG_5, BackCam__SIG_6, BackCam__SIG_7);
/*vex-vision-config:end*/
limit LimitBack = limit(Brain.ThreeWirePort.E);

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