#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    ofBackground(0);
    
    
    /* 1 - Two-Shot */
    
    for(int i=0; i<N_VIDEO_PLAYERS_1; i++) {
        videoPlayers1.push_back(new ofxAVFVideoPlayer());
    }
    videoPlayers1[0]->loadMovie("videos/dialogue0.mov");
    videoPlayers1[1]->loadMovie("videos/dialogue1.mov");
    for(auto p : videoPlayers1) {
        //p->play();
        p->setLoopState(OF_LOOP_NONE);
        p->setPaused(true);
    }
    /* * * * * * * * * * */
    
    
    /* 2 - Boy/Girl Branching Dialogue  */
    
    for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
        vid_2_A.push_back(new ofxAVFVideoPlayer());
        vid_2_A[i]->loadMovie("videos/narr_A.mov");
        vid_2_A[i]->setLoopState(OF_LOOP_NONE);
        vid_2_A[i]->setPaused(true);
        
        vid_2_B.push_back(new ofxAVFVideoPlayer());
        vid_2_B[i]->loadMovie("videos/narr_B_" + ofToString(i) + ".mov");
        vid_2_B[i]->setLoopState(OF_LOOP_NONE);
        vid_2_B[i]->setPaused(true);
    }
    flopper2 = true;
    v2_Path = "videos/narr_";
    v2_Scene = "A";
    v2_Done = false;
    v2_onDeck = true;
    v2_next = 0;
    /* * * * * * * * * * */
    
    
	//ofSetLogLevel(OF_LOG_VERBOSE);
    dmode = A;
    meters = 0.0;
    feet = 0.0;
    world = ofVec3f(0,1,1);
    float angle = 0.0;
    
    whichVid_1 = 0;
    whichVid_2 = 0;
    whichThresh = 0;
    
    aVideoIsPlaying = false;
    showTimeline = false;
    scrubber = 20;
    
    
    gui.setup();
    gui.add(minBlob.setup("Min Blob Size", 10000, 5, 20000));
    gui.add(maxBlob.setup("Max Blob Size", 300000, 5, 800000));
    gui.add(nearThresh.setup("Near Threshold", 230, 0, 512));
    gui.add(farThresh.setup("Far Threshold", 70, 0, 512));
    gui.add(kAngle.setup("Kinect Angle", 15, -30, 30));
    
    
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
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.setCameraTiltAngle(kAngle);
    
    switch (dmode) {
        case A:
            {
            break;
            }
            
        case B:
            {
            /* 1 - Two-Shot Players */
                for(auto p : videoPlayers1) {
                    p->update();
                }
                
                for(auto p : videoPlayers1) {
                    if (p->getIsMovieDone()) {
                        aVideoIsPlaying=false;
                        p->setPaused(true);
                        p->setPosition(0.0);
                    }
                }
            break;
            }
        case C:
            {
            /* 2 - Boy/Girl Branching Dialogue  */
                for(auto p : vid_2_A) {
                    p->update();
                }
                
                for(auto p : vid_2_B) {
                    p->update();
                }
                
                //if (vid_2_A[whichVid_2]->getPosition() > 0.9 && !v2_onDeck) {
                if (!v2_onDeck) {
                    if (v2_Scene == "A") {
                        v2_Scene = "B";
                    } else if (v2_Scene == "B") {
                        v2_Scene = "C";
                    } else if (v2_Scene == "C") {
                        v2_Scene = "D";
                    } else if (v2_Scene == "D") {
                        v2_Scene = "E";
                    } else if (v2_Scene == "E") {
                        v2_Scene = "F";
                    }
                    
                    //string onDeckPath = v2_Path + v2_Scene + "_" + ofToString(whichThresh) + ".mov";
                    
                    if (flopper2) {
                        for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
                            
                            if (v2_Scene != "B") {
                                vid_2_B[i]->stop();
                                vid_2_B[i]->closeMovie();
                                vid_2_B[i]->loadMovie(v2_Path + v2_Scene + "_" + ofToString(i) + ".mov");
                                cout << "loading B: " << vid_2_B[i]->getMoviePath() << endl;
                            }
                            
                            //vid_2_B[i]->setPaused(true);

                        }
                    } else {
                        for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
                            
                            if (v2_Scene != "B") {
                                vid_2_A[i]->stop();
                                vid_2_A[i]->closeMovie();
                                vid_2_A[i]->loadMovie(v2_Path + v2_Scene + "_" + ofToString(i) + ".mov");
                                cout << "loading A: " << vid_2_A[i]->getMoviePath() << endl;
                            }
                            //vid_2_A[i]->setPaused(true);
                            
                        }
                    }
                    
                    v2_onDeck = true;
                }
                
                
                if (flopper2) {
                    if (vid_2_A[whichVid_2]->getPosition() > 0.98 && !v2_Done){
                        v2_next = whichThresh;
                        vid_2_B[v2_next]->play();
                        cout << "playing B" << endl;
                    }
                    
                    if (vid_2_A[whichVid_2]->getIsMovieDone() && !v2_Done) {
                        cout << "VID A Done: " << vid_2_A[whichVid_2]->getMoviePath() << endl;
                        if (v2_Scene == "F") {
                            v2_Done = true;
                        }
                        whichVid_2 = v2_next;
                        v2_next = 0;
                        flopper2 = !flopper2;
                        //videoPlayers2[whichVid_2]->play();
                        v2_onDeck = false;
                        
                    }
                } else {
                    if (vid_2_B[whichVid_2]->getPosition() > 0.98 && !v2_Done){
                        v2_next = whichThresh;
                        vid_2_A[v2_next]->play();
                        cout << "playing B" << endl;
                    }
                    
                    if (vid_2_B[whichVid_2]->getIsMovieDone() && !v2_Done) {
                        cout << "VID B Done: " << vid_2_B[whichVid_2]->getMoviePath() << endl;
                        if (v2_Scene == "F") {
                            v2_Done = true;
                        }
                        whichVid_2 = v2_next;
                        v2_next = 0;
                        flopper2 = !flopper2;
                        //videoPlayers2[whichVid_2]->play();
                        v2_onDeck = false;
                        
                    }
                }
                
                
                if (v2_Done) {
                    aVideoIsPlaying=false;
                    for(auto p : vid_2_A) {
                        p->stop();
                    }
                    for(auto p : vid_2_B) {
                        p->stop();
                    }
                    v2_Done = false;
                }
                
                
                
            break;
            }
        default:
            break;
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
            whichVid_1 = 0;
        } else {
            whichVid_1 = 1;
        }
        
        float vidPos = ofMap(angle, 45, 135, 0, 1,true);
        
        //make decisions
        
    } else {
        //No blobs
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    ofSetColor(255, 255, 255);
    
    switch (dmode) {
        case A:
            {
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
            }
            
        case B:
            {
            
            float vidHeight = (ofGetWidth()*videoPlayers1[0]->getHeight())/videoPlayers1[0]->getWidth();
            float vidStart = (ofGetHeight()-vidHeight)/2;
            
            videoPlayers1[whichVid_1]->draw(0, vidStart, ofGetWidth(), vidHeight);
            break;
            }
    
        case C:
            {
            ofPushMatrix();
            //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            //videoPlayers2[whichVid_2]->draw(-200, -150, 400, 300);
                
            if (flopper2) {
                float vidHeight = (ofGetWidth()*vid_2_A[whichVid_2]->getHeight())/vid_2_A[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                
                vid_2_A[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else {
                float vidHeight = (ofGetWidth()*vid_2_B[whichVid_2]->getHeight())/vid_2_B[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                
                vid_2_B[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            }
                
            ofPopMatrix();
            
            
            
            
            if (showTimeline) {
                ofSetColor(255, 200);
                if (whichThresh == 0) {
                    ofSetColor(94, 0, 182, 200);
                }
                ofRect(20, ofGetHeight() - 100, ofGetWidth()-40, 10);
                
                ofSetColor(255, 200);
                if (whichThresh == 1) {
                    ofSetColor(0, 172, 236, 200);
                }
                ofRect(20, ofGetHeight() - 80, ofGetWidth()-40, 10);
                
                ofSetColor(255, 200);
                if (whichThresh == 2) {
                    ofSetColor(224, 146, 47, 200);
                }
                ofRect(20, ofGetHeight() - 60, ofGetWidth()-40, 10);
                
                ofSetColor(255, 200);
                if (whichThresh == 3) {
                    ofSetColor(190, 0, 0, 200);
                }
                ofRect(20, ofGetHeight() - 40, ofGetWidth()-40, 10);
                
                int xAcross = scrubber;
                if (xAcross > ofGetWidth() - 20) {
                    xAcross = 20;
                    scrubber = 20;
                }
                ofSetColor(0, 0, 255, 255);
                ofRect(xAcross, ofGetHeight()-110, 2, 90);
                scrubber+=0.4;
            }
            
            ofSetColor(255, 255, 255, 255);
            break;
            }
        case D:
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
        
        /* 1 - Two-Shot */
        stringstream video1;
        video1 << "Two-Shot" << endl;
        
        for(auto p : videoPlayers1) {
            video1 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
            video1 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
        }
        
        ofDrawBitmapString(video1.str(), 650, 400);
        
        
        /* 2 - Boy/Girl Branching Dialogue  */
        stringstream video2;
        video2 << "Boy/Girl Dialogue" << endl;
        
        for(auto p : vid_2_A) {
            video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
            video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
        }
        for(auto p : vid_2_B) {
            video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
            video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
        }
        
        
        ofDrawBitmapString(video2.str(), 850, 400);
        
        
        
        
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
            
        case '1':
			dmode = A;
			break;
            
        case '2':
            aVideoIsPlaying = false;
            for(auto p : videoPlayers1) {
                p->setPaused(true);
            }
            for(auto p : vid_2_A) {
                p->setPaused(true);
            }
            for(auto p : vid_2_B) {
                p->setPaused(true);
            }
			dmode = B;
			break;
            
        case '3':
            aVideoIsPlaying = false;
            for(auto p : videoPlayers1) {
                p->setPaused(true);
            }
            for(auto p : vid_2_A) {
                p->setPaused(true);
            }
            for(auto p : vid_2_B) {
                p->setPaused(true);
            }
			dmode = C;
			break;
            
        case '4':
			dmode = D;
			break;
            
        case 't':
            showTimeline = !showTimeline;
			break;
            
        case ' ':
			if (dmode == B) {
                if (aVideoIsPlaying) {
                    for(auto p : videoPlayers1) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                } else {
                    for(auto p : videoPlayers1) {
                        p->play();
                        aVideoIsPlaying = true;
                    }
                }
                
            } else if (dmode == C) {
                if (aVideoIsPlaying) {
                    for(auto p : vid_2_A) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                    for(auto p : vid_2_B) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                } else {
                    if (flopper2) {
                        vid_2_A[whichVid_2]->play();
                    } else {
                        vid_2_B[whichVid_2]->play();
                    }
                    
                    aVideoIsPlaying = true;
                }
            }
			break;
            
        case 'r':
            aVideoIsPlaying = false;
			if (dmode == B) {
                for(auto p : videoPlayers1) {
                    p->setPaused(true);
                    p->setPosition(0.0);
                }
            } else if (dmode == C) {
                for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
                    vid_2_A.push_back(new ofxAVFVideoPlayer());
                    vid_2_A[i]->loadMovie("videos/narr_A.mov");
                    vid_2_A[i]->setLoopState(OF_LOOP_NONE);
                    vid_2_A[i]->setPaused(true);
                    
                    vid_2_B.push_back(new ofxAVFVideoPlayer());
                    vid_2_B[i]->loadMovie("videos/narr_B_" + ofToString(i) + ".mov");
                    vid_2_B[i]->setLoopState(OF_LOOP_NONE);
                    vid_2_B[i]->setPaused(true);
                }
                flopper2 = true;
                whichVid_2 = 0;
                v2_Path = "videos/narr_";
                v2_Scene = "A";
                v2_Done = false;
                v2_Scene = "A";
                scrubber = 20;
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