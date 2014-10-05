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
        p->setLoopState(OF_LOOP_NONE);
        p->setPaused(true);
    }
    
	//ofSetLogLevel(OF_LOG_VERBOSE);
    dmode = A;
    meters = 0.0;
    feet = 0.0;
    world = ofVec3f(0,1,1);
    float angle = 0.0;
    
    whichVid = 0;
    whichThresh = 0;
    
    playAll = false;
    
    
    gui.setup();
    gui.add(minBlob.setup("Min Blob Size", 10000, 5, 20000));
    gui.add(maxBlob.setup("Max Blob Size", 300000, 5, 800000));
    gui.add(nearThresh.setup("Near Threshold", 230, 0, 512));
    gui.add(farThresh.setup("Far Threshold", 70, 0, 512));
    gui.add(kAngle.setup("Kinect Angle", 0, -30, 30));
    
    
	kinect.init();
	kinect.init(false, false); // disable video image (faster fps)
    //kinect.setDepthClipping(500,4000);
	kinect.open();
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    
	//grayCamImage.allocate(kinect.width, kinect.height);
	//grayBg.allocate(kinect.width, kinect.height);
	//grayDiff.allocate(kinect.width, kinect.height);
	
	//nearThreshold = 230;
	//farThreshold = 70;
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.setCameraTiltAngle(kAngle);
    
    for(auto p : videoPlayers1) {
        p->update();
    }
    
    
	ofBackground(100, 100, 100);
	
	kinect.update();
	if(kinect.isFrameNew()) {
		
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThresh, true);
        grayThreshFar.threshold(farThresh);
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
        
        if (feet >= 8) {
            whichThresh = 3;
        } else if (feet >= 6) {
            whichThresh = 2;
        } else if (feet >= 4) {
            whichThresh = 1;
        } else {
            whichThresh = 0;
        }
        
        
        world = kinect.getWorldCoordinateAt(blob.centroid.x, blob.centroid.y);
        angle = ofRadToDeg(atan(world.z/world.x));
        
        if (angle == -90.0) {
            angle = 90.0;
        } else if (angle < 0) {
            //angle *= -1;
            angle += 180;
        }
        
        if (angle <= 90) {
            whichVid = 0;
        } else {
            whichVid = 1;
        }
        
        float vidPos = ofMap(angle, 45, 135, 0, 1,true);
        
        //make decisions
        
    } else {
        //No blobs
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofSetColor(255, 255, 255);
    
    switch (dmode) {
        case A:
            
            kinect.drawDepth(0, 0, ofGetWidth(),ofGetWidth()*0.75);
            contourFinder.draw(0, 0, ofGetWidth(),ofGetWidth()*0.75);
            
            
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
            
        case B:
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            videoPlayers1[whichVid]->draw(-200, -150, 400, 300);
            //videoPlayers1[1]->draw(ofGetWidth()/2, 0);
            ofPopMatrix();
            
            
            cout << videoPlayers1[0]->getIsMovieDone() << endl;
            break;
            
        case C:
            switch (whichThresh) {
                case 0:
                    ofSetColor(120, 0, 0);
                    break;
                
                case 1:
                    ofSetColor(0, 120, 0);
                    break;
                    
                case 2:
                    ofSetColor(0, 0, 120);
                    break;
                    
                case 3:
                    ofSetColor(120, 120, 120);
                    break;
                    
                default:
                    break;
            }
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(255, 255, 255);
            break;
            
        default:
            break;
    }

    
	
    if (dmode == A) {
        // draw instructions
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        
        reportStream << "Which Zone: " << ofToString(whichThresh) << " " << endl << "num blobs found: " << contourFinder.nBlobs << endl
        << "fps: " << ofGetFrameRate() << endl
        << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
        
        ofDrawBitmapString(reportStream.str(), 20, 652);
        
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
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
			dmode = A;
			break;
            
        case '2':
            playAll = false;
			dmode = B;
			break;
            
        case '3':
            playAll = false;
			dmode = C;
			break;
            
        case ' ':
			if (dmode == B) {
                if (playAll) {
                    for(auto p : videoPlayers1) {
                        p->stop();
                        playAll = false;
                    }
                } else {
                    for(auto p : videoPlayers1) {
                        p->play();
                        playAll = true;
                    }
                }
                
            }
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
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}