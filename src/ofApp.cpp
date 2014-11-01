#include "ofApp.h"
#include "Constants.h"
#include "Utils.h"

//--------------------------------------------------------------
void ofApp::setup(){
    srand (time(NULL));

//    Particle* p = new Particle(10.0f, ofColor(159,174,232), ofVec2f(100.0f,100.0f), ofVec2f(0.0f,0.0f), ofVec2f(0.0f,0.0f));
//    particles.push_back(p);
    
    this->spawnRandomParticles(100);
    
    lastInput.resize(bbInputSize);
    last2Input.resize(bbInputSize);

    soundStream.setup(this, 0, 1, 44100, 512, 4);
    
    soundPlayer.loadSound("gunsout.mp3");
    soundPlayer.setVolume(0.5f);
    soundPlayer.setMultiPlay(true);
    soundPlayer.play();
}

void ofApp::audioReceived(float *input, int bufferSize, int nChannels)
{
    float sum = 0.0f;
    for (int i = 0; i < bufferSize * nChannels; i++) {
        sum += input[i];
//        cout << "input[" << i << "] = " << input[i] << "\n";
        last2Input[i] = lastInput[i];
        lastInput[i] = input[i];
    }
    
    float avg = float(sum) / float(bufferSize);
    
    //cout << input[0] << "\n";
//    cout << avg << "\n";
    
    volume = avg;
}

//--------------------------------------------------------------
void ofApp::update()
{
//    ofVec2f mouseVec = getDistToCenter(mouseVec);
    ofVec2f mouseVec = getMouseToCenter();
//    mouseVec = this->getPerpendicularVector(mouseVec);
    
    if ((mouseVec.x > ofGetWindowWidth()*0.5f) ||
        (mouseVec.x < ofGetWindowWidth()*-0.5f) ||
        (mouseVec.y > ofGetWindowHeight()*0.5f) ||
        (mouseVec.y < ofGetWindowHeight()*-0.5f)) {
        mouseVec = ofVec2f(0.0f, 0.0f);
    }
    
    float mouseRotationSpeed = 0.25f * mouseVec.length() / (0.5 * ofGetWindowHeight());
    
    if (mousePos.x < ofGetWindowWidth()*0.5) {
        mouseRotationSpeed *= -1.0f;
    }
    
    for(Particle* p : particles){
        //pre-update1
        
        ofVec2f particleToCenter = ofVec2f(p->pos - ofVec2f(ofGetWindowWidth()*0.5f, ofGetWindowHeight()*0.5f));
        ofVec2f particleVel = getPerpendicularVector(mouseRotationSpeed * addNoiseToVec(particleToCenter, 0.2f, 5.0f) * 0.05f);
        
        p->vel = particleVel; //this->addNoiseToVec(mouseVec, 0.2f, 5.0f) * 0.05f;
        
        //update1
        p->update();
        if ( ofRandom(p->lifetime + p->accel.length()) < 1.0f) { //p->lifetime <= 60 &&
            //p->accel.length() < 0.05f) {
//            cout << p->accel.length() << "\n";
            p->flagForRemoval = true;
        }
        
        //post-update1
        this->wrapOnScreenBounds(p);
    }
    
    for(Particle* p : particles){
        this->mergeIfNeeded(p, bbMergeThreshold);
    }
    
    for(Particle* p : particles){
        if (p->flagForRemoval) {
            particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
        }
    }
    
    // second loop
    for(Particle* p : particles){
        //pre-update2
        this->countNeighbors(p, bbNeighborThreshold);
        
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
        Particle* p = new Particle(10.0f, bbBlueColor, ofVec2f(ofRandomWidth(),ofRandomHeight()), bbZeroVec, bbZeroVec, bbDefaultLifetime);
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
//
//ofVec2f ofApp::getPerpendicularVector(ofVec2f startVec)
//{
//    return ofVec2f(startVec.y, -1 * startVec.x);
//}
//
//ofVec2f ofApp::addNoiseToVec(ofVec2f baseVec, float dMult, float dAdd){
//    float randMult = 1.0f + (dMult * ofRandom(-1.0f, 1.0f));
//    float randAdd = dAdd * ofRandom(-1.0f, 1.0f);
//    ofVec2f retVal = ofVec2f(baseVec.x * randMult + randAdd, baseVec.y * randMult + randAdd);
//    return retVal;
//}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float scaledVolume = 50.0f * float(bbInputSize) / 10.0f * volume;
    
//    int r = lerpVal(ofGetBackground().r,scaledVolume,0.2f);
//    int g = lerpVal(ofGetBackground().g,scaledVolume,0.2f);
//    int b = lerpVal(ofGetBackground().b,scaledVolume,0.2f);
//    ofBackground(r, g, b);
    
    ofBackground(0);
    
    ofSetColor(113,110,161,100);
    for(int i = 0; i < bbInputSize; i++) {
        float xPos = i / float(bbInputSize) * ofGetWindowWidth();
        
        float lerpedVolume = lerpVal(last2Input[i], lastInput[i], 0.5f);
        float volHeight = bbInputSize/10.0f * lerpedVolume * ofGetWindowHeight() * 0.5f;
        
        ofRect(xPos, ofGetWindowHeight(), ofGetWindowWidth()/float(bbInputSize), volHeight);
    }
    
    for(Particle* p : particles){
        p->setVolumeScale(bbInputSize/10.0f * volume);
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
    Particle* p = new Particle(bbDefaultRadius, bbBlueColor, ofVec2f(x,y), bbZeroVec, bbZeroVec, bbDefaultLifetime);
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