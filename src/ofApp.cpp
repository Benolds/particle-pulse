#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    srand (time(NULL));

//    Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(100.0f,100.0f), ofVec2f(0.0f,0.0f), ofVec2f(0.0f,0.0f));
//    particles.push_back(p);
    
    this->spawnRandomParticles(100);
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofVec2f mouseVec = this->getMouseToCenter();
//    mouseVec = this->getPerpendicularVector(mouseVec);
    
    if ((mouseVec.x > ofGetWindowWidth()*0.5f) ||
        (mouseVec.x < ofGetWindowWidth()*-0.5f) ||
        (mouseVec.y > ofGetWindowHeight()*0.5f) ||
        (mouseVec.y < ofGetWindowHeight()*-0.5f)) {
        mouseVec = ofVec2f(0.0f, 0.0f);
    }
    
    float mouseRotationSpeed = mouseVec.length() / (0.5 * ofGetWindowHeight());
    
    for(Particle* p : particles){
        //pre-update1
        
        ofVec2f particleToCenter = ofVec2f(p->pos - ofVec2f(ofGetWindowWidth()*0.5f, ofGetWindowHeight()*0.5f));
        ofVec2f particleVel = this->getPerpendicularVector(mouseRotationSpeed * this->addNoiseToVec(particleToCenter, 0.2f, 5.0f) * 0.05f);
        
        p->vel = particleVel; //this->addNoiseToVec(mouseVec, 0.2f, 5.0f) * 0.05f;
        
        //update1
        p->update();
        if ( ofRandom(p->lifetime + p->accel.length()) < 1.0f) { //p->lifetime <= 60 &&
            //p->accel.length() < 0.05f) {
            cout << p->accel.length() << "\n";
            p->flagForRemoval = true;
        }
        
        //post-update1
        this->wrapOnScreenBounds(p);
    }
    
    for(Particle* p : particles){
        this->mergeIfNeeded(p, 5.0f);
    }
    
    for(Particle* p : particles){
        if (p->flagForRemoval) {
            particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
        }
    }
    
    // second loop
    for(Particle* p : particles){
        //pre-update2
        this->countNeighbors(p, 150.0f);
        
        //update2
        p->postUpdate();        
    }
    
    spawnRandomParticles(1);
}

void ofApp::mergeIfNeeded(Particle *p, float mergeThreshold)
{
    if (p->flagForRemoval) { return; }
    
    for(Particle* n : particles){
        if (n != p && !n->flagForRemoval) {
            ofVec2f dPos = n->pos - p->pos;
            if (dPos.lengthSquared() < mergeThreshold*mergeThreshold) {
                p->flagForRemoval = true;
                n->lifetime += p->lifetime * 0.5f;
                //n->baseRadius += p->baseRadius * 0.5f;
            }
        }
    }
}

void ofApp::spawnRandomParticles(int numToSpawn)
{
    for (int i = 0; i < numToSpawn; i++) {
        Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(ofRandomWidth(),ofRandomHeight()), ofVec2f(0.0f,0.0f), ofVec2f(0.0f,0.0f), 120);
        particles.push_back(p);
    }
}

void ofApp::countNeighbors(Particle *p, float threshold)
{
    p->clearNeighbors();
    
    for(Particle* n : particles){
        if (n != p) {
            ofVec2f dPos = n->pos - p->pos;
            if (dPos.lengthSquared() < threshold*threshold) {
                p->addNeighbor(n);
            }
        }
    }
}

void ofApp::wrapOnScreenBounds(Particle *p)
{
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

ofVec2f ofApp::getPerpendicularVector(ofVec2f startVec)
{
    return ofVec2f(startVec.y, -1 * startVec.x);
}

ofVec2f ofApp::addNoiseToVec(ofVec2f baseVec, float dMult, float dAdd){
    float randMult = 1.0f + (dMult * ofRandom(-1.0f, 1.0f));
    float randAdd = dAdd * ofRandom(-1.0f, 1.0f);
    ofVec2f retVal = ofVec2f(baseVec.x * randMult + randAdd, baseVec.y * randMult + randAdd);
    return retVal;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(255, 255, 255);
    
//    ofBackground(187, 219, 255);
    
//    ofSetColor(ofColor::whiteSmoke);
//    ofCircle(mousePos.x, mousePos.y, 100);
    
    
    for(Particle* p : particles){
        p->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    this->spawnRandomParticles(10);
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
    Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(x,y), ofVec2f(0.0f,0.0f), ofVec2f(0.0f,0.0f), 120);
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