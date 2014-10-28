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
    
    void pauseAll();
    
    ofxIntSlider minBlob;
    ofxIntSlider maxBlob;
    ofxIntSlider nearThresh;
    ofxIntSlider farThresh;
    ofxFloatSlider kAngle;
    
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
    //ofxCvGrayscaleImage 	grayCamImage;
    //ofxCvGrayscaleImage 	grayBg;
    //ofxCvGrayscaleImage 	grayDiff;
    
    //int bgSubThreshold;
    bool bLearnBakground;
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	
	//int nearThreshold;
	//int farThreshold;
	
	int kinectAngle;
    
    enum displayModes {A, B, C, D, E, Z};
    displayModes dmode;
    
    float meters;
    float feet;
    ofVec3f world;
    float angle;
    
    int whichVid_1;
    int whichThresh;
    int whichThreshLR;
    bool showTimeline;
    float scrubber;
    
    bool showLoaded;
    
    bool aVideoIsPlaying;
    
    float trim;

    /* 1 - Two-Shot Players */
    std::vector<ofxAVFVideoPlayer *> videoPlayers1;
    static const int N_VIDEO_PLAYERS_1 = 2;
    /* * * * * * * * * * */
    
    
    /* 2 - Boy/Girl Branching Dialogue  */
	std::vector<ofxAVFVideoPlayer *> vid_2_A;
    std::vector<ofxAVFVideoPlayer *> vid_2_B;
    std::vector<ofxAVFVideoPlayer *> vid_2_C;
    std::vector<ofxAVFVideoPlayer *> vid_2_D;
    std::vector<ofxAVFVideoPlayer *> vid_2_E;
    std::vector<ofxAVFVideoPlayer *> vid_2_F;
    static const int N_VIDEO_PLAYERS_2 = 4;
    bool flopper2;
    
    string v2_Path;
    string v2_Scene;
    string v2_Next;
    int v2_Thresh;
    bool v2_onDeck;
    bool v2_Done;
    int whichVid_2;
    /* * * * * * * * * * */
    
    
    /* 3 - Distance  */
    std::vector<ofxAVFVideoPlayer *> vid_3;
    static const int N_VIDEO_PLAYERS_3 = 4;
    bool invDist;
    int whichVid_3;
    /* * * * * * * * * * */
    
    
    /* 4 - Bike  */
    std::vector<ofxAVFVideoPlayer *> vid_4;
    static const int N_VIDEO_PLAYERS_4 = 4;
    int whichVid_4;
    /* * * * * * * * * * */
};
