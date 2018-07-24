#include "Image.h"

Image::Image()
{
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rot = 0;              // degrees 
	bSelected = false;    // not selected 
	rot_axis = 'z';
}

// draw the image.  If testing for selection, using color selection
// just draw a solid rectangle using "index" as a color code for the red 
// channel (r, g, b)
//
//
void Image::draw(bool bSelectMode, int index) {
	ofPushMatrix();
	ofTranslate(trans);
	// cout << "Rotation axis: " << rot_axis;
	if (rot_axis == 'x')
		ofRotateX(rot);
	else if (rot_axis == 'y')
		ofRotateY(rot);
	else
		ofRotateZ(rot);
	ofScale(scale);

	if (!bSelectMode) {
		ofSetColor(255, 255, 255, 255);
		image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
	}
	else {
		ofFill();
		ofSetColor(index + 1, 0, 0);
		ofDrawRectangle(-image.getWidth() / 2.0, -image.getHeight() / 2.0,
			image.getWidth(), image.getHeight());
	}
	ofPopMatrix();
}


