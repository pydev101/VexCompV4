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
    virtual void call(int pressX, int pressY) {};
    virtual bool newLine() { return false; };

    void setVisibility(bool v) {
        positionInfomation.visible = v;
    }
};

class Spacer : public Object {
protected:
    bool isNewLine = false;
public:
    const static int classID = 1;
    Spacer(int width, int height, bool newLine, bool visibility=true) : Object(0, 0, width, height, visibility) {
        isNewLine = newLine;
    }

    bool newLine() {
        return isNewLine;
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
    const static int classID = 2;
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
    const static int classID = 3;
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

typedef struct {
    int classID;
    void* classObj;
}FrameStorageObj;

class Frame : Object {
protected:
    int drawingNum = 0;
    FrameStorageObj* objects = (FrameStorageObj*)std::malloc(0);

    void iDraw(FrameStorageObj z, int screenX, int screenY) {
        if (z.classID == 1) {
            Spacer* x = (Spacer*)(z.classObj);
            x->positionInfomation.x = screenX;
            x->positionInfomation.y = screenY;
            x->draw();
        }else if (z.classID == 2) {
            Label* x = (Label*)(z.classObj);
            x->positionInfomation.x = screenX;
            x->positionInfomation.y = screenY;
            x->draw();
        }else if (z.classID == 3) {
            Button* x = (Button*)(z.classObj);
            x->positionInfomation.x = screenX;
            x->positionInfomation.y = screenY;
            x->draw();
        }else if (z.classID == 4) {
            Frame* x = (Frame*)(z.classObj);
            x->positionInfomation.x = screenX;
            x->positionInfomation.y = screenY;
            x->draw();
        }
    }
    details iInfo(FrameStorageObj z) {
        if (z.classID == 1) {
            Spacer* x = (Spacer*)(z.classObj);
            return x->positionInfomation;
        }
        else if (z.classID == 2) {
            Label* x = (Label*)(z.classObj);
            return x->positionInfomation;
        }
        else if (z.classID == 3) {
            Button* x = (Button*)(z.classObj);
            return x->positionInfomation;
        }else if (z.classID == 4) {
            Frame* x = (Frame*)(z.classObj);
            return x->positionInfomation;
        }
    }
    void iCall(FrameStorageObj z, int pressX, int pressY) {
        if (z.classID == 3) {
            Button* x = (Button*)(z.classObj);
            x->call(pressX, pressY);
        }else if (z.classID == 4) {
            Frame* x = (Frame*)(z.classObj);
            x->call(pressX, pressY);
        }
    }
public:
    const static int classID = 4;
    ~Frame() {
        free(objects);
    }
    void add(Spacer L) {
        FrameStorageObj* temp = (FrameStorageObj*)realloc(objects, sizeof(FrameStorageObj) * drawingNum);
        if (temp != NULL) {
            objects = temp;
            objects[drawingNum - 1] = { L.classID, &L };
        }
    }
    void add(Label L) {
        FrameStorageObj* temp = (FrameStorageObj*)realloc(objects, sizeof(FrameStorageObj) * drawingNum);
        if (temp != NULL) {
            objects = temp;
            objects[drawingNum - 1] = { L.classID, &L };
        }
    }
    void add(Button L) {
        FrameStorageObj* temp = (FrameStorageObj*)realloc(objects, sizeof(FrameStorageObj) * drawingNum);
        if (temp != NULL) {
            objects = temp;
            objects[drawingNum - 1] = { L.classID, &L };
        }
    }
    void call(int screenX, int screenY) {
        for (int i = 0; i < drawingNum; i++) {
            iCall(objects[i], screenX, screenY);
        }
    }

    //TODO set width as calc width and height dependent on drawn
    void draw() {
        if (positionInfomation.visible) {
            int baseX = positionInfomation.x;
            int baseY = positionInfomation.y;
            int currX = 0;
            int currY = 0;
            int yDeltaMax = 0;
            int xMaxWidth = 0;
            //For if visible then add x+y then draw or if spacer shift pos values
            for (int i = 0; i < drawingNum; i++) {
                details d = iInfo(objects[i]);
                if (d.visible) {
                    //Issue if its a frame then its width and height is unknown until drawn
                    if ((d.up + d.down + d.height) > yDeltaMax) {
                        yDeltaMax = d.up + d.down + d.height;
                    }
                    if (objects[i].classID == 1) {
                        //Spacer
                        Spacer* s = (Spacer*)(objects[i].classObj);
                        if (s->newLine()) {
                            currY = currY + yDeltaMax;
                            yDeltaMax = 0;
                            if (currX+d.width+d.right > xMaxWidth) {
                                xMaxWidth = currX + d.width + d.right;
                            }
                            currX = 0;
                        }else {
                            currX = currX + d.width + d.left + d.right;
                        }
                    }else {
                        currX = currX + d.left;
                        iDraw(objects[i], baseX + currX, baseY + currY + d.up);
                        d = iInfo(objects[i]);
                        currX = currX + d.width + d.right;
                    }
                }
            }
            positionInfomation.height = currY + yDeltaMax;
            if (currX > xMaxWidth) {
                xMaxWidth = currX;
            }
            positionInfomation.width = xMaxWidth;
        }
    }
    void call()

};

int main(){
    Label test = Label({ "Hello", 3, "blue" }, 30, true);
    test.draw();


    return 0;
}