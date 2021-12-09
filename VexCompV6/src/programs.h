#include "Intergration.h"

//Defines autonomous program and GUI selection screen

bool programRunning = false;

typedef struct {
  std::string name;
  void (*function)(int);
  int mod;

} autoEntry;

void blueLeft(int mod){
  moveTime(80, 900);
  setFront(true);
  setBack(false);
  moveTime(33, 800, true);
  moveTime(-70, 500);
  setBack(true);
  moveTime(33, 480, true);
  moveTime(70, 1300);
  setBack(false);
  moveTime(-33, 660, true);
  moveTime(-50, 500);
  moveArm(100, 500);
  setIntake(-50);
}
void redRight(int mod){
  moveTime(100, 580);
  setFront(true);
  setBack(false);
  moveTime(-33, 680, true);
  moveTime(-70, 520);
  setBack(true);
  wait(200, timeUnits::msec);
  moveTime(33, 1100, true);
  moveTime(-70, 1600);
  setBack(false);
  moveTime(50, 390);
  moveTime(33, 360, true);
  moveTime(-50, 450);
  setBack(true);
  moveArm(60, 800);
  setIntake(50);
  moveTime(70, 720);
}
void OneMinute(int mod){
  setFront(false);
  moveArm(60, 100);
  moveTime(70, 2000);
  moveTime(-30, 360, true);
  moveTime(-70, 2500);
  moveTime(30, 1000);
  moveTime(30, 360, true);
  moveTime(70, 2000);
}

autoEntry entries[] = {
  {"Blue Left", blueLeft},
  {"Blue Right", redRight},
  {"Red Right", redRight},
  {"Red Left", blueLeft},
  {"One Minute", OneMinute},
};
int indexAuto = 0;


//START OF GUI------------------------------------------------------------------------
class ButtonGUI {
  public:
    int x;
    int y;
    int width;
    int height;
    void (*callback)();
    void (*drawing)(int x, int y, int w, int h, bool press);
    int *point;
    bool isPressed = false;

    ButtonGUI(int X, int Y, int W, int H, void (*call)(), void (*drawFunct)(int x, int y, int w, int h, bool press)){
      x = X;
      y = Y;
      width = W;
      height = H;
      callback = call;
      drawing = drawFunct;
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
      drawing(x, y, width, height, isPressed);
    }
};

void increaseIndex(){
  indexAuto += 1;
  if(indexAuto > (sizeof(entries)/sizeof(entries[0]))-1){
    indexAuto = 0;
  }
}

void drawAAA(int x, int y, int w, int h, bool press){
  if(press){
    Brain.Screen.setFillColor(green);
  }else{
    Brain.Screen.setFillColor(red);
  }
  Brain.Screen.drawRectangle(x, y, w, h);
  Brain.Screen.setFillColor(white);
  Brain.Screen.drawLine((w/4)+x, (3*h/4)+y, (w/2)+x, (h/4)+y);
  Brain.Screen.drawLine((3*w/4)+x, (3*h/4)+y, (w/2)+x, (h/4)+y);
}
ButtonGUI increaseBTN(480-50,0,50,70, increaseIndex, drawAAA);

void decreaseIndex(){
  indexAuto -= 1;
  if(indexAuto < 0){
    indexAuto = (sizeof(entries)/sizeof(entries[0]))-1;
  }
}
void drawAAB(int x, int y, int w, int h, bool press){
  if(press){
    Brain.Screen.setFillColor(green);
  }else{
    Brain.Screen.setFillColor(red);
  }
  Brain.Screen.drawRectangle(x, y, w, h);
  Brain.Screen.setFillColor(white);
  Brain.Screen.drawLine((w/4)+x, (h/4)+y, (w/2)+x, (3*h/4)+y);
  Brain.Screen.drawLine((3*w/4)+x, (h/4)+y, (w/2)+x, (3*h/4)+y);
}
ButtonGUI decreaseBTN(480-50,80,50,70, decreaseIndex, drawAAB);

bool GUIActivate = true;
void setAuto(){
  GUIActivate = false;
}
void drawAAC(int x, int y, int w, int h, bool press){
  if(press){
    Brain.Screen.setFillColor(white);
  }else{
    Brain.Screen.setFillColor(red);
  }
  Brain.Screen.drawRectangle(x, y, w, h);
  if(press){
    Brain.Screen.setFillColor(green);
  }else{
    Brain.Screen.setFillColor(white);
  }
  Brain.Screen.drawCircle((w/2)+x, (h/2)+y, (w/4));
}
ButtonGUI setBTN(480-50,160,50,70, setAuto, drawAAC);

ButtonGUI buttons[] = {increaseBTN, decreaseBTN, setBTN};

void pressHandler(){
  if(!GUIActivate){return;}
  static bool free = true;

  if(free){
    free = false;
  }else{
    return;
  }
  
  if(Brain.Screen.pressing()){
    for(int i=(sizeof(buttons)/sizeof(ButtonGUI))-1; i >= 0; i--){
      if(buttons[i].call(Brain.Screen.xPosition(), Brain.Screen.yPosition())){
        buttons[i].draw();
        waitUntil(!Brain.Screen.pressing());
        buttons[i].isPressed = false;
        free = true;
        return;
      }
    }
  }
}

void BrainGUIProgram(){
  Brain.Screen.pressed(pressHandler);

  while(GUIActivate){
    Brain.Screen.setFillColor(black);
    Brain.Screen.setPenColor(white);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);

    std::string out = entries[indexAuto].name;
    Brain.Screen.print(out.c_str());

    for(int i=0; i < (sizeof(buttons)/sizeof(ButtonGUI)); i++){
      buttons[i].draw();
    }
    Brain.Screen.render();
    wait(50, msec);
  }

  Brain.Screen.setFillColor(black);
  Brain.Screen.setPenColor(white);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  std::string out = entries[indexAuto].name;
  Brain.Screen.print("Selected: %s", out.c_str());
  Brain.Screen.render();
}

void autonomous(void){
  GUIActivate = false;
  entries[indexAuto].function(entries[indexAuto].mod);
}

//Brain.SDcard.isInserted();