#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    w = 1024;
    h = 1024;
    
    framerate = 23.98;
    
    ofSetWindowShape(w, h);
    ofSetFrameRate(framerate);
    
    ////////////////////////////////
    
    filename = "test";
    container = ".mov";
    
    vidRecorder.setVideoCodec("prores");
    //vidRecorder.setVideoBitrate("800k");
    //vidRecorder.setPixelFormat("yuv420p"); //glitches with h264 codec
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    record = false;
    
    ////////////////////////////////

    //ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY; //DEPRECATED
    movie.load("iphone_cap_1.mov");
    movie.setUseTexture(true);
    movie.play();
    
    movTex.allocate(movie.getWidth(), movie.getHeight(), GL_RGBA);
    
    ////////////////////////////////
    
    shader.load("shaders/displace.vert", "shaders/basic.frag");
    
    ////////////////////////////////
    
    float planeScale = 0.75;
    int planeWidth = int(ofGetWidth() * planeScale * 9.0/16.0);
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 8;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
    plane.mapTexCoordsFromTexture(movTex);
    plane.enableNormals();
    
    ////////////////////////////////
    
    cam.setDistance(1280);
    
    ////////////////////////////////
    
    record_fbo.allocate(w, h, GL_RGB);
    record_fbo.setUseTexture(true);
    record_pix.allocate(w, h, 3);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (record){
        vidRecorder.addFrame(record_pix);
    }
    
    movie.update();
    if (movie.isFrameNew()){
        movTex.loadData(movie.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //record_fbo.begin();
    
    ////////////////////////////////
    
    ofPushMatrix();
    
    cam.begin();
    ofRotateX(ofRadToDeg(0.5));
    ofRotateY(ofRadToDeg(-0.5));
    
    ////////////////////////////////
    
    shader.begin();
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    movTex.bind();
    plane.draw();
    movTex.unbind();
    
    shader.end();
    
    ////////////////////////////////
    
    cam.end();
    
    ofPopMatrix();
    
    ////////////////////////////////
    
    //record_fbo.readToPixels(record_pix);
    //record_fbo.end();
    
    //record_fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    shader.load("shaders/displace.vert", "shaders/basic.frag");
    
    if (key == 'r'){
        record = !record;
        if (record && !vidRecorder.isInitialized()){
            vidRecorder.setup(filename+ofGetTimestampString()+container, w, h, framerate);
            vidRecorder.start();
        }
    }
    
    if(key=='c'){
        record = false;
        vidRecorder.close();
    }
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recorded video file is now complete." << endl;
}
