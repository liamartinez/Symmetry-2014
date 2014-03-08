#include "textureApp.h"

//--------------------------------------------------------------
void textureApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetVerticalSync(false);
	
	doDrawInfo	= true;
		
	consoleListener.setup(this);
	
	w = 768;
	h = 1024; 


	omxCameraSettings.width = w;
	omxCameraSettings.height = h;
	//omxCameraSettings.framerate = 25;
	omxCameraSettings.isUsingTexture = true;
	//omxCameraSettings.doRecording = true;
	
	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();

    title.loadImage("title.jpg"); 

	fbo.allocate(w,h, GL_RGB);

	pic.allocate(w, h, OF_IMAGE_COLOR);
    mirror.allocate(w, h, OF_IMAGE_COLOR);
    left.allocate(w, h, OF_IMAGE_COLOR);
    left2.allocate(w, h, OF_IMAGE_COLOR);
    right.allocate(w, h, OF_IMAGE_COLOR);

    state = 0;
    newPic = true;


    waitTime = 5000; 
    displayTime = 3000; 
    startTime = ofGetElapsedTimeMillis(); 

    firstSession = true; 
    newSession = true; 

    rotateOn = false; 

}

//--------------------------------------------------------------
void textureApp::update()
{
	/*if (videoGrabber.isFrameNew())
	{
		
	}*/

}


//--------------------------------------------------------------
void textureApp::draw(){


	int drawWidth = w;
	int drawHeight = h;

	ofSetColor(255);

	switch (state) {
		case 0:
        	//fbo
            fbo.begin();
            videoGrabber.getTextureReference().draw(0,0);
            fbo.end();

            //show camera flipped
            ofPushMatrix();
            ofScale(-1, 1);
            ofSetColor(255); 
            fbo.draw(-drawWidth, 0, drawWidth, drawHeight);
            ofPopMatrix();
            ofSetColor(200, 0, 0);
            ofSetLineWidth(5);
            ofLine(fbo.getWidth()/2, 0, fbo.getWidth()/2, fbo.getHeight()); 
		break; 

		case 1: 
            if (newPic) {
            saveImg();
            if (firstSession) firstSession = false; 
            newPic = false;
            }

            pic.draw(0, 0, w/2, h/2);
            right.draw(drawWidth/2, 0, w/2, h/2);
            left.draw(0, drawHeight/2, w/2, h/2);
            left2.draw(drawWidth/2, drawHeight/2, w/2, h/2);
	}	

}

//--------------------------------------------------------------
void textureApp::saveImg(){
    //pic.setFromPixels(vidGrabber.getPixelsRef());
 //    	fbo.begin(); 
	// videoGrabber.draw();
	// fbo.end(); 
	fbo.readToPixels(pix); 
	pic.setFromPixels(pix); 
    pic.reloadTexture();
    makeRight(); 
    makeLeft();
    makeLeft2();

}

//--------------------------------------------------------------
void textureApp::makeRight() {
    right.clone(pic);

    ofPixels leftPix = right.getPixelsRef();
    
    for (int y = 0; y < h; y++) {
        for (int x = w/2; x < w; x++) {
            int index = y*(w*3)+x*3;
            int leftIndex = ((w*3-(1*3)) -x*3) + y*(w*3);
            leftPix[leftIndex] = leftPix[index];
            leftPix[leftIndex + 1] = leftPix[index + 1];
            leftPix[leftIndex + 2] = leftPix[index + 2];
        }
    }
    
    right.setFromPixels(leftPix);
    
    }

    //--------------------------------------------------------------
void textureApp::makeLeft() {
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
void textureApp::makeLeft2() {
    left2.clone(pic);

    ofPixels left2Pix = left2.getPixelsRef();
    
    for (int y = 0; y < h; y++) {
        for (int x = w/2; x < w; x++) {
            int index2 = y*(w*3)+x*3;
            int left2Index = ((w*3-(1*3)) -x*3) + y*(w*3);
            left2Pix[left2Index] = left2Pix[index2];
            left2Pix[left2Index + 1] = left2Pix[index2 + 1];
            left2Pix[left2Index + 2] = left2Pix[index2 + 2];
        }
    }
    
    left2.setFromPixels(left2Pix);

}

//--------------------------------------------------------------
void textureApp::keyPressed  (int key)
{

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


	ofLogVerbose(__func__) << key;
	
	
	if (key == 'e')
	{
		videoGrabber.applyImageFilter(filterCollection.getNextFilter());
	}
	
	if (key == 'g')
	{
		doDrawInfo = !doDrawInfo;
	}
	if (key == 'c')
	{
		//controlPanel.increaseContrast();
	}
	if (key == 'q')
	{
		ofLogVerbose(__func__) << "SENDING QUIT";
		//videoGrabber.engine->want_quit = true;
		videoGrabber.stopRecording();
	}
}

void textureApp::onCharacterReceived(SSHKeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

