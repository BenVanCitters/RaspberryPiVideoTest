#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{

	camWidth 		= 160;	// try to grab at this size.
	camHeight 		= 120;
    
    m_frameBuffer.allocate(camWidth, camHeight, GL_RGB);
	
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
//	ofSetVerticalSync(true);
    


//video grabber - rpi uses /Users/benvancitters/Documents/of_v0.8.0_osx_release/libs/openFrameworks/video/ofGstVideoGrabber.h
//video grabber - rpi uses ofQTKitGrabber.h
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
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
        ofPixels opixels;

        m_frameBuffer.readToPixels(opixels);
//        m_frameBuffer.getTextureReference().
		for (int i = 0; i < totalPixels; i++)
        {
            
            videoInverted[i] =   opixels[i]*8.0/9 + pixels[i]/9.0;  //255 - pixels[i];
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

    ofVec2f rot(400*cos(curTime),400*sin(curTime));
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()/2,ofGetWindowHeight()/2);
    ofTranslate(rot);
    ofRect(500,500,10,10);
    ofPopMatrix();
    
    ofRect(500,500,320,240);//
    
    
	videoTexture.draw(0,0);
    ofSetHexColor(0xffffff);
    m_frameBuffer.end();
    m_frameBuffer.draw(0,0,600,600);

    mDrawTime =(ofGetElapsedTimef()-startTime);
    ofDrawBitmapString("updateTime: "+ofToString(mUpdateTime)+ " drawTime: " + ofToString(mDrawTime),600,100);
    ofDrawBitmapString("mVidUpdateInterval: "+ofToString(mVidUpdateInterval),600,150);
    ofDrawBitmapString("rot: "+ofToString(rot),600,200);
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
