#include "testApp.h"
#include <math.h>
//--------------------------------------------------------------
void testApp::setup()
{
    showText = false;
	camWidth = 160;	// try to grab at this size.
	camHeight = 120;
    
    lookupSz = 600;
    sinlkup = new float[(int)((lookupSz)*M_PI*2)];
    for(int i = 0; i < lookupSz; i++)
    {
        sinlkup[i] = sin(i*2.f*M_PI/lookupSz);
    }
                        
    m_frameBuffer.allocate(camWidth, camHeight, GL_RGB);
	m_frameBuffer.readToPixels(opixels);
    //we can now get back a list of devices. 
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(unsigned int i = 0; i < devices.size(); i++)
    {
		cout << devices[i].id << ": " << devices[i].deviceName; 
        if( devices[i].bAvailable )
        {
            cout << endl;
        }
        else
        {
            cout << " - unavailable " << endl; 
        }
	}
    
	vidGrabber.setDeviceID(0); 
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight,true);
	
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);	

//video grabber - rpi uses /Users/benvancitters/Documents/of_v0.8.0_osx_release/libs/openFrameworks/video/ofGstVideoGrabber.h
//video grabber - rpi uses ofQTKitGrabber.h
}

float testApp::sn(double t)
{
//    t += M_PI*2*(abs((int)(t/(2*M_PI))) + 1);
    t =fmodl(t,2*M_PI) + 2*M_PI;
    t = fmodl(t,2*M_PI);
    t /= 2*M_PI;
    t *= lookupSz;
    return sinlkup[(int)t];
}


//--------------------------------------------------------------
void testApp::update()
{
    float curTime = ofGetElapsedTimef();
	float startTime =curTime;
    
	vidGrabber.update();
	if (vidGrabber.isFrameNew())
    {
        mVidUpdateInterval =ofGetElapsedTimef() -mVidUpdateTime;
        mVidUpdateTime = ofGetElapsedTimef();
        
    }
//	if (vidGrabber.isFrameNew())
    {
        
        float colorMult = 1.f + .2f*sin(155+curTime/13.0);
        float capToBufPct = .01f + .99f*(sn(curTime/9.033)+1)/2.f;
        float bufToScrPct = .03f + .5*(1+sn(50+curTime/6.0))/2.f;
        
		int totalPixels = camWidth*camHeight*3;
        
        unsigned char * pixels = vidGrabber.getPixels();
        
        float wxh =totalPixels/3.f;
        ofVec3f rotCtr(camWidth*(sn(curTime/4)+1)/2,camHeight*(sn(500+curTime/5)+1)/2);
//        for (int i = 0; i < totalPixels/3; i++)
//        {
//            float x = fmod(i,camWidth*1.0f);
//            float y = (i)/camWidth;
//            
//            float ang = atan2(y-rotCtr[0],x-rotCtr[1]);
//            float dist = .1+ofDist(x,y,rotCtr[0],rotCtr[1]);
//            float d = sn(curTime/8)*dist/30;
//            x += (d*cos(curTime/2+ang+HALF_PI));
//            y += (d*sn(curTime/2+ang+HALF_PI));
//            y = (y+ 1.*sn((x+curTime)/(20.*(1+sn(curTime/2.1))/2)));
//            x = (x- 4.*sn((y+curTime/2)/(18.*(1+sn(curTime/5))/2)));
//            int ny = (int)(y);
//            int nx = (int)(x);
//            ny = (ny%camHeight) * camWidth;
//            nx = nx%camWidth;
//            int tmpIndex = (ny + nx);
//            int newColorIdx = fmodf(fmodf(tmpIndex,wxh) + wxh, wxh);
//            int* newColor = (int*)(&pixels[newColorIdx]);
//            int actualColor = (*newColor);
//            videoInverted[3*i+0] = actualColor;//(tmpIndex%(wxh) + (wxh))%(wxh);
//            videoInverted[3*i+1] = (tmpIndex%(wxh) + (wxh))%(wxh);
//            videoInverted[3*i+2] = (tmpIndex%(wxh) + (wxh))%(wxh);
//        }
        float denom = (20.*(1+sn(curTime/2.1))/2);
        float dMult =sn(curTime/8)/30.f;
        for (int i = 0; i < totalPixels; i++)
        {
            float x = fmod(i,camWidth*3.0f);
            float y = i/(camWidth*3.f);
            
            float ang = atan2(y-rotCtr[0],x-rotCtr[1]);
            float dist = 1.6+ofDist(x,y,rotCtr[0],rotCtr[1]);
            float d = dMult*dist;
            x += (d*sn(curTime/3.2+ang));
            y += (d*sn(curTime/4+ang+HALF_PI));
            y = (y+ 1.*sn((x+curTime)/denom));
////            x = (x- 4.*sn((y+curTime/2)/(18.*(1+sn(curTime/5))/2)));
//            float d = 25;
//            x += (d*sn(HALF_PI+y/7.f+curTime*1.5));
//            y += (d*sn(x/10.f+curTime*2));
            int ny = (int)(y);
            int nx = (int)(x);
            ny = (ny%camHeight) * camWidth*3;
            nx = nx%(camWidth*3);
            int tmpIndex = (ny + nx);
            videoInverted[i] = opixels[(tmpIndex%(totalPixels) + (totalPixels))%(totalPixels)];
        }
		for (int i = 0; i < totalPixels; i++)
        {
            videoInverted[i] =ofLerp(videoInverted[i], pixels[i], capToBufPct);
		}
        for (int i = 0; i < camHeight * camWidth; i++)
        {
            int colorINT = (opixels[3*i+0] << 8) | (opixels[3*i+1] << 4) | opixels[3*i+2];
            colorINT =(int)(colorINT *colorMult);
            videoInverted[3*i+0] =ofLerp(videoInverted[3*i+0], (colorINT >> 8) & 255, bufToScrPct);
            videoInverted[3*i+1] =ofLerp(videoInverted[3*i+1], (colorINT >> 4) & 255, bufToScrPct);
            videoInverted[3*i+2] =ofLerp(videoInverted[3*i+2], (colorINT >> 0) & 255, bufToScrPct);
        }
		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}
    mUpdateTime =(ofGetElapsedTimef()-startTime);
}

//--------------------------------------------------------------
void testApp::draw()
{
//    m_frameBuffer.getTextureReference().
    float startTime =ofGetElapsedTimef();
    m_frameBuffer.begin();
//    ofBackground(0,0,0);
    
    float curTime = ofGetElapsedTimef();
//    ofSetColor(255*(1+cos(curTime))/2,255*(sin(curTime)+1)/2, 255*(cos(curTime)+1)/2,255*(sin(144+curTime)+1)/2);

    
	videoTexture.draw(0,0);

    m_frameBuffer.end();
    ofSetHexColor(0xffffff);
    m_frameBuffer.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());

    mDrawTime =(ofGetElapsedTimef()-startTime);
    if(showText)
    {
        ofDrawBitmapString("updateTime: "+ofToString(mUpdateTime)+ " drawTime: " + ofToString(mDrawTime),40,100);
        ofDrawBitmapString("mVidUpdateInterval: "+ofToString(mVidUpdateInterval),40,150);
    }
    m_frameBuffer.readToPixels(opixels);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	
	// Video settings no longer works in 10.7
	// You'll need to compile with the 10.6 SDK for this
    // For Xcode 4.4 and greater, see this forum post on instructions on installing the SDK
    // http://forum.openframeworks.cc/index.php?topic=10343        
	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}
    if (key == 'd' || key == 'D')
    {
        showText = !showText;
        ofLog(OF_LOG_NOTICE, "showText: " + ofToString(showText?"true":"false"));
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
