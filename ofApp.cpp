#include "ofApp.h"
#include "Manip.h"


//--------------------------------------------------------------
void ofApp::setup(){

	manip = new Manip();
	manip->x_pressed = false;
	manip->y_pressed = false;

	ofSetVerticalSync(true);
//	ofEnableDepthTest();
	ofEnableAlphaBlending();

	//
	// set when <control> key is held down
	//
	ctrlKeyDown = false;
	shiftKeyDown = false;
	selectedImage = -1;      // selection empty;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	ofFill();

	for (int i = 0; i < images.size(); i++ ) {
		images[i].draw();
	}
	manip->draw(ManipDrawNormal);
}


void ofApp::exit() {
	delete manip;
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		break;

	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		break;
	case 's':
		savePicture();
		break;
	case 'm':
	case 'M':
		multiplyImage();
		break;
	case 'u':
		undoTransformations();
		break;
	case 'x':
	case 'X':
		if (imageSelected()) {
			manip->x_pressed = true;
			int image_index = selectedImage;
			images[image_index].rot_axis = 'x';
		}
		break;
	case 'y':
	case 'Y':
		if (imageSelected()) {
			manip->y_pressed = true;
			int image_index = selectedImage;
			images[image_index].rot_axis = 'y';
		}
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		ctrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		shiftKeyDown = true;
		break;
	case OF_KEY_DEL:
		doDelete();
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case 'x':
	case 'X':
		manip->x_pressed = false;
		if (imageSelected()) {
			images[selectedImage].rot_axis = 'z';
		}
		break;
	case 'y':
	case 'Y':
		manip->y_pressed = false;
		if (imageSelected()) {
			images[selectedImage].rot_axis = 'z';
		}
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		ctrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		shiftKeyDown = false;
		break;
	case OF_KEY_UP:
		if (selectedImage > -1 && selectedImage < (images.size() - 1)) {
			Image temp = images[selectedImage];
			images[selectedImage] = images[selectedImage + 1];
			images[selectedImage + 1] = temp;
			selectedImage++;
			manip->connect(&images[selectedImage]);
		}
		break;
	case OF_KEY_DOWN:
		if (selectedImage > 0) {
			Image temp = images[selectedImage];
			images[selectedImage] = images[selectedImage - 1];
			images[selectedImage - 1] = temp;
			selectedImage--;
			manip->connect(&images[selectedImage]);
		}
		break;
	}
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
//
// If the manipulator is active, send mouse events to it to process
//
void ofApp::mouseDragged(int x, int y, int button) {

	if (manip->active)
		manip->processMouse(x, y, button);
}

//---------------------------------------------------------
//
// delete the selected image
//
void ofApp::doDelete() {
	if (imageSelected())
	{
		manip->disconnect();
		images.erase(images.begin() + selectedImage);
		selectedImage = -1;
	}
}

//  Save the window to an image file on disk
//
void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
}



void ofApp::multiplyImage() {
	if (imageSelected()) {
		int i = selectedImage;
		Image duplicateImage;
		manip->disconnect();
		duplicateImage.trans = images[i].trans + ofVec2f(15, 15);
		duplicateImage.image_center = images[i].image_center + ofVec2f(15, 15);
		duplicateImage.scale = images[i].scale;
		duplicateImage.rot_axis = 'z';
		images.push_back(duplicateImage);
		selectedImage = images.size() - 1;
		manip->connect(&images[selectedImage]);
	}
}
//   Undo transformations. Set back to a reasonable default 
// 
void ofApp::undoTransformations() {
	if (imageSelected()) {
		int i = selectedImage;
		images[i].scale = ofVec2f(1, 1);
		images[i].trans = ofVec2f(0, 0);
		images[i].rot = 0;
	}
}

//--------------------------------------------------------------
//
//  Mouse is pressed down, test to see what objects are selected
//
void ofApp::mousePressed(int x, int y, int button){
	renderSelection();
	processSelection(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	manip->setActive(false);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
//
// Simple file drag and drop implementation.  Drop the image
// under the 
//
void ofApp::dragEvent(ofDragInfo dragInfo){ 

	Image imageObj;

	if (imageObj.image.load(dragInfo.files[0]) == true)
	{
		// disconnect manipulator (if connected)
		//
		manip->disconnect();

		// translate image to drop location
		//
		imageObj.trans = ofVec3f(dragInfo.position.x, dragInfo.position.y);

		// save image on stack, select it and connect manipulator
		//
		imageObj.bSelected = true;
		imageObj.image_center = ofVec2f(mouseX, mouseY);
		imageObj.rot_axis = 'z';
		images.push_back(imageObj);
		selectedImage = images.size() - 1;
		manip->connect(&images[selectedImage]);
		manip->x_pressed = false;
		manip->y_pressed = false;
		cout << "Rotation axis " << imageObj.rot_axis;
	}
	else {
		cout << "Can't load image: " << dragInfo.files[0] << endl;
	}
}

//
// Render for the purposes of selection hit testing.  In this case
// we use the color method. We render echo object as a different
// value of r,g,b.  We then compare the pixel under the mouse. The
// value is the index into the image list;
//
void ofApp::renderSelection() {

	ofBackground(0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	// check for any images selected; the color index
	// we use is simple the index of the image in the vector
	//
	for (int i = 0; i < images.size(); i++) {
		images[i].draw(true, i);
	}

	//
	// check for manipulator handles selected
	//
	manip->draw(ManipDrawTestSelect);



	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

void ofApp::processSelection(int x, int y) {
	unsigned char res[4];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	// check if manipulator to see which part of it is selected
	// and set state.
	//
	if (manip->isAHandle(res[1])) {
		manip->setActive(true);
		manip->setHandle(res[1]);
		manip->setMouse(x, y);
		cout << "handle: " << res[1] << endl;
		return;
	}
	else if (manip->isATranslateBox(res[1])) {
		manip->setActive(true);
		manip->setMouse(x, y);
		return;
	}

	// Manipulator not selected, disconnect
	//
	if (imageSelected()) {
		images[selectedImage].bSelected = false;
		selectedImage = -1;
		manip->disconnect();
	}
	
	if (res[0] > 0 && res[0] <= images.size()) {
		int id = res[0] - 1;
		Image img = images[id];
		img.bSelected = true;

		// move selected image to end of list
		//
		images.erase(images.begin() + id );
		images.push_back(img);
		selectedImage = images.size() - 1;

		// connect manipulator
		//
		manip->connect(&images[selectedImage]);
	}
}