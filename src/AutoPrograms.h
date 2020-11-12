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

class ButtonGUI {
  public:
    int x;
    int y;
    int width;
    int height;
    void (*callback)();
    int *point;

    ButtonGUI(int X, int Y, int W, int H, void (*call)()){
      x = X;
      y = Y;
      width = W;
      height = H;
      callback = call;
    }

    bool call(int X, int Y){
      if((X >= x) && (X <= (x+width))){
        if((Y >= y) && (Y <= (y+height))){
          callback();
          return true;
        }
      }
      return false;
    }

    void draw(){
      Brain.Screen.drawRectangle(x, y, width, height);
    }
};

int indexAuto = 0;
void changeIndex(){
  indexAuto += 1;
  std::cout << indexAuto << std::endl;
}

autoEntry selectedAutoProgram = entries[0];
void BrainGUIProgram(){
  ButtonGUI test(10,10,50,70, changeIndex);

  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);


    
    wait(1000, msec);
  }
}