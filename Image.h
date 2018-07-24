//--------------------------------------------------------------
//
//  CS235 - User Interface Design - Kevin M. Smith
//
//  Class wrapper for an image
// 
//  Simple class wrapper to store an ofImage with
//  tranformation attributes (trans, rot and scale)
//
//  This example is a translate/scale manipulator
//
//

#pragma once
#include "ofMain.h"

class BaseObject {
public:
	ofVec2f image_center;
	ofVec2f trans, scale;
	float	rot;
	char rot_axis;
	bool	bSelected;
};

class Image : public BaseObject {

public:
	Image();
	ofImage image;
	void draw(bool bSelectMode = false, int index = 0);
};



