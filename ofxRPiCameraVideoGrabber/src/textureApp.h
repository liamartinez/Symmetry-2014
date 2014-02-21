#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "ConsoleListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"

class textureApp : public ofBaseApp, public SSHKeyListener{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

	void onCharacterReceived(SSHKeyListenerEventData& e);
	ConsoleListener consoleListener;
	ofxRPiCameraVideoGrabber videoGrabber;
	
	ImageFilterCollection filterCollection;
	
	OMXCameraSettings omxCameraSettings;
	bool doDrawInfo;

	ofPixels pix; 
	ofFbo fbo; 

	        void saveImg();
        void makeMirror();
        void makeLeft();
        void makeRight(); 
    
		ofVideoGrabber 		vidGrabber;
		int w;
		int h;
    
        ofImage pic, mirror, left, right;
        int  state;
        bool newPic; 
};

