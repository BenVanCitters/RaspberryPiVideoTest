#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth 		= 320;	// try to grab at this size. 
	camHeight 		= 240;
	
    //we can now get back a list of devices. 
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName; 
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl; 
        }
	}
    
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight,true);
	
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);	
	ofSetVerticalSync(true);
    
#ifdef TARGET_OPENGLES
    ofFile vertShader(ofToDataPath("shaders_gles/noise.vert"));
    ofFile fragShader(ofToDataPath("shaders_gles/noise.frag"));
    if(vertShader.exists())
    {
        cout << "the vert exists/was found" << endl;
    }
    if(fragShader.exists())
    {
        cout << "the frag exists/was found" << endl;
    }
	shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
#else
	if(ofGetGLProgrammableRenderer()){
		shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
	}else{
        ofFile vertShader(ofToDataPath("shaders/noise.vert"));
        ofFile fragShader(ofToDataPath("shaders/noise.frag"));
        if(vertShader.exists())
        {
            cout << "the vert exists/was found" << endl;
        }
        if(fragShader.exists())
        {
            cout << "the frag exists/was found" << endl;
        }

		shader.load("shaders/noise.vert", "shaders/noise.frag");
	}
#endif
    
	doShader = true;
    m_ofVBOMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    m_ofVBOMesh.enableTextures();
    m_ofVBOMesh.addVertex(ofVec3f(0,0,0));
    m_ofVBOMesh.addTexCoord(ofVec2f(0,0));
    
    m_ofVBOMesh.addVertex(ofVec3f(0,240,0));
    m_ofVBOMesh.addTexCoord(ofVec2f(0,100));
    
    m_ofVBOMesh.addVertex(ofVec3f(320,0,0));
    m_ofVBOMesh.addTexCoord(ofVec2f(100,0));
    
    m_ofVBOMesh.addVertex(ofVec3f(320,240,0));
    m_ofVBOMesh.addTexCoord(ofVec2f(100,100));
}


//--------------------------------------------------------------
void testApp::update(){
	float startTime =ofGetElapsedTimef();
    ofBackground(100,100,100);
    
//	ofLog(OF_LOG_NOTICE, "starting update at %f",);
	vidGrabber.update();
	
//	if (vidGrabber.isFrameNew()){
//		int totalPixels = camWidth*camHeight*3;
//		unsigned char * pixels = vidGrabber.getPixels();
//		for (int i = 0; i < totalPixels; i++){
//			videoInverted[i] = 255 - pixels[i];
//		}
//		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
//	}
    ofLog(OF_LOG_NOTICE, "completing update at %f",(ofGetElapsedTimef()-startTime));
}

//--------------------------------------------------------------
void testApp::draw(){
    float startTime =ofGetElapsedTimef();
    if( doShader ){
		shader.begin();
        //we want to pass in some varrying values to animate our type / color
//        shader.setUniformTexture("s_texture", vidGrabber.getTextureReference(), 1);
        shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
        shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
        
        //we also pass in the mouse position
        //we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
        shader.setUniform2f("mouse", mouseX - ofGetWidth()/2, ofGetHeight()/2-mouseY );
        shader.setUniformTexture("s_texture", vidGrabber.getTextureReference() , 1 );
	}
    
    ofSetHexColor(0xffffff);
    ofPushMatrix();
    ofTranslate(mouseX, mouseY);
    m_ofVBOMesh.draw();
    ofPopMatrix();
//    ofRect(mouseX,mouseY,320,240);//
//	vidGrabber.draw(20,20);

	if( doShader ){
		shader.end();
	}
ofLog(OF_LOG_NOTICE, "completing draw at %f",(ofGetElapsedTimef()-startTime));
//    vidGrabber.draw(20,20);
//	vidGrabber.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());
//	videoTexture.draw(20+camWidth,20,camWidth,camHeight);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
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
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
