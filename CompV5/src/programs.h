#include "Intergration.h"

void autonomous(void){
  while(true){
    updatePosition();
    setDPS(robot.getSpeedVars());
    //Move to position, perform other actions if inside area, update next target after reaching area

    wait(15, msec);
  }
}