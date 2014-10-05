#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    ofBackground(0);
    
    for(int i=0; i<N_VIDEO_PLAYERS_1; i++) {
        videoPlayers1.push_back(new ofxAVFVideoPlayer());
    }
    videoPlayers1[0]->loadMovie("videos/fingers.mov");
    videoPlayers1[1]->loadMovie("videos/fingers2.mov");
    for(auto p : videoPlayers1) {
        //p->play();
        p->setLoopState(OF_LOOP_NORMAL);
    }
    
	ofSetLogLevel(OF_LOG_VERBOSE);
    dmode = A;
    eyeTest.loadMovie("eyeroll.mov");
    eyeTest.stop();
    meters = 0.0;
    feet = 0.0;
    world = ofVec3f(0,1,1);
    float angle = 0.0;
    
    
    gui.setup(); // most of the time you don't need a name
    
    gui.add(minBlob.setup("Min Blob Size", 10000, 5, 20000));
    gui.add(maxBlob.setup("Max Blob Size", 300000, 5, 800000));
	gui.add(bgSubThreshold.setup("BG Sub Threshold", 80, 1, 255));
    
	//kinect.setRegistration(true);  // enable depth->video image calibration
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
    //kinect.setDepthClipping(500,4000);
	kinect.open();
	
    /*
     // print the intrinsic IR sensor values
     if(kinect.isConnected()) {
     ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
     ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
     ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
     ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
     }
     */
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    
	grayCamImage.allocate(kinect.width, kinect.height);
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);
    
	bLearnBakground = true;
    //bgSubThreshold = 30;
	
	nearThreshold = 230;
	farThreshold = 70;
	
	
	// zero the tilt on startup
	//kinectAngle = 0;
	//kinect.setCameraTiltAngle(kinectAngle);
}

//--------------------------------------------------------------
void ofApp::update(){
    for(auto p : videoPlayers1) {
        p->update();
    }
    if (videoPlayers1[0]->isLoaded() && videoPlayers1[1]->isLoaded()) {
        videoPlayers1[0]->play();
        videoPlayers1[1]->play();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //video.draw(0, 0);
    videoPlayers1[0]->draw(0, 0);
    videoPlayers1[1]->draw(ofGetWidth()/2, 0);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
