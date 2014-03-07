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
	omxCameraSettings.doRecording = false;
	
	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();

	fbo.allocate(w,h, GL_RGB);

	pic.allocate(w, h, OF_IMAGE_COLOR);
    mirror.allocate(w, h, OF_IMAGE_COLOR);
    left.allocate(w, h, OF_IMAGE_COLOR);
    right.allocate(w, h, OF_IMAGE_COLOR);
    
    ofBackground(0);
    ofSetWindowShape(h, w);
    state = 0;
    newPic = true;

    switchPin = 5; 
    oldSwitchState = 0;
    switchState = 0 ;  

    activePin = -1; 

    if(wiringPiSetup() == -1){
        printf("Error on wiringPi setup\n");
        }

    //pinMode(switchPin,INPUT);
    //pinMode (6, OUTPUT); 

    /*
    Pins 0 through 6 (BCM_GPIO 17, 18,  21, 22, 23, 24, 25) 
    */

    //populate from XML
    switches.push_back (5);
    switches.push_back (0);
    switches.push_back (1);
    switches.push_back (3);
    switches.push_back (4);
    switches.push_back (6);
    newPin = false; 

    for (int i = 0; i < switches.size(); i++) {
        pinMode (switches[i], INPUT); 
    }

    title.loadImage("title.jpg"); 

    waitTime = 5000; 
    displayTime = 3000; 
    startTime = ofGetElapsedTimeMillis(); 

    firstSession = true; 
    newSession = true; 

}

//--------------------------------------------------------------
void textureApp::update()
{
	/*if (videoGrabber.isFrameNew())
	{
		
	}*/


    for (int i = 0; i < switches.size(); i++) {
        if (digitalRead(switches[i]) == 1 && i != activePin) {
            newPin = true; 
            activePin = i; 
            cout << "         ACTIVE PIN!!!        " << activePin << endl; 
        }
    }

    if (newPin) {
        if (firstSession) {
            if (activePin == 1) state = 1; 
            else state = 0; 
        } else { 
        state = activePin;
        }   
        startTime = ofGetElapsedTimeMillis();
        goCycle = false; //stop cycling when a switch is touched
        newPin = false;   
       
    }

    /*
    switchState = digitalRead(switchPin);

    if (switchState == 1 && oldSwitchState == 0) {
        goSwitch = true; 
        goCycle = false; //stop cycling when a switch is touched
        newSession = true; 
    }

    oldSwitchState = switchState; 
    */

    if (goSwitch) {
        if (state < 6) state ++; 
        else state = 1;       
        goSwitch = false;
    } 

    //if inactive after a certain amount of time, cycle    
    if (ofGetElapsedTimeMillis() - startTime > waitTime) {
        newSession = true; 
        if (firstSession) {
            state = 6; 
        } else { //only do this if the title has already been displayed and its not the first session of the day
            goCycle = true; 
        }
    }

    if (goCycle) { 
        activePin = -1; 
        if (newSession) { //start by displaying the title
            state = 6; 
            startCycleTime = ofGetElapsedTimeMillis(); 
            newSession = false; 
        } else { //then just cycle as necessary
            if (ofGetElapsedTimeMillis() - startCycleTime > displayTime) {
                goSwitch = true; 
                startCycleTime = ofGetElapsedTimeMillis(); 
            }
        }
    }
}


//--------------------------------------------------------------
void textureApp::draw(){
	
	int drawWidth = w;
	int drawHeight = h;
	

ofPushMatrix(); 
ofTranslate(0, ofGetHeight()); 
ofRotateZ(-90); 

//videoGrabber.getTextureReference().draw(0, 0, drawWidth, drawHeight);

ofSetColor(255);
    int scale = 4;
    ofPushMatrix();
    //ofTranslate(-(vidGrabber.getWidth() * scale)/5, 0);
    //ofScale(scale, scale);
    
    switch (state) {
        case 0:
        	//fbo
            fbo.begin();
            videoGrabber.getTextureReference().draw(0,0);
            fbo.end();

            //show camera
            ofPushMatrix();
            ofScale(-1, 1);
            ofSetColor(255); 
            fbo.draw(-drawWidth, 0, drawWidth, drawHeight);
            ofPopMatrix();
            ofSetColor(200, 0, 0);
            ofSetLineWidth(5);
            ofLine(fbo.getWidth()/2, 0, fbo.getWidth()/2, ofGetHeight());

            newPic = true;
            
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

            if (newPic) {
            saveImg();
            if (firstSession) firstSession = false; 
            newPic = false;
            }
            
            ofPushMatrix();
            pic.draw(0, 0, w/2, h/2);
            mirror.draw(ofGetWidth()/2, 0, w/2, h/2);
            left.draw(0, ofGetHeight()/2, w/2, h/2);
            right.draw(ofGetWidth()/2, ofGetHeight()/2, w/2, h/2);
            ofPopMatrix();
            break;
            
        case 6:
            displayTitle(); 
            break; 

        default:
            break;
    }

ofPopMatrix(); 

//--------
	stringstream info;
	info << "App FPS: " << ofGetFrameRate() << "\n";
	//info << "Camera frameCounter: " << videoGrabber.frameCounter << "\n";
	//info << "App frameCounter: " << ofGetFrameNum() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
	//info << "DO RECORDING: " << omxCameraSettings.doRecording << "\n";
	
	//info <<	filterCollection.filterList << "\n";
	
	info << "\n";
	info << "Press e to increment filter" << "\n";
	info << "Press g to Toggle info" << "\n";





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
    makeMirror();
    makeLeft();
    makeRight(); 
}

//--------------------------------------------------------------
void textureApp::makeMirror() {
    mirror.clone(pic);
    mirror.mirror(false, true);
    mirror.reloadTexture();
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
void textureApp::makeRight() {
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

//--------------------------------------------------------------
void textureApp::displayTitle() {
    title.draw(0,0);
}

//--------------------------------------------------------------
void textureApp::cyclePics() {
    
    if (ofGetElapsedTimeMillis() - startTime) {
        goSwitch = true; 
    }
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
    if (key == '2') state = 2;
    if (key == '3') state = 3;
    if (key == '4') state = 4;
    if (key == '5') state = 5;


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

