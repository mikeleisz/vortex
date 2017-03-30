#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    int w, h;
    
    float downsampleAmt;
    int ds_w, ds_h;
    
    ofShader geo_shader, blur_shader;
    ofFbo blur_fbo;
    
    ofPlanePrimitive plane;
    ofNode light;
    
    ofEasyCam cam;
    
    bool record;
    float framerate;
    string filename;
    string container;
    
    ofxVideoRecorder vidRecorder;
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    
    ofVideoPlayer movie;
    ofTexture movTex;
	
    ofFbo record_fbo;
    ofPixels record_pix;
};
