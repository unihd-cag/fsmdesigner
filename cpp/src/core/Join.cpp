/*
 * Join.cpp
 *
 *  Created on: Aug 16, 2011
 *      Author: rleys
 */


// Includes
//------------------------

//-- Core
#include <core/Trackpoint.h>
#include <core/State.h>

#include "Join.h"

Join::Join() {
	this->id = -1;
	this->targetState = NULL;
	this->posx = 0;
	this->posy = 0;

}

Join::~Join() {
	// TODO Auto-generated destructor stub
}

Trackpoint * Join::addTrackpoint() {

	//-- Create Trackpoint
	Trackpoint * tp = new Trackpoint(0,0,0);

	//-- Add
	this->trackpoints.push_back(tp);

	return tp;

}

Trackpoint * Join::addTrackpoint(Trackpoint * trackpoint) {

	//-- Add
	this->trackpoints.push_back(trackpoint);

	return trackpoint;

}

/**
 *
 * @return The list of trackpoints
 */
list<Trackpoint*> Join::getTrackpoints() {
    return this->trackpoints;
}

void Join::setId(int id) {
    this->id = id;
}

int Join::getId() {
    return this->id;
}

void Join::setTargetState(State * state) {
    this->targetState = state;
}

State * Join::getTargetState() {
    return this->targetState;
}

void Join::setPosx(int x) {
    this->posx = x;
}

int Join::getPosx() {
    return this->posx;
}

void Join::setPosy(int y) {
    this->posy = y;
}

int Join::getPosy() {
    return this->posy;
}
