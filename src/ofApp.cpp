#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    srand (time(NULL));

    Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(100.0f,100.0f), ofVec2f(20.0f,-5.0f), ofVec2f(0.00f,0.00f));
    particles.push_back(p);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofVec2f mouseVec = this->getMouseToCenter();
    for(Particle* p : particles){
        p->vel = this->addNoiseToVec(mouseVec, 0.2f, 5.0f) * 0.05f;
        p->update();
        this->wrapOnScreenBounds(p);
    }
}

void ofApp::wrapOnScreenBounds(Particle *p){
    if (p->pos.x > ofGetWindowWidth()) {
        p->pos.x = 0.0f;
    } else if (p->pos.x < 0) {
        p->pos.x = ofGetWindowWidth();
    }
    
    if (p->pos.y > ofGetWindowHeight()) {
        p->pos.y = 0.0f;
    } else if (p->pos.y < 0) {
        p->pos.y = ofGetWindowHeight();
    }    
}

ofVec2f ofApp::getWindowCenter()
{
    return ofVec2f(ofGetWindowWidth()*0.5f, ofGetWindowHeight()*0.5f);
}

ofVec2f ofApp::getMouseToCenter()
{
    ofVec2f windowCenter = this->getWindowCenter();
    return ofVec2f(mousePos.x - windowCenter.x, mousePos.y - windowCenter.y);
}

ofVec2f ofApp::addNoiseToVec(ofVec2f baseVec, float dMult, float dAdd){
    float randMult = 1.0f + (dMult * ofRandom(-1.0f, 1.0f));
    float randAdd = dAdd * ofRandom(-1.0f, 1.0f);
    ofVec2f retVal = ofVec2f(baseVec.x * randMult + randAdd, baseVec.y * randMult + randAdd);
    return retVal;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(187, 219, 255);
    
//    ofSetColor(ofColor::whiteSmoke);
//    ofCircle(mousePos.x, mousePos.y, 100);
    
    
    for(Particle* p : particles){
        p->draw();
    }
    
//    p.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    mousePos.x = x;
    mousePos.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(x,y), ofVec2f(20.0f,-5.0f), ofVec2f(0.00f,0.00f));
    particles.push_back(p);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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