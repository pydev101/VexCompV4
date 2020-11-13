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


//START OF GUI------------------------------------------------------------------------
class ButtonGUI {
  public:
    int x;
    int y;
    int width;
    int height;
    void (*callback)();
    int *point;
    bool isPressed = false;

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
          isPressed = true;
          return true;
        }
      }
      return false;
    }

    void draw(){
      if(isPressed){
        Brain.Screen.setFillColor(green);
      }else{
        Brain.Screen.setFillColor(red);
      }
      Brain.Screen.drawRectangle(x, y, width, height);
    }
};

int indexAuto = 0;
void changeIndex(){
  indexAuto += 1;
  std::cout << indexAuto << std::endl;
}

ButtonGUI test(10,10,50,70, changeIndex);
ButtonGUI zeta(10,10,50,70, changeIndex);
ButtonGUI buttons[] = {test};

void pressHandler(){
  static bool free = true;

  if(free){
    free = false;
  }else{
    return;
  }
  
  if(Brain.Screen.pressing()){
    for(int i=(sizeof(buttons)/sizeof(ButtonGUI))-1; i >= 0; i--){
      if(buttons[i].call(Brain.Screen.xPosition(), Brain.Screen.yPosition())){
        waitUntil(!Brain.Screen.pressing());
        buttons[i].isPressed = false;
        free = true;
        return;
      }
    }
  }
}

autoEntry selectedAutoProgram = entries[0];
void BrainGUIProgram(){
  Brain.Screen.pressed(pressHandler);

  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);

    for(int i=0; i < (sizeof(buttons)/sizeof(ButtonGUI)); i++){
      buttons[i].x += 5;
      if(buttons[i].x > 240){buttons[i].x=0;}
      buttons[i].draw();
    }

    wait(200, msec);
  }
}