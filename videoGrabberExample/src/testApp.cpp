#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	w 		= 320;	// try to grab at this size.
	h 		= 240;
	
    //we can now get back a list of devices. 
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName; 
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl; 
        }
	}
    
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(w,h);

	ofSetVerticalSync(true);
    
    pic.allocate(w, h, OF_IMAGE_COLOR);
    mirror.allocate(w, h, OF_IMAGE_COLOR);
    left.allocate(w, h, OF_IMAGE_COLOR);
    right.allocate(w, h, OF_IMAGE_COLOR);
    
    ofBackground(0);
    ofSetWindowShape(768, 1024);
    state = 0;
    newPic = true;
}


//--------------------------------------------------------------
void testApp::update(){
	
	
	
	vidGrabber.update();
   

}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(255);
    int scale = 4;
    ofPushMatrix();
    ofTranslate(-(vidGrabber.getWidth() * scale)/5, 0);
    ofScale(scale, scale);
    
    switch (state) {
        case 0:
            //show camera
            ofPushMatrix();
            ofScale(-1, 1);
            vidGrabber.draw(-vidGrabber.getWidth(),0);
            ofPopMatrix();
            ofSetColor(200, 0, 0);
            ofSetLineWidth(5);
            ofLine(vidGrabber.width/2, 0, vidGrabber.width/2, ofGetHeight());
            
            break;
        
        case 2:
            //show pic
            pic.draw(0, 0);
            
            break;
            
        case 3:
            mirror.draw(0, 0);
            //show mirror
            break;
            
        case 4:
            //show left sym
            left.draw(0, 0);
            break;
            
        case 5:
            right.draw(0, 0);
            //show right sym
            break;
            
        default:
            break;
    }
    
    ofPopMatrix();
    
    switch (state) {

        case 1:
            ofPushMatrix();
            pic.draw(0, 0);
            mirror.draw(ofGetWidth()/2, 0);
            left.draw(0, ofGetHeight()/2);
            right.draw(ofGetWidth()/2, ofGetHeight()/2);
            ofPopMatrix();
            break;
            
        default:
            break;
    }
    
    
}



//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    saveImg();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == '0') {
        state = 0;
        newPic = true;
    }
    if (key == '1') {
        if (newPic) {
        saveImg();
        newPic = false;
        }
        state = 1;
    }
    if (key == '2') state = 2;
    if (key == '3') state = 3;
    if (key == '4') state = 4;
    if (key == '5') state = 5;
}

//--------------------------------------------------------------
void testApp::saveImg(){
    pic.setFromPixels(vidGrabber.getPixelsRef());
    pic.reloadTexture();
    makeMirror();
    makeLeft();
    makeRight(); 
}

//--------------------------------------------------------------
void testApp::makeMirror() {
    mirror.clone(pic);
    mirror.mirror(false, true);
    mirror.reloadTexture();
}

//--------------------------------------------------------------
void testApp::makeLeft() {
    left.clone(pic);

    ofPixels leftPix = left.getPixelsRef();
    
    for (int y = 0; y < h; y++) {
        for (int x = w/2; x < w; x++) {
            int index = y*(w*3)+x*3;
            int leftIndex = ((w*3-(1*3)) -x*3) + y*(w*3);
            leftPix[leftIndex] = leftPix[index];
            leftPix[leftIndex + 1] = leftPix[index + 1];
            leftPix[leftIndex + 2] = leftPix[index + 2];
        }
    }
    
    left.setFromPixels(leftPix);

}

//--------------------------------------------------------------
void testApp::makeRight() {
    right.clone(pic);
    
    ofPixels rightPix = right.getPixelsRef();
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w/2; x++) {
            int index = y*(w*3)+x*3;
            int leftIndex = ((w*3-(1*3)) -x*3) + y*(w*3);
            rightPix[leftIndex] = rightPix[index];
            rightPix[leftIndex + 1] = rightPix[index + 1];
            rightPix[leftIndex + 2] = rightPix[index + 2];
        }
    }
    
    right.setFromPixels(rightPix);
    
}



