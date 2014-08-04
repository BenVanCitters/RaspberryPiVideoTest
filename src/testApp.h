#pragma once

#include "ofMain.h"
#include "ThreadedIndexUpdate.h"
//class ThreadedIndexUpdate;

class testApp : public ofBaseApp
{
    
public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);		

    ofVideoGrabber 		vidGrabber;
    unsigned char * 	m_newVideoPixels;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;

    
    float mVidUpdateTime;
    float mVidUpdateInterval;
    float mUpdateTime;
    float mDrawTime;
    ofFbo m_frameBuffer;
    ofPixels opixels;
    bool m_showText;
    bool m_recordingPNGs;
    ThreadedIndexUpdate m_indexUpdater;
};
