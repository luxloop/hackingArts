#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    ofBackground(0);
    
    trim = 0.97;
    
    showLoaded = true;
    
    
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
    
    vid_2_A.push_back(new ofxAVFVideoPlayer());
    vid_2_A[0]->loadMovie("videos/narr_A.mov");
    vid_2_A[0]->setLoopState(OF_LOOP_NONE);
    //vid_2_A[0]->setPaused(true);
    
    vid_2_F.push_back(new ofxAVFVideoPlayer());
    vid_2_F[0]->loadMovie("videos/narr_F_0.mov");
    vid_2_F[0]->setLoopState(OF_LOOP_NONE);
    //vid_2_F[0]->setPaused(true);
    
    for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
        vid_2_B.push_back(new ofxAVFVideoPlayer());
        vid_2_B[i]->loadMovie("videos/narr_B_" + ofToString(i) + ".mov");
        vid_2_B[i]->setLoopState(OF_LOOP_NONE);
        //vid_2_B[i]->setPaused(true);
        
        vid_2_C.push_back(new ofxAVFVideoPlayer());
        vid_2_C[i]->loadMovie("videos/narr_C_" + ofToString(i) + ".mov");
        vid_2_C[i]->setLoopState(OF_LOOP_NONE);
        //vid_2_C[i]->setPaused(true);
        
        vid_2_D.push_back(new ofxAVFVideoPlayer());
        vid_2_D[i]->loadMovie("videos/narr_D_" + ofToString(i) + ".mov");
        vid_2_D[i]->setLoopState(OF_LOOP_NONE);
        //vid_2_D[i]->setPaused(true);
        
        vid_2_E.push_back(new ofxAVFVideoPlayer());
        vid_2_E[i]->loadMovie("videos/narr_E_" + ofToString(i) + ".mov");
        vid_2_E[i]->setLoopState(OF_LOOP_NONE);
        //vid_2_E[i]->setPaused(true);
    }
    flopper2 = true;
    v2_Path = "videos/narr_";
    v2_Scene = "A";
    v2_Done = false;
    v2_onDeck = true;
    v2_Next = "B";
    /* * * * * * * * * * */
    
    
    /* 3 - Distance  */
    
    for(int i=0; i<N_VIDEO_PLAYERS_3; i++) {
        vid_3.push_back(new ofxAVFVideoPlayer());
        vid_3[i]->loadMovie("videos/distance_" + ofToString(i) + ".mov");
        vid_3[i]->setLoopState(OF_LOOP_NONE);
        //vid_3[i]->setPaused(true);
    }
    
    invDist = false;
    whichVid_3 = 0;
    /* * * * * * * * * * */
    
    
    /* 4 - Bike  */
    
    for(int i=0; i<N_VIDEO_PLAYERS_4; i++) {
        vid_4.push_back(new ofxAVFVideoPlayer());
        vid_4[i]->loadMovie("videos/bike_" + ofToString(i) + ".mov");
        vid_4[i]->setLoopState(OF_LOOP_NONE);
        //vid_4[i]->setPaused(true);
    }
    
    whichVid_4 = 0;
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
    whichThreshLR = 0;
    
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
    
    for(auto p : videoPlayers1) {
        p->update();
    }
    
    for(auto p : vid_2_A) {
        p->update();
    }
    for(auto p : vid_2_B) {
        p->update();
    }
    for(auto p : vid_2_C) {
        p->update();
    }
    for(auto p : vid_2_D) {
        p->update();
    }
    for(auto p : vid_2_E) {
        p->update();
    }
    for(auto p : vid_2_F) {
        p->update();
    }
    
    for(auto p : vid_3) {
        p->update();
    }
    
    for(auto p : vid_4) {
        p->update();
    }

    
    switch (dmode) {
        case A:
            {
            break;
            }
            
        case B:
            {
            /* 1 - Two-Shot Players */
//                for(auto p : videoPlayers1) {
//                    p->update();
//                }
                
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
//                for(auto p : vid_2_A) {
//                    p->update();
//                }
//                for(auto p : vid_2_B) {
//                    p->update();
//                }
//                for(auto p : vid_2_C) {
//                    p->update();
//                }
//                for(auto p : vid_2_D) {
//                    p->update();
//                }
//                for(auto p : vid_2_E) {
//                    p->update();
//                }
//                for(auto p : vid_2_F) {
//                    p->update();
//                }
                
                
                if (v2_Scene == "A") {
                    v2_Next = "B";
                    if (vid_2_A[whichVid_2]->getPosition() > trim){
                        v2_Thresh = whichThresh;
                        vid_2_B[v2_Thresh]->play();
                    }
                    
                    if (vid_2_A[whichVid_2]->getIsMovieDone()) {
                        whichVid_2 = v2_Thresh;
                        v2_Thresh = 0;
                        //videoPlayers2[whichVid_2]->play();
                        v2_Scene = v2_Next;
                    }
                    
                } else if (v2_Scene == "B") {
                    v2_Next = "C";
                    if (vid_2_B[whichVid_2]->getPosition() > trim){
                        v2_Thresh = whichThresh;
                        vid_2_C[v2_Thresh]->play();
                    }
                    
                    if (vid_2_B[whichVid_2]->getIsMovieDone()) {
                        whichVid_2 = v2_Thresh;
                        v2_Thresh = 0;
                        //videoPlayers2[whichVid_2]->play();
                        v2_Scene = v2_Next;
                    }
                    
                } else if (v2_Scene == "C") {
                    v2_Next = "D";
                    if (vid_2_C[whichVid_2]->getPosition() > trim){
                        v2_Thresh = whichThresh;
                        vid_2_D[v2_Thresh]->play();
                    }
                    
                    if (vid_2_C[whichVid_2]->getIsMovieDone()) {
                        whichVid_2 = v2_Thresh;
                        v2_Thresh = 0;
                        //videoPlayers2[whichVid_2]->play();
                        v2_Scene = v2_Next;
                    }
                    
                } else if (v2_Scene == "D") {
                    v2_Next = "E";
                    if (vid_2_D[whichVid_2]->getPosition() > trim){
                        v2_Thresh = whichThresh;
                        vid_2_E[v2_Thresh]->play();
                    }
                    
                    if (vid_2_D[whichVid_2]->getIsMovieDone()) {
                        whichVid_2 = v2_Thresh;
                        v2_Thresh = 0;
                        //videoPlayers2[whichVid_2]->play();
                        v2_Scene = v2_Next;
                    }
                    
                } else if (v2_Scene == "E") {
                    v2_Next = "F";
                    if (vid_2_E[whichVid_2]->getPosition() > trim){
                        v2_Thresh = whichThresh;
                        vid_2_F[0]->play();
                    }
                    
                    if (vid_2_E[whichVid_2]->getIsMovieDone()) {
                        whichVid_2 = v2_Thresh;
                        v2_Thresh = 0;
                        //videoPlayers2[whichVid_2]->play();
                        v2_Scene = v2_Next;
                    }
                    
                } else if (v2_Scene == "F") {
                    v2_Next = "X";
                    if (vid_2_F[0]->getIsMovieDone()) {
                        v2_Done = true;
                    }
                }
                
                
                if (v2_Done) {
                    aVideoIsPlaying=false;
                    for(auto p : vid_2_A) {
                        p->stop();
                        p->closeMovie();
                    }
                    for(auto p : vid_2_B) {
                        p->stop();
                        p->closeMovie();
                    }
                    for(auto p : vid_2_C) {
                        p->stop();
                        p->closeMovie();
                    }
                    for(auto p : vid_2_D) {
                        p->stop();
                        p->closeMovie();
                    }
                    for(auto p : vid_2_E) {
                        p->stop();
                        p->closeMovie();
                    }
                    for(auto p : vid_2_F) {
                        p->stop();
                        p->closeMovie();
                    }
                    v2_Done = false;
                }
                
            break;
            }
            
        case D:
        {
            for(auto p : vid_3) {
                p->setPaused(true);
            }
            
            if (!invDist) {
                whichVid_3 = whichThresh;
            } else {
                whichVid_3 = abs(whichThresh-3);
            }
            for (int i = 0; i < N_VIDEO_PLAYERS_3; i++) {
                if (i == whichVid_3 && aVideoIsPlaying) {
                    vid_3[i]->play();
                }
            }
            
            for(auto p : vid_3) {
                if (p->getIsMovieDone()) {
                    aVideoIsPlaying=false;
                    p->setPaused(true);
                    p->setPosition(0.0);
                }
            }
            
            break;
        }
            
        case E:
        {
            for(auto p : vid_4) {
                p->setPaused(true);
            }
            
            whichVid_4 = whichThreshLR;
            
            for (int i = 0; i < N_VIDEO_PLAYERS_4; i++) {
                if (i == whichVid_4 && aVideoIsPlaying) {
                    vid_4[i]->play();
                }
            }
            
            for(auto p : vid_4) {
                if (p->getIsMovieDone()) {
                    aVideoIsPlaying=false;
                    p->setPaused(true);
                    p->setPosition(0.0);
                }
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
            if (angle <= 70) {
                whichThreshLR = 0;
            } else {
                whichThreshLR = 1;
            }
        } else {
            whichVid_1 = 1;
            if (angle <= 110) {
                whichThreshLR = 2;
            } else {
                whichThreshLR = 3;
            }
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
                
            } else {
                float vidHeight = (ofGetWidth()*vid_2_B[whichVid_2]->getHeight())/vid_2_B[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                
                vid_2_B[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            }
                
            
            if (v2_Scene == "A") {
                float vidHeight = (ofGetWidth()*vid_2_A[whichVid_2]->getHeight())/vid_2_A[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_A[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else if (v2_Scene == "B") {
                float vidHeight = (ofGetWidth()*vid_2_B[whichVid_2]->getHeight())/vid_2_B[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_B[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else if (v2_Scene == "C") {
                float vidHeight = (ofGetWidth()*vid_2_C[whichVid_2]->getHeight())/vid_2_C[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_C[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else if (v2_Scene == "D") {
                float vidHeight = (ofGetWidth()*vid_2_D[whichVid_2]->getHeight())/vid_2_D[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_D[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else if (v2_Scene == "E") {
                float vidHeight = (ofGetWidth()*vid_2_E[whichVid_2]->getHeight())/vid_2_E[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_E[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            } else if (v2_Scene == "F") {
                float vidHeight = (ofGetWidth()*vid_2_F[whichVid_2]->getHeight())/vid_2_F[whichVid_2]->getWidth();
                float vidStart = (ofGetHeight()-vidHeight)/2;
                vid_2_F[whichVid_2]->draw(0, vidStart, ofGetWidth(), vidHeight);
            }
                
            ofPopMatrix();
            
            
            
            
//            if (showTimeline) {
//                ofSetColor(255, 200);
//                if (whichThresh == 0) {
//                    ofSetColor(94, 0, 182, 200);
//                }
//                ofRect(20, ofGetHeight() - 100, ofGetWidth()-40, 10);
//                
//                ofSetColor(255, 200);
//                if (whichThresh == 1) {
//                    ofSetColor(0, 172, 236, 200);
//                }
//                ofRect(20, ofGetHeight() - 80, ofGetWidth()-40, 10);
//                
//                ofSetColor(255, 200);
//                if (whichThresh == 2) {
//                    ofSetColor(224, 146, 47, 200);
//                }
//                ofRect(20, ofGetHeight() - 60, ofGetWidth()-40, 10);
//                
//                ofSetColor(255, 200);
//                if (whichThresh == 3) {
//                    ofSetColor(190, 0, 0, 200);
//                }
//                ofRect(20, ofGetHeight() - 40, ofGetWidth()-40, 10);
//                
//                int xAcross = scrubber;
//                if (xAcross > ofGetWidth() - 20) {
//                    xAcross = 20;
//                    scrubber = 20;
//                }
//                ofSetColor(0, 0, 255, 255);
//                ofRect(xAcross, ofGetHeight()-110, 2, 90);
//                scrubber+=0.4;
//            }
//            
//            ofSetColor(255, 255, 255, 255);
            break;
            }
            
        case D:
        {
            
            float vidHeight = (ofGetWidth()*vid_3[0]->getHeight())/vid_3[0]->getWidth();
            float vidStart = (ofGetHeight()-vidHeight)/2;
            
            vid_3[whichVid_3]->draw(0, vidStart, ofGetWidth(), vidHeight);
            break;
        }
            
        case E:
        {
            
            float vidHeight = (ofGetWidth()*vid_4[0]->getHeight())/vid_4[0]->getWidth();
            float vidStart = (ofGetHeight()-vidHeight)/2;
            
            vid_4[whichVid_4]->draw(0, vidStart, ofGetWidth(), vidHeight);
            break;
        }
            
        case Z:
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

    
	
    if (dmode == A) {
        // draw instructions
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        
        reportStream << "Which Zone: " << ofToString(whichThresh) << " " << endl << "num blobs found: " << contourFinder.nBlobs << endl
        << "fps: " << ofGetFrameRate() << endl
        << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
        
        ofDrawBitmapString(reportStream.str(), 20, 652);
        
        if (showLoaded) {
        
            /* 1 - Two-Shot */
            stringstream video1;
            video1 << "Two-Shot" << endl;
            
            for(auto p : videoPlayers1) {
                video1 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video1 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            ofDrawBitmapString(video1.str(), 650, 50);
            
            
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
            for(auto p : vid_2_C) {
                video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            for(auto p : vid_2_D) {
                video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            for(auto p : vid_2_E) {
                video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            for(auto p : vid_2_F) {
                video2 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video2 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            ofDrawBitmapString(video2.str(), 850, 50);
            
            /* 3 - Distance */
            stringstream video3;
            video3 << "Distance" << endl;
            
            for(auto p : vid_3) {
                video3 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video3 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            ofDrawBitmapString(video3.str(), 1050, 50);
            
            /* 4 - Bike */
            stringstream video4;
            video4 << "Bike" << endl;
            
            for(auto p : vid_4) {
                video4 << ofToString(p->getMoviePath()) << endl;                ///////////////NEEDS LUXLOOP VERSION OF AVFVIDEOPLAYER
                video4 << "isLoaded: " << p->isLoaded() << endl << "" << endl;
            }
            ofDrawBitmapString(video4.str(), 1050, 350);
        }
        
        
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
            pauseAll();
			dmode = A;
			break;
            
        case '2':
            pauseAll();
            dmode = B;
			break;
            
        case '3':
            pauseAll();
			dmode = C;
			break;
            
        case '4':
            pauseAll();
			dmode = D;
			break;
            
        case '5':
            pauseAll();
			dmode = E;
			break;
            
        case '0':
            pauseAll();
			dmode = Z;
			break;
            
        case 't':
            showTimeline = !showTimeline;
			break;
            
        case 'l':
            showLoaded = !showLoaded;
			break;
        
        case 'i':
            invDist = !invDist;
			break;
            
        case 'x':
            clearAll();
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
                    for(auto p : vid_2_C) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                    for(auto p : vid_2_D) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                    for(auto p : vid_2_E) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                    for(auto p : vid_2_F) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                } else {
                    if (v2_Scene == "A") {
                        vid_2_A[whichVid_2]->play();
                    } else if (v2_Scene == "B") {
                        vid_2_B[whichVid_2]->play();
                    } else if (v2_Scene == "C") {
                        vid_2_C[whichVid_2]->play();
                    } else if (v2_Scene == "D") {
                        vid_2_D[whichVid_2]->play();
                    } else if (v2_Scene == "E") {
                        vid_2_E[whichVid_2]->play();
                    } else if (v2_Scene == "F") {
                        vid_2_F[whichVid_2]->play();
                    }
                    
                    aVideoIsPlaying = true;
                }
            } else if (dmode == D) {
                if (aVideoIsPlaying) {
                    for(auto p : vid_3) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                } else {
                    for(auto p : vid_3) {
                        p->play();
                        aVideoIsPlaying = true;
                    }
                }
                
            } else if (dmode == E) {
                if (aVideoIsPlaying) {
                    for(auto p : vid_4) {
                        //p->stop();
                        p->setPaused(true);
                        aVideoIsPlaying = false;
                    }
                } else {
                    for(auto p : vid_4) {
                        p->play();
                        aVideoIsPlaying = true;
                    }
                }
                
            }
			break;
            
        case 'r':
            aVideoIsPlaying = false;
			if (dmode == B) {
                videoPlayers1.clear();
                for(int i=0; i<N_VIDEO_PLAYERS_1; i++) {
                    videoPlayers1.push_back(new ofxAVFVideoPlayer());
                }
                videoPlayers1[0]->loadMovie("videos/dialogue0.mov");
                videoPlayers1[1]->loadMovie("videos/dialogue1.mov");
                for(auto p : videoPlayers1) {
                    p->setPaused(true);
                    p->setPosition(0.0);
                }
            } else if (dmode == C) {
                vid_2_A.clear();
                vid_2_B.clear();
                vid_2_C.clear();
                vid_2_D.clear();
                vid_2_E.clear();
                vid_2_F.clear();
                
                vid_2_A.push_back(new ofxAVFVideoPlayer());
                vid_2_A[0]->loadMovie("videos/narr_A.mov");
                vid_2_A[0]->setLoopState(OF_LOOP_NONE);
                //vid_2_A[0]->setPaused(true);
                
                
                vid_2_F.push_back(new ofxAVFVideoPlayer());
                vid_2_F[0]->loadMovie("videos/narr_F_0.mov");
                vid_2_F[0]->setLoopState(OF_LOOP_NONE);
                //vid_2_F[0]->setPaused(true);
                
                for(int i=0; i<N_VIDEO_PLAYERS_2; i++) {
                    //vid_2_B.clear();
                    vid_2_B.push_back(new ofxAVFVideoPlayer());
                    vid_2_B[i]->loadMovie("videos/narr_B_" + ofToString(i) + ".mov");
                    vid_2_B[i]->setLoopState(OF_LOOP_NONE);
                    //vid_2_B[i]->setPaused(true);
                    
                    //vid_2_C.clear();
                    vid_2_C.push_back(new ofxAVFVideoPlayer());
                    vid_2_C[i]->loadMovie("videos/narr_C_" + ofToString(i) + ".mov");
                    vid_2_C[i]->setLoopState(OF_LOOP_NONE);
                    //vid_2_C[i]->setPaused(true);
                    
                    //vid_2_D.clear();
                    vid_2_D.push_back(new ofxAVFVideoPlayer());
                    vid_2_D[i]->loadMovie("videos/narr_D_" + ofToString(i) + ".mov");
                    vid_2_D[i]->setLoopState(OF_LOOP_NONE);
                    //vid_2_D[i]->setPaused(true);
                    
                    //vid_2_E.clear();
                    vid_2_E.push_back(new ofxAVFVideoPlayer());
                    vid_2_E[i]->loadMovie("videos/narr_E_" + ofToString(i) + ".mov");
                    vid_2_E[i]->setLoopState(OF_LOOP_NONE);
                    //vid_2_E[i]->setPaused(true);
                }
                flopper2 = true;
                v2_Path = "videos/narr_";
                v2_Scene = "A";
                v2_Done = false;
                v2_onDeck = true;
                v2_Next = "B";
                
                whichVid_2 = 0;
            } else if (dmode == D) {
                vid_3.clear();
                for(int i=0; i<N_VIDEO_PLAYERS_3; i++) {
                    vid_3.push_back(new ofxAVFVideoPlayer());
                    vid_3[i]->loadMovie("videos/distance_" + ofToString(i) + ".mov");
                    vid_3[i]->setLoopState(OF_LOOP_NONE);
                    //vid_3[i]->setPaused(true);
                }
//                for(auto p : vid_3) {
//                    p->setPaused(true);
//                    p->setPosition(0.0);
//                }
                //invDist = false;
                //whichVid_3 = 0;
            } else if (dmode == E) {
                vid_4.clear();
                for(int i=0; i<N_VIDEO_PLAYERS_4; i++) {
                    vid_4.push_back(new ofxAVFVideoPlayer());
                    vid_4[i]->loadMovie("videos/bike_" + ofToString(i) + ".mov");
                    vid_4[i]->setLoopState(OF_LOOP_NONE);
                    //vid_4[i]->setPaused(true);
                }
//                for(auto p : vid_4) {
//                    p->setPaused(true);
//                    p->setPosition(0.0);
//                }
                //invDist = false;
                //whichVid_r = 0;
            }
			break;
            
            /* 4 - Bike  */
            
            
            
            whichVid_4 = 0;
            /* * * * * * * * * * */
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
void ofApp::clearAll() {
    aVideoIsPlaying = false;
    
    
    switch (dmode) {
        case B:
            videoPlayers1.clear();
            break;
            
        case C:
            videoPlayers1.clear();
            vid_2_A.clear();
            vid_2_B.clear();
            vid_2_C.clear();
            vid_2_D.clear();
            vid_2_E.clear();
            vid_2_F.clear();
            break;
            
        case D:
            vid_3.clear();
            break;
            
        case E:
            vid_4.clear();
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::pauseAll() {
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
    for(auto p : vid_2_C) {
        p->setPaused(true);
    }
    for(auto p : vid_2_D) {
        p->setPaused(true);
    }
    for(auto p : vid_2_E) {
        p->setPaused(true);
    }
    for(auto p : vid_2_F) {
        p->setPaused(true);
    }
    
    for(auto p : vid_3) {
        p->setPaused(true);
    }
    
    for(auto p : vid_4) {
        p->setPaused(true);
    }
    
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}