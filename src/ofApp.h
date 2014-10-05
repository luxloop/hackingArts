#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxAVFVideoPlayer.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    
	ofxIntSlider minBlob;
    ofxIntSlider maxBlob;
    ofxIntSlider bgSubThreshold;
    
	ofxPanel gui;
    
    ofxKinect kinect;
    
#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    
    //ofxCvColorImage			colorCamImg;
    ofxCvGrayscaleImage 	grayCamImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    //int bgSubThreshold;
    bool bLearnBakground;
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	
	int nearThreshold;
	int farThreshold;
	
	int kinectAngle;

    float meters;
    float feet;
    ofVec3f world;
    float angle;
    
    enum displayModes {A, B, C};
    displayModes dmode;
    
    std::vector<ofxAVFVideoPlayer *> videoPlayers;
    static const int N_VIDEO_PLAYERS = 2;
   
};
