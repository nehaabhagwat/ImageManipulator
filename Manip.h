#pragma once
#include "Image.h"

#define HANDLE_SIZE 20

#define MANIP_RECT_ID 99
#define HANDLE_ID_UL 100
#define HANDLE_ID_UR 101
#define HANDLE_ID_LR 102
#define HANDLE_ID_LL 103
#define HANDLE_ID_UC 104
#define HANDLE_ID_LC 105
#define HANDLE_ID_BC 106
#define HANDLE_ID_RC 107
#define HANDLE_ID_ROT 108

typedef enum { ManipDrawNormal, ManipDrawActive, ManipDrawTestSelect } ManipDrawMode;

class Manip : public BaseObject {

public:
	Manip();
	void draw(ManipDrawMode mode);
	void scale(int x, int y);
	void rotate(int x, int y);
	void translate(int x, int y);
	void connect(Image *obj) { this->obj = obj; }
	void disconnect() { this->obj = NULL; }
	void setMouse(int x, int y) { this->mouseLast = ofVec2f(x, y); }
	bool isAHandle(int n);
	bool isATranslateBox(int n);
	void setHandle(int n) { handleIDSelected = n; handleSelected = true; }
	void setActive(bool);
	void processMouse(int x, int y, int button);
	void rotateAtAxes(int x, int y);
	bool active;
	bool handleSelected;
	bool x_pressed;
	bool y_pressed;
	

private:
	Image *obj;   // object the  manipulator is connected to
	int handleSize;
	ofVec2f mouseLast;
	int handleIDSelected;
};

