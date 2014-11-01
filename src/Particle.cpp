//
//  Particle.cpp
//  MAVPA
//
//  Created by Benjamin Reynolds on 10/31/14.
//
//

#include "Particle.h"

//Particle::Particle()
//{
//    cout << "don't use this";
//}

Particle::Particle(float i_radius, ofColor i_col, ofVec2f i_pos, ofVec2f i_vel, ofVec2f i_accel)
{
    this->radius = i_radius;
    this->col = i_col;
    this->pos = i_pos;
    this->vel = i_vel;
    this->accel = i_accel;
}

void Particle::update()
{
    this->vel = ofVec2f(this->vel.x + this->accel.x, this->vel.y + this->accel.y);
    this->pos = ofVec2f(this->vel.x + this->pos.x, this->vel.y + this->pos.y);
    
    cout << this->pos.x << "\n";
}

void Particle::draw()
{
    ofSetColor(col);
    ofCircle(pos.x, pos.y, radius);
}