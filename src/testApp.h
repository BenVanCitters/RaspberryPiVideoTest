#pragma once

#include "ofMain.h"



class testApp : public ofBaseApp{
	
    float sn(double t);

    
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
    int lookupSz;
    float* sinlkup;
    float mVidUpdateTime;
    float mVidUpdateInterval;
    float mUpdateTime;
    float mDrawTime;
    ofFbo m_frameBuffer;
    ofPixels opixels;
    bool showText;
};
