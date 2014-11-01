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

Particle::Particle(float i_radius, ofColor i_col, ofVec2f i_pos, ofVec2f i_vel, ofVec2f i_accel, int i_lifetime)
{
    this->radius = i_radius;
    this->baseRadius = i_radius;
    this->col = i_col;
    this->pos = i_pos;
    this->vel = i_vel;
    this->accel = i_accel;
    this->numNeighbors = 0;
    this->lifetime = i_lifetime;
    this->baseLifetime = i_lifetime;
    this->flagForRemoval = false;
}

void Particle::update()
{
    accel *= 0.9f;
    vel = ofVec2f(vel.x + accel.x, vel.y + accel.y);
    pos = ofVec2f(vel.x + pos.x, vel.y + pos.y);
    
    lifetime--;
}

void Particle::postUpdate()
{
    for (Particle* n : neighbors)
    {
        ofVec2f dPos = n->pos - pos;
        accel += (dPos * 0.01);
    }
    
    float destinationRadius = 2.0f * baseRadius / pow((1.0f + numNeighbors), 0.33f) * lifetime/baseLifetime;
    radius = this->lerp(radius, destinationRadius, 0.2f);
}

void Particle::draw()
{
    ofSetColor(col);
    
    this->drawLinesToNeighbors();
    
    ofCircle(pos.x, pos.y, radius);
}

void Particle::drawLinesToNeighbors()
{
    for( Particle* n : neighbors){
        ofLine(pos.x, pos.y, n->pos.x, n->pos.y);
    }
}

void Particle::clearNeighbors()
{
    neighbors.clear();
    numNeighbors = 0;
}

void Particle::addNeighbor(Particle* p)
{
    neighbors.push_back(p);
    numNeighbors++;
}


float Particle::lerp(float current, float destination, float percent)
{
    return percent * destination + (1.0f - percent) * current;
}

