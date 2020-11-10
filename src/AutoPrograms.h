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
  void (*function)(int);
  int mod;

} autoEntry;

void leftBlue(int mod){

}

void leftRed(int mod){
  
}

void rightBlue(int mod){
  
}

void rightRed(int mod){
  intake(1);
  move(10, 0);
  move(12, 54);
  turnTo(90);
  move(60, 0, 500.0);
  intake(0);
}

autoEntry entries[] = {
  {"Right Red", rightRed},
  {"Left Red", leftRed},
  {"Right Blue", rightBlue},
  {"Left Blue", leftBlue}
};

autoEntry selectedAutoProgram = entries[0];
void BrainGUIProgram(){
  int index = 0;

}