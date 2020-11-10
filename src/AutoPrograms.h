/*----------------------------------------------------------------------------
//                                                                            
//    Module:       BaseMotor.h                                               
//    Author:       Carson Easterling                                    
//    Created:      Thu Sep 10 2020                                           
//    Description:  Header file that contains top level game functions for the automous portains of the compeition                                           
//                                                                            
//--------------------------------------------------------------------------*/

#include "DriveControl/Odom.h"

typedef struct {
  std::string name;
  void (*function)(void);

} autoEntry;

void leftBlue(){

}

void leftRed(){
  
}

void rightBlue(){
  
}

void rightRed(){
  intake(1);
  move(10, 0);
  move(12, 54);
  turnTo(90);
  move(60, 0, 500.0);
  intake(0);
}

autoEntry entries[] = {
  {"Left Blue", leftBlue}
};

autoEntry selectedAutoProgram;
void BrainGUIProgram(){

}