#include "Intergration.h"

bool programRunning = false;

typedef struct {
  std::string name;
  void (*function)(int);
  int mod;

} autoEntry;

void oneMinute(int mod){
  move(10, 0);
  turnToHead(200);
  move(5, 0);
}

autoEntry entries[] = {
  {"One Minute", oneMinute}
};


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

int indexAuto = 0;
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
}

void autonomous(void){
  //GUIActivate = false;
  //entries[indexAuto].function(entries[indexAuto].mod);
  setIntake(1);
  setDPS(600,600);
  wait(1700, msec);
  setDPS(-400,400);
  wait(1200, msec);

  setDPS(400,400);
  wait(2700, msec);
  setDPS(0,0);

  setBottom(1);
  setTop(1);
  wait(1600, msec);
  setTop(0);
  setBottom(0);
  setIntake(0);

  setDPS(-800,-800);
  wait(500, msec);
  setBottom(-1);
  setIntake(-1);
  wait(500, msec);
  setDPS(0,0);
  wait(500, msec);
  setBottom(0);
  setIntake(0);

  setDPS(400,-400);
  wait(400, msec);
  setDPS(-1000, -1000);
  wait(2000, msec);
  for(int i=-1000; i<-400; i+=20){
    setDPS(i, i);  
    wait(5,msec);
  }
  setDPS(400, -400);
  wait(600, msec);
  setDPS(0,0);
  setDPS(700, 700);
  wait(3015, msec);
  setDPS(0,0);

  setDPS(400,-400);
  wait(450, msec);
  setDPS(400,400);
  setIntake(1);
  setBottom(1);
  wait(1900, msec);

  setDPS(0,0);
  wait(4000, msec);
  setIntake(0);
  setBottom(0);

  setDPS(-600,-600);
  wait(1300, msec);
  setDPS(0,0);

  setDPS(400,-400);
  wait(400, msec);
  
  setDPS(-600,-600);
  wait(2000, msec);
  setDPS(0,0);

  setDPS(400, -400);
  wait(600, msec);
  setDPS(0,0);

  setDPS(600, 600);
  wait(1400, msec);

  for(int i=0; i<4; i++){
    setDPS(-400, -400);
    wait(700, msec);
    setDPS(400, 400);
    wait(700, msec);
  }
  setDPS(0,0);
}

//Brain.SDcard.isInserted();