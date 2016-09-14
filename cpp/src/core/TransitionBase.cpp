/**
 * TransitionBase.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */
// Includes
//-----------------

//-- Std
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

//-- Qt
#include <QtCore>

//-- Core
#include <core/UniqueIDObject.h>
#include <core/Condition.h>
#include <core/Trackpoint.h>


#include "TransitionBase.h"

TransitionBase::TransitionBase() : UniqueIDObject() {
    this->name = "";

}

TransitionBase::~TransitionBase() {

}


string TransitionBase::getName() {
    return this->name;
}


void TransitionBase::setName(string name) {
    this->name = name;
}


vector<Condition*>& TransitionBase::getConditions() {
    return this->conditions;
}

Condition* TransitionBase::getConditionByID(unsigned int id) {

    //-- Check
    if (id>=this->conditions.size())
        throw invalid_argument("Trying to get a condition at an index > size()");

    //-- Find pointer
    return  *(this->conditions.begin()+id);

}

Condition * TransitionBase::addCondition(int numberOfInputBits) {

    this->conditions.push_back(new Condition(numberOfInputBits));
    return *(this->conditions.end()-1);
}

Condition* TransitionBase::addCondition(Condition * condition,unsigned int position) {

    if (condition==NULL)
        throw new invalid_argument("Trying to add  a NULL condition");

    //-- Insert at position
    this->conditions.insert(this->conditions.begin()+position,condition);

    //-- Rereturn the pointer to condition
    return condition;

}

void TransitionBase::deleteCondition(unsigned  int id) {

    // Delete
    this->conditions.erase(this->conditions.begin() + id);

}

unsigned int TransitionBase::deleteCondition(Condition * conditionToDelete) {


    //-- Search Condition index
    unsigned int cIndex = 0;
    bool found = false;
    FOREACH_CONDITIONS(this)
        if (condition == conditionToDelete) {
            found = true;
            break;
        }
        cIndex++;
    END_FOREACH_CONDITIONS

    //-- Throw an exception if not found
    if (!found)
        throw new invalid_argument("Trying to delete an inexisting condition");

    //-- Delete
    this->conditions.erase(this->conditions.begin() + cIndex);

    //-- Return deleted index
    return cIndex;

}


vector<Trackpoint*>& TransitionBase::getTrackpoints() {

    return this->trackpoints;
}

Trackpoint * TransitionBase::appendTrackpoint(double posx, double posy) {

    // Add at the end
    Trackpoint * tp = new Trackpoint(posx, posy, this);
    tp->setTransition(this);
    this->trackpoints.push_back(tp);
    return tp;

}

Trackpoint * TransitionBase::appendTrackpoint(Trackpoint * tp) {

    // Add at the end
    trackpoints.push_back(tp);
    tp->setTransition(this);
    return tp;

}

Trackpoint * TransitionBase::addTrackpointBefore(Trackpoint * baseTrackpoint,
        double posx, double posy) {

    vector<Trackpoint*>::iterator ti;

    for (ti = trackpoints.begin(); ti != trackpoints.end(); ti++) {
        if ((*ti) == baseTrackpoint) {

            // Add after baseTrackpoint
            Trackpoint * tp = new Trackpoint(posx, posy, 0);
            tp->setTransition(this);
            this->trackpoints.insert(ti, tp);
            return tp;
        }
    }

    return baseTrackpoint;

}

Trackpoint * TransitionBase::addTrackpointBefore(Trackpoint * baseTrackpoint,
        Trackpoint *newTrackpoint) {

    //-- Look for baseTrackPoint
    vector<Trackpoint*>::iterator ti;
    for (ti = trackpoints.begin(); ti != trackpoints.end(); ti++) {
        if ((*ti)  == baseTrackpoint ) {

            // Add after baseTrackpoint
            trackpoints.insert(ti, newTrackpoint);
            newTrackpoint->setTransition(this);
            return newTrackpoint;
        }
    }

    return newTrackpoint;

}

Trackpoint * TransitionBase::addFirstTrackpoint(double posx, double posy) {

    // Add front
    Trackpoint * tp = new Trackpoint(posx, posy, this);
    tp->setTransition(this);
    this->trackpoints.insert(this->trackpoints.begin(),tp);
    return tp;
}

Trackpoint * TransitionBase::deleteTrackpoint(unsigned int id) {

    Trackpoint * trackpoint = this->trackpoints.at(id);
    this->trackpoints.erase(this->trackpoints.begin() + id);


    return trackpoint;

}


Trackpoint * TransitionBase::deleteTrackpoint(Trackpoint * trackpoint) {

    //-- look for trackpoint
    vector<Trackpoint*>::iterator ti;
    unsigned count = 0;
    for (ti = trackpoints.begin(); ti != trackpoints.end(); ti++) {

        Trackpoint * tp = *(ti.base());
        if (tp == trackpoint) {
            trackpoints.erase(ti);
            return trackpoint;

        }
        count++;
    }

    return NULL;
}
