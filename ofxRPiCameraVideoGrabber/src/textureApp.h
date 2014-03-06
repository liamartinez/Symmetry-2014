#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "ConsoleListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"

#include "wiringPi.h"

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
        void displayTitle(); 
        void cyclePics(); 
    
		ofVideoGrabber 		vidGrabber;
		int w;
		int h;
    
        ofImage pic, mirror, left, right;
        int  state; 
        bool switchState, oldSwitchState; 
        bool goSwitch; 
        int switchPin;  
        bool newPic; 

        //first time
        bool firstSession; //of the day - for making sure we don't cycle blank images
        bool newSession; //new person - for making sure we always start a cycle with the title 

        //clock 
        int waitTime, displayTime; 
        int startTime, startCycleTime; 
        bool isCycling;  //control some variables 
        bool goCycle;  //start the cycling

        //title
        ofImage title; 
};

