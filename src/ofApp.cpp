#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofBackground(50);
    ofBackground(239);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetLineWidth(3);
    ofSetVerticalSync(true);
    
#ifdef TARGET_OPENGLES
    printf("TARGET_OPENGLES\n");
#else
    if(ofIsGLProgrammableRenderer()){
        printf("ofIsGL\n");
    }else{
        printf("else\n");
    }
#endif

    
    // load shader file
    if(!this->shader.load("shader.vert", "shader.frag")) { exit(); };
    
    // allocate fbo
    this->fbo.allocate(ofGetWidth(), ofGetHeight());
    
    // initialize y_noise
    for (int i=0; i < ofGetHeight(); i++) { this->y_noise.push_back(0.f); }
}

//--------------------------------------------------------------
void ofApp::update(){
    // fix to generate ranfom nomber
    ofSeedRandom(39);
    
    // begin fbo
    this->fbo.begin();
    ofClear(0);
    
    // generate location
    vector <ofVec2f> locations;
    
    for (int i=0; i < 30; i++) {
        auto location = glm::vec2(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum()*0.003), 0, 1, 0, ofGetWidth()),
                                  ofMap(ofNoise(ofRandom(1000), ofGetFrameNum()*0.003), 0, 1, 0, ofGetHeight()));
        locations.push_back(location);
    }
    
    
    auto r = 20.f;
    for (int i=0; i < locations.size(); i++) {
        auto min_dist = 150.f;
        for (int j=0; j < locations.size(); j++) {
            // compare any particles
            if (i == j) { continue; }
            
            // get distance and direction between i particle and j particle
            auto dist = ofDist(locations[i].x, locations[i].y, locations[j].x, locations[j].y);
            auto dir = ofVec2f(locations[j].x - locations[i].x, locations[j].y - locations[i].y);
            dir.normalize();
            // save now distance
            if (dist < min_dist) { min_dist = dist; };

            // calculate line-circle interseption
            auto point_i = ofVec2f(locations[i].x + dir.x * r, locations[i].y + dir.y * r);
            auto point_j = ofVec2f(locations[j].x - dir.x * r, locations[j].y - dir.y * r);
            
            // if distance is less than 200 then connect i and j particles
            if (dist < 150) {
                //ofSetColor(198, 156, 197, ofMap(dist, 0, 200, 255, 0));
                ofSetColor(43, 46, 46, ofMap(dist, 0, 150, 255, 0));
                ofDrawCircle(point_i, r * 0.4);
                ofDrawLine(point_i, point_j);
            }
        }
        
        // draw particle
        //ofSetColor(198, 156, 197);
        ofSetColor(43, 46, 46);
        ofNoFill();
        ofDrawCircle(locations[i], r);
        
        ofFill();
        ofDrawCircle(locations[i], ofMap(min_dist, 0, 150, r*0.7, 0));
    }
    
    // end fbo
    this->fbo.end();
    
    float noise_value;
    for (int y=0; y < this->y_noise.size(); y++) {
        if (y % 72 == 0) {
            noise_value = ofMap(ofNoise(y * 0.05, ofGetFrameNum() * 0.03), 0, 1, -1, 1);
            
            if      (noise_value > 0.65)    { noise_value -= 0.65; }
            else if (noise_value < -0.65)   { noise_value += 0.65; }
            else                            { noise_value = 0.0; }
        }
        
        this->y_noise[y] = noise_value;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //this->cam.begin();
    
    this->shader.begin();
    // send to GPU
    this->shader.setUniform1f("time", ofGetElapsedTimef());
    this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    this->shader.setUniformTexture("tex", this->fbo.getTexture(), 1);
    this->shader.setUniform1fv("y_noise", &this->y_noise[0], ofGetHeight());
    
    // draw 
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    this->shader.end();
    
    //this->fbo.draw(0, 0);
    //this->cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
