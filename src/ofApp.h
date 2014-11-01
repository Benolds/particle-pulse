#pragma once

#include "ofMain.h"
#include "Particle.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private:
        ofVec2f mousePos;
        std::vector<Particle *>particles;
        void wrapOnScreenBounds(Particle* p);
        void countNeighbors(Particle* p, float threshold);
        ofVec2f getWindowCenter();
        ofVec2f getMouseToCenter();
//        ofVec2f getPerpendicularVector(ofVec2f startVec);
//        ofVec2f addNoiseToVec(ofVec2f baseVec, float dMult, float dAdd);
        void spawnRandomParticles(int numToSpawn);
        void mergeIfNeeded(Particle* p, float mergeThreshold);
    
        //audio
        ofSoundStream soundStream;
        void audioReceived(float *input, int bufferSize, int nChannels);
//        void audioOut(float *output, int bufferSize, int nChannels);

        float volume;
        std::vector<float> lastInput;
        std::vector<float> last2Input;
    
        ofSoundPlayer soundPlayer;

};
