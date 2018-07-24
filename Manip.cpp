
#include "Manip.h"

Manip::Manip() {
	obj = NULL;
	active = false;
	handleSize = HANDLE_SIZE;
	handleSelected = false;
}

void Manip::draw(ManipDrawMode mode) {
	if (obj == NULL) return;

	//
	// manipulator is drawn in world space for this demo, but probably better to draw in
	// screen space (for 3D) to avoid scaling of handles when camera zooms in/out...KMS
	//
	ofPushMatrix();
	ofTranslate(obj->trans);
	if (x_pressed)
		ofRotateX(obj->rot);
	else if (y_pressed)
		ofRotateY(obj->rot);
	else
		ofRotateZ(obj->rot);
	ofScale(obj->scale);

	ofNoFill();


	// set colour depending on selection mode
	//
	switch (mode) {
	case ManipDrawNormal:
		ofSetColor(255, 0, 0);
		break;
	case ManipDrawActive:
		ofSetColor(255, 255, 255);
		break;
	}

	// active override
	//
	if (active) ofSetColor(255, 255, 255);

	ofSetLineWidth(2);

	float s = handleSize;
	float w = obj->image.getWidth();
	float h = obj->image.getHeight();

	// draw frame
	//
	if (mode == ManipDrawTestSelect) {
		ofFill();
		ofSetColor(0, MANIP_RECT_ID, 0);
		ofDrawRectangle((-w / 2), (-h / 2), w, h);
	}
	else {
		ofNoFill();
		ofDrawRectangle((-w / 2), (-h / 2), w, h);
	}


	// draw handles
	//
	if (mode == ManipDrawTestSelect) {
		ofFill();
		ofSetColor(0, HANDLE_ID_UL, 0);
		ofDrawRectangle((-w / 2) - s / 2, (-h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_LL, 0);
		ofDrawRectangle((-w / 2) - s / 2, (h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_LR, 0);
		ofDrawRectangle((w / 2) - s / 2, (h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_UR, 0);
		ofDrawRectangle((w / 2) - s / 2, (-h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_UC, 0);
		ofDrawRectangle(- s / 2, (-h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_BC, 0);
		ofDrawRectangle(- s / 2, (h / 2) - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_LC, 0);
		ofDrawRectangle((-w / 2) - s / 2, - s / 2, s, s);
		ofSetColor(0, HANDLE_ID_RC, 0);
		ofDrawRectangle((w / 2) - s / 2, -s / 2, s, s);
		ofSetColor(0, HANDLE_ID_ROT, 0);
		ofDrawCircle(0, (- h / 2) - s, s/2);
		
	}
	else {
		ofDrawRectangle((-w / 2) - s / 2, (-h / 2) - s / 2, s, s);
		ofDrawRectangle((-w / 2) - s / 2, (h / 2) - s / 2, s, s);
		ofDrawRectangle((w / 2) - s / 2, (h / 2) - s / 2, s, s);
		ofDrawRectangle((w / 2) - s / 2, (-h / 2) - s / 2, s, s);
		ofDrawRectangle(-s / 2, (-h / 2) - s / 2, s, s);
		ofDrawRectangle(-s / 2, (h / 2) - s / 2, s, s);
		ofDrawRectangle((-w / 2) - s / 2, - s / 2, s, s);
		ofDrawRectangle((w / 2) - s / 2, - s / 2, s, s);
		ofDrawCircle(0, (-h / 2) - s, s/2);
	}
	ofPopMatrix();
}

//  Process mouse events in the manipulator. In this case, the manipulator
//  is a scale/translate event.
//
void Manip::processMouse(int x, int y, int button) {
	if (handleSelected) {
		if (handleIDSelected == HANDLE_ID_ROT)
			rotate(x, y);
		scale(x, y);
	}
	else
		translate(x, y);
}

void Manip::scale(int x, int y) {


	if (obj == NULL) return;  // guard
	ofVec2f mouse = ofVec2f(x, y);
	ofVec2f delta = mouse - mouseLast;
	mouseLast = mouse;

	switch (handleIDSelected) {
	case HANDLE_ID_LL:
	case HANDLE_ID_UL:
		obj->scale = obj->scale + -delta.x / (obj->image.getWidth() / 2);
		break;
	case HANDLE_ID_LR:
	case HANDLE_ID_UR:
		obj->scale = obj->scale + delta.x / (obj->image.getWidth() / 2);
		break;
	case HANDLE_ID_UC:
		obj->scale.y = obj->scale.y + -delta.y / 80;
		break;
	case HANDLE_ID_LC:
		obj->scale.x = obj->scale.x + -delta.x / 80;
		break;
	case HANDLE_ID_BC:
		obj->scale.y = obj->scale.y + delta.y / 80;
		break;
	case HANDLE_ID_RC:
		obj->scale.x = obj->scale.x + delta.x / 80;
		break;
	default:
		break;
	}

}

void Manip::rotate(int x, int y) {
	ofVec2f mouse_curr = ofVec2f(x, y);
	
	ofVec3f v1 = ofVec3f(mouseLast - obj->image_center);
	ofVec3f v2 = ofVec3f(mouse_curr - obj->image_center);
	float angle = (v2).angle(v1);
	ofVec3f normalized = (v2).cross(v1).getNormalized();
	if (normalized.z <= 0)
		angle = -angle;


	switch (handleIDSelected) {
	case(HANDLE_ID_ROT):
		if (x_pressed || y_pressed)
			obj->rot += 180;
		obj->rot = obj->rot - angle;
		break;
	default:
		break;
	}
	
}



void Manip::translate(int x, int y) {

	if (obj == NULL) return;   // check that we are connected to something

	ofVec2f mouse = ofVec2f(x, y);
	ofVec2f delta = mouse - mouseLast;

	// store value of where the mouse was last for next entry
	// 
	mouseLast = mouse;

	// apply translation to image (will be translated to this value on next redraw)
	//
	obj->trans += delta;
	obj->image_center = obj->image_center + delta;
}

void Manip::setActive(bool state) {
	active = state;
	if (!active) handleSelected = false;
}

bool Manip::isAHandle(int n) {
	return (n == HANDLE_ID_UL || n == HANDLE_ID_UR ||
		n == HANDLE_ID_LL || n == HANDLE_ID_LR ||
		n == HANDLE_ID_UC || n == HANDLE_ID_LC ||
		n == HANDLE_ID_BC || n == HANDLE_ID_RC ||
		n == HANDLE_ID_ROT);
}

bool Manip::isATranslateBox(int n) {
	return (n == MANIP_RECT_ID);
}