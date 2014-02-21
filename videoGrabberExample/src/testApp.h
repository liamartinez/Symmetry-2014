#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		void mousePressed(int x, int y, int button);
        void keyPressed (int key);
		
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
