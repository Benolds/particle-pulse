//
//  Particle.h
//  MAVPA
//
//  Created by Benjamin Reynolds on 10/31/14.
//
//

#ifndef __MAVPA__Particle__
#define __MAVPA__Particle__

#include <stdio.h>
#include "ofMain.h"

class Particle {
public:
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f accel;
    ofColor col;
    float radius;
    
//    Particle();
    Particle(float i_radius, ofColor i_col, ofVec2f i_pos, ofVec2f i_vel, ofVec2f i_accel);
    void update();
    void draw();

};

#endif /* defined(__MAVPA__Particle__) */
