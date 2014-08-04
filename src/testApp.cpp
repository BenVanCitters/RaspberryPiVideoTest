#include "testApp.h"
#include <math.h>
#include "ThreadedIndexUpdate.h"

//--------------------------------------------------------------
void testApp::setup()
{

    m_showText = m_recordingPNGs = false;
    //set our camera resolution - ultra low res for rasberry pi B
	camWidth = 160;
	camHeight = 120;

    //setup our threaded function with capture dimensions
    m_indexUpdater.setup(camWidth,camHeight);
    
    //screen framebuffer
    m_frameBuffer.allocate(camWidth, camHeight, GL_RGB);
	m_frameBuffer.readToPixels(opixels);
    
    //setup capture
	vidGrabber.setDeviceID(0); 
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight,true);
	
	m_newVideoPixels = new unsigned char[camWidth*camHeight*3];
    
	videoTexture.allocate(camWidth,camHeight, GL_RGB);
    
    //notes about the video source
    //video grabber - rpi uses of_v0.8.0_osx_release/libs/openFrameworks/video/ofGstVideoGrabber.h
    //video grabber - rpi uses ofQTKitGrabber.h
    
    //fire off the index offset thread
    m_indexUpdater.start();
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

    float colorMult = 1.f + .2f* m_indexUpdater.sn(155+curTime/13.0);
    float capToBufPct = .01f + .99f*(m_indexUpdater.sn(curTime/9.033)+1)/2.f;
    float bufToScrPct = .03f + .5*(1+m_indexUpdater.sn(50+curTime/6.0))/2.f;
    
    int totalPixels = camWidth*camHeight*3;
    
    unsigned char * pixels = vidGrabber.getPixels();
    
    //apply motion vector to the pixels
    for (int i = 0; i < totalPixels; i++)
    {
        int ndx =m_indexUpdater.m_indecies[i];
        m_newVideoPixels[i] = opixels[ndx];
    }
    
    //blend pixels from last frame to this frame
    for (int i = 0; i < totalPixels; i++)
    {
        m_newVideoPixels[i] =ofLerp(m_newVideoPixels[i], pixels[i], capToBufPct);
    }
    
    // do a processing-style multiply the entire color-as-int and determine the result
    // what that means is: treat the each pixel's color tuple as a single  int(like
    // in processing) multiply that int by a floating point value then convert the
    // result back to correct pixel format
    for (int i = 0; i < camHeight * camWidth; i++)
    {
        int colorINT = (opixels[3*i+0] << 8) | (opixels[3*i+1] << 4) | opixels[3*i+2];
        colorINT =(int)(colorINT *colorMult);
        m_newVideoPixels[3*i+0] = ofLerp(m_newVideoPixels[3*i+0], (colorINT >> 8) & 255, bufToScrPct);
        m_newVideoPixels[3*i+1] = ofLerp(m_newVideoPixels[3*i+1], (colorINT >> 4) & 255, bufToScrPct);
        m_newVideoPixels[3*i+2] = ofLerp(m_newVideoPixels[3*i+2], (colorINT >> 0) & 255, bufToScrPct);
    }
    videoTexture.loadData(m_newVideoPixels, camWidth,camHeight, GL_RGB);
	
    mUpdateTime =(ofGetElapsedTimef()-startTime);
}

//--------------------------------------------------------------
void testApp::draw()
{
    float startTime =ofGetElapsedTimef();

    //draw to the frame buffer so that
    m_frameBuffer.begin();
	videoTexture.draw(0,0);
    m_frameBuffer.end();
    ofSetHexColor(0xffffff);
    m_frameBuffer.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());

    mDrawTime =(ofGetElapsedTimef()-startTime);
    
    if(ofGetElapsedTimef() < 5)
        ofDrawBitmapString("Press 'd' to show image processing info\nPress 'r' to turn frame recording on and off",10,60);
    if(m_showText)
    {
        ofDrawBitmapString("updateTime: "+ofToString(mUpdateTime)+ " drawTime: " + ofToString(mDrawTime),40,100);
        ofDrawBitmapString("mIndexUpdateDuration: "+ofToString(m_indexUpdater.getUpdateDuration()),40,125);
        ofDrawBitmapString("mVidUpdateInterval: "+ofToString(mVidUpdateInterval),40,150);
    }
    m_frameBuffer.readToPixels(opixels);
    
    //code to allow recording 'processing-style' png-sequence
    if(m_recordingPNGs)
    {
        ofDrawBitmapString("recording...",40,100);
        std::ostringstream ss;
        ss << "frame" << std::setw(5) << std::setfill('0') << ofGetFrameNum() << ".png";
        ofSaveImage(opixels, "frames/"+ss.str());
//        ofSaveScreen("frames/"+ss.str());
    }
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
        m_showText = !m_showText;
        ofLog(OF_LOG_NOTICE, "showText: " + ofToString(m_showText?"true":"false") + " tm: " + ofToString(ofGetElapsedTimef()));
	}
    
    if(key == 'r' || key == 'R')
    {
        m_recordingPNGs = ! m_recordingPNGs;
        ofLog(OF_LOG_NOTICE, "recordingPNGs: " + ofToString(m_recordingPNGs?"true":"false") + " tm: " + ofToString(ofGetElapsedTimef()));
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
