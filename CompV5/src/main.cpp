/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\carson.easterling                                */
/*    Created:      Fri Dec 04 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  //A
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  char buf[10000];
  vexFileDirectoryGet("", buf, sizeof(buf)-1);
  puts(buf);

  FIL* x = vexFileOpen("lastestAuto.csv", "r");
  std::cout << vexFileSize(x) << std::endl;
  vexFileRead(buf, sizeof(buf)-1, 1, x);
  vexFileClose(x);
  puts(buf);
}