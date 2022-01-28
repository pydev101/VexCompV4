#include <stdlib.h>
#include <sstream>
#include <iostream>
using namespace std;


typedef struct {
    int x, y, width, height, left, right, up, down;
    bool visible;
} details;

typedef struct {
    std::string value;
    int penwidth;
    //fontType font;
    std::string color;
} TextInfo;
int getTextInfoWidth(TextInfo text) {
    //Brain.Screen.setFont(font);
    //Brain.Screen.setPenWidth(penwidth); 
    //return Brain.Screen.getStringWidth(text.value.c_str());
    return 10;
}
int getTextInfoHeight(TextInfo text) {
    //Brain.Screen.setFont(font);
    //Brain.Screen.setPenWidth(penwidth);
    //return Brain.Screen.getStringHeight(text.value.c_str());
    return 10;
}

class Object {
public:
    details positionInfomation;

    Object(int x, int y, int width, int height, bool visible) {
        positionInfomation = {x, y, width, height, 0, 0, 0, 0, visible};
    }
    Object(int x, int y, int width, int height, bool visible, int leftSpacing, int rightSpacing, int upSpacing, int downSpacing) {
        positionInfomation = {x, y, width, height, leftSpacing, rightSpacing, upSpacing, downSpacing, visible };
    }

    virtual void draw() {};

    details* getPosInfo() {
        return &positionInfomation;
    }

    void setVisibility(bool v) {
        positionInfomation.visible = v;
    }
};

class Label : public Object {
protected:
    TextInfo text;

    static int widthCalculator(int width, TextInfo textInfo) {
        int widthText = getTextInfoWidth(textInfo);

        if (width < widthText) {
            return widthText;
        }
        return width;
    }
public:
    Label(TextInfo textinfo, int width, bool visiblity) : Object(0, 0, widthCalculator(width, textinfo), getTextInfoHeight(textinfo), visiblity) {
        text = textinfo;
        setVisibility(visiblity);
    }
    Label(TextInfo textinfo, int width, bool visiblity, int leftSpacing, int rightSpacing, int upSpacing, int downSpacing) : Object(0, 0, widthCalculator(width, textinfo), getTextInfoHeight(textinfo), visiblity, leftSpacing, rightSpacing, upSpacing, downSpacing) {
        text = textinfo;
        setVisibility(visiblity);
    }
    std::string getText() { return text.value; };
    void setText(TextInfo textvalue) {
        text = textvalue;
        positionInfomation.width = widthCalculator(positionInfomation.width, text);
        positionInfomation.height = getTextInfoHeight(text);
    };
    void draw() {
        if (positionInfomation.visible) {
            //Brain.Screen.setFont(text.font);
            //Brain.Screen.setPenWidth(text.penwidth); 
            //Brain.Screen.setPenColor(text.color.c_str());
            //Brain.Screen.printAt(positionInfomation.x, positionInfomation.y, text.value.c_str());
            std::cout << "DRAWING " << text.value << std::endl;
        }
    }
};

class Button : public Object {
protected:
    bool active = true;
    bool pressing = false;
    void (*drawing)(Button* self);
    void (*callback)(Button* self);

public:
    Button(int width, int height, bool visiblity, bool active, void (*drawingFunction)(Button* self), void (*callbackFunction)(Button* self)) : Object(0, 0, width, height, visiblity) {
        active = active;
        drawing = drawingFunction;
        callback = callbackFunction;
    }
    Button(int width, int height, bool visiblity, bool active, void (*drawingFunction)(Button* self), void (*callbackFunction)(Button* self), int leftSpacing, int rightSpacing, int upSpacing, int downSpacing) : Object(0, 0, width, height, visiblity, leftSpacing, rightSpacing, upSpacing, downSpacing) {
        active = active;
        drawing = drawingFunction;
        callback = callbackFunction;
    }

    void setActive(bool v) {
        active = v;
    }
    bool getActive() {
        return active;
    }
    bool isPressing() {
        return pressing;
    }
    void forcecall() {
        callback(this);
    }
    void call(int pressX, int pressY) {
        if (active) {
            if ((positionInfomation.x <= pressX) && (pressX <= (positionInfomation.x+positionInfomation.width))) {
                if ((positionInfomation.y <= pressY) && (pressY <= (positionInfomation.y + positionInfomation.height))) {
                    if (!pressing) {
                        pressing = true;
                        callback(this);
                    }
                } else {
                    pressing = false;
                }
            } else {
                pressing = false;
            }
        } else {
            pressing = false;
        }
    }
    void draw() {
        if (positionInfomation.visible) {
            drawing(this);
        }
    }
};

class Frame : Object {
protected:
    int drawingNum = 0;
    details* drawingInfo = (details*)std::malloc(0);
    void** objects = (void**)std::malloc(0);
public:
    ~Frame() {
        free(drawingInfo);
        free(objects);
    }
    void add(Object L) {
        drawingNum++;
        drawingInfo = (details*)realloc(drawingInfo, sizeof(details) * drawingNum);
        objects = (void**)realloc(objects, sizeof(void*) * drawingNum);

        drawingInfo[drawingNum - 1] = L.positionInfomation;
        objects[drawingNum - 1] = &L;
    }
    void add(Label L) {
        drawingNum++;
        drawingInfo = (details*)realloc(drawingInfo, sizeof(details) * drawingNum);
        objects = (void**)realloc(objects, sizeof(void*) * drawingNum);

        drawingInfo[drawingNum - 1] = L.positionInfomation;
        objects[drawingNum - 1] = &L;
    }
    void add(Button L) {
        drawingNum++;
        drawingInfo = (details*)realloc(drawingInfo, sizeof(details) * drawingNum);
        objects = (void**)realloc(objects, sizeof(void*) * drawingNum);

        drawingInfo[drawingNum - 1] = L.positionInfomation;
        objects[drawingNum - 1] = &L;
    }
};

int main(){
    Label test = Label({ "Hello", 3, "blue" }, 30, true);
    test.draw();

    return 0;
}