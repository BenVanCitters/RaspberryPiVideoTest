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
		unsigned char * 	videoInverted;
		ofTexture			videoTexture;
		int 				camWidth;
		int 				camHeight;

    float mVidUpdateTime;
    float mVidUpdateInterval;
    float mUpdateTime;
    float mDrawTime;
    ofFbo m_frameBuffer;
    ofPixels opixels;
    bool showText;
    ThreadedIndexUpdate m_indexUpdater;
};
