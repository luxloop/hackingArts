#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
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
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	//kinectAngle = 0;
	//kinect.setCameraTiltAngle(kinectAngle);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);
	
	kinect.update();
	if(kinect.isFrameNew()) {
		
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		
        
		grayImage.flagImageChanged();
	}
    
    contourFinder.findContours(grayImage, minBlob, maxBlob, 5, false);
	
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
    
    if (contourFinder.nBlobs > 0) {
        ofxCvBlob blob = contourFinder.blobs.at(0);
        ofVec2f center = blob.centroid;
        center.y -= blob.boundingRect.height/2;
        ofSetColor(255, 0, 0);
        ofCircle(center.x, center.y, 30);
        
        meters = kinect.getDistanceAt(blob.centroid.x, blob.centroid.y) / 1000;
        feet = meters * 3.28083333;
        
        
        world = kinect.getWorldCoordinateAt(blob.centroid.x, blob.centroid.y);
        angle = ofRadToDeg(atan(world.z/world.x));
        
        if (angle == -90.0) {
            angle = 90.0;
        } else if (angle < 0) {
            //angle *= -1;
            angle += 180;
        }
        
        float vidPos = ofMap(angle, 45, 135, 0, 1,true);

        
    } else {
        //No Blobs
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    kinect.drawDepth(0, 0,400,300);
    contourFinder.draw(0,0,400,300);
    
    
    
    switch (dmode) {
        case A:
            if (contourFinder.nBlobs > 0) {
                string worldCoordFeet = ofToString(world.x / 1000 * 3.28083333, 3) + ", " + ofToString(world.y / 1000 * 3.28083333, 3) + ", " + ofToString(world.z / 1000 * 3.28083333, 3);
                string worldCoordMeters = ofToString(world.x / 1000, 3) + ", " + ofToString(world.y / 1000, 3) + ", " + ofToString(world.z / 1000, 3);
                ofDrawBitmapStringHighlight(worldCoordFeet, 660,40);
                
                
                ofSetColor(255, 255, 255);
                ofPushMatrix();
                ofTranslate(ofGetWidth()-320, 480);
                ofScale(0.5, -0.5);
                glLineWidth(2);
                
                ofLine(-320, 0, 320, 0);
                
                ofLine(0, 0, world.x, world.z);
                
                ofDrawBitmapStringHighlight(ofToString(angle,3), 20 , 20);
                
                
                glLineWidth(1);
                ofPopMatrix();
            }
            break;
            
        default:
            break;
    }
    
    
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
    
	reportStream << "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
	ofDrawBitmapString(reportStream.str(), 20, 652);
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(kinectAngle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case OF_KEY_UP:
			kinectAngle++;
			if(kinectAngle>30) kinectAngle=30;
			kinect.setCameraTiltAngle(kinectAngle);
			break;
			
		case OF_KEY_DOWN:
			kinectAngle--;
			if(kinectAngle<-30) kinectAngle=-30;
			kinect.setCameraTiltAngle(kinectAngle);
			break;
            
        case '1':
			//dmode = A;
			break;
            
        case '2':
			//dmode = B;
			break;
            
        case '3':
			//dmode = C;
			break;
            
        case 'b':
			bLearnBakground = true;
			break;
	}
    
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

//--------------------------------------------------------------
void ofApp::exit() {
	//kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}
