#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    w = 1024;
    h = 1024;
    
    framerate = 23.98;
    
    ofSetWindowShape(w, h);
    ofSetFrameRate(framerate);
    ofSetVerticalSync(true);
    ofBackground(8, 8, 8);
    ofEnableAntiAliasing();
    //ofToggleFullscreen();
    
    ////////////////////////////////
    
    filename = "test";
    container = ".mov";
    
    vidRecorder.setVideoCodec("prores");
    //vidRecorder.setVideoBitrate("800k");
    //vidRecorder.setPixelFormat("yuv420p"); //glitches with h264 codec
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    record = false;
    
    ////////////////////////////////

    movie.load("iphone_cap_1.mov");
    movie.setUseTexture(true);
    movie.play();
    
    movTex.allocate(movie.getWidth(), movie.getHeight(), GL_RGBA);
    
    ////////////////////////////////
    
    geo_shader.load("shaders/displace.vert", "shaders/displaceTex.frag");
    blur_shader.load("shaders/generic.vert", "shaders/gaussian.frag");
    
    displacement_map.load("Displacements.png");
    displaceTex.allocate(displacement_map.getWidth(), displacement_map.getHeight(), GL_RGBA);
    displaceTex.loadData(displacement_map.getPixels());
    
    ////////////////////////////////
    
    downsampleAmt = 1.0/64.0;
    ds_w = movie.getWidth() * downsampleAmt;
    ds_h = movie.getHeight() * downsampleAmt;
    
    blur_fbo.allocate(ds_w, ds_h, GL_RGB);
    blur_fbo.setUseTexture(true);
    
    record_fbo.allocate(w, h, GL_RGB);
    record_fbo.setUseTexture(true);
    record_pix.allocate(w, h, 3);
    
    ////////////////////////////////
    
    float planeScale = 0.75;
    int planeWidth = int(ofGetWidth() * planeScale * 9.0/16.0);
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 2;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
    plane.mapTexCoordsFromTexture(movTex);
    plane.enableNormals();
    
    ////////////////////////////////
    
    cam.setDistance(1280);
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
    
    blur_fbo.begin();
    blur_shader.begin();
    
    movie.draw(0, 0, ds_w, ds_h);
    
    blur_shader.end();
    blur_fbo.end();
    
    ////////////////////////////////
    
    ofEnableDepthTest();
    ofPushMatrix();
    
    cam.begin();
    ofRotateX(ofRadToDeg(3.14));
    ofRotateX(ofRadToDeg(ofGetElapsedTimef() * 0.1));
    ofRotateY(ofRadToDeg(ofGetElapsedTimef() * 0.25));
    
    ////////////////////////////////
    
    geo_shader.begin();
    
    float t = int(ofGetElapsedTimef() * 50.0) % int(displaceTex.getWidth() * 0.5);
    
    geo_shader.setUniformMatrix4f("camPosition", cam.getModelViewMatrix());
    geo_shader.setUniform1f("time", t);
    geo_shader.setUniform2f("resolution", movTex.getWidth(), movTex.getHeight());
    
    geo_shader.setUniform1f("downsampleAmt", downsampleAmt);
    geo_shader.setUniform1f("displaceAmt", 350.0);
    
    geo_shader.setUniformTexture("tex0", blur_fbo.getTexture(), 0);
    geo_shader.setUniformTexture("tex1", movTex, 1);
    geo_shader.setUniformTexture("tex2", displaceTex, 2);

    plane.draw();
    
    geo_shader.end();
    
    ////////////////////////////////
    

    ////////////////////////////////
    
    cam.end();
    
    ofPopMatrix();
    ofDisableDepthTest();
    
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
    geo_shader.load("shaders/displace.vert", "shaders/basic.frag");
    
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
