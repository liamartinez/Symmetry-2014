#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
if(wiringPiSetup() == -1){
    	printf("Error on wiringPi setup\n");
    	}

    	pinMode(5,INPUT);
    	pinMode (6, OUTPUT); 
}

//--------------------------------------------------------------
void testApp::update(){

cout << digitalRead (5) << endl; 
}

//--------------------------------------------------------------
void testApp::draw(){

	    digitalWrite (6, HIGH) ; delay (500) ;
    digitalWrite (6,  LOW) ; delay (500) ;

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}