/***********************************************************************
 *                                                                      *
 * (C) 2006, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

// Includes
//------------------
//-- Std
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>
using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
#include <core/Trackpoint.h>
#include <core/State.h>

#include "Trans.h"
Trans::Trans(State* start, State* end) :
        UniqueIDObject() {

    //-- Default values
    this->setStartState(start);
    this->setEndState(end);
    this->defaultTransition = false;
    this->color = 0;
    this->name = "trans_";

    this->textpos.first = 10;
    this->textpos.second = 10;
}

Trans::~Trans() {

}

pair<double, double>& Trans::getTextPosition() {
    return this->textpos;
}

void Trans::setTextPosition(double x, double y) {
    this->textpos.first = x;
    this->textpos.second = y;
}

int Trans::getColor() {
    return this->color;
}

string Trans::getName() {
    return this->name;
}

bool Trans::isDefault() {
    return this->defaultTransition;
}

void Trans::setColor(int color) {
    this->color = color;
}

void Trans::setDefault(bool defaultTransition) {
    this->defaultTransition = defaultTransition;
}

void Trans::setName(string name) {
    this->name = name;
}

void Trans::setStartState(State* state) {
    this->start = state;
}
State * Trans::getStartState() {
    return this->start;
}

void Trans::setEndState(State*state) {
    this->end = state;
}
State * Trans::getEndState() {
    return this->end;
}

vector<Condition*>& Trans::getConditions() {
    return this->cv;
}


Condition* Trans::getConditionByID(unsigned int id) {

    //-- Check
    if (id>=this->cv.size())
        throw invalid_argument("Trying to get a condition at an index > size()");

    //-- Find pointer
    return  *(this->cv.begin()+id);

}

Condition* Trans::addCondition(int numberOfInputBits) {

    cv.push_back(new Condition(numberOfInputBits));


    return *(cv.end()-1);
}

Condition* Trans::addCondition(Condition * condition,unsigned int position) {

    if (condition==NULL)
        throw new invalid_argument("Trying to add  a NULL condition");

    //-- Insert at position
    this->cv.insert(this->cv.begin()+position,condition);

    //-- Rereturn the pointer to condition
    return condition;

}

void Trans::deleteCondition(unsigned int id) {

    // Delete
    this->cv.erase(this->cv.begin() + id);

}

unsigned int Trans::deleteCondition(Condition * conditionToDelete) {


    //-- Search Condition index
    unsigned int cIndex = 0;
    bool found = false;
    FOREACH_TRANSITION_CONDITIONS(this)
        if (condition == conditionToDelete) {
            found = true;
            break;
        }
        cIndex++;
    END_FOREACH_TRANSITION_CONDITIONS

    //-- Throw an exception if not found
    if (!found)
        throw new invalid_argument("Trying to delete an inexisting condition");

    //-- Delete
    this->cv.erase(this->cv.begin() + cIndex);

    //-- Return deleted index
    return cIndex;

}


vector<Trackpoint*>& Trans::getTrackpoints() {

    return this->tl;
}

Trackpoint * Trans::appendTrackpoint(double posx, double posy) {

    // Add at the end
    Trackpoint * tp = new Trackpoint(posx, posy, 0);
    tp->setTransition(this);
    this->tl.push_back(tp);
    return tp;

}

Trackpoint * Trans::appendTrackpoint(Trackpoint * tp) {

    // Add at the end
    tl.push_back(tp);
    tp->setTransition(this);
    return tp;

}

Trackpoint * Trans::addTrackpointBefore(Trackpoint * baseTrackpoint,
        double posx, double posy) {

    vector<Trackpoint*>::iterator ti;

    for (ti = tl.begin(); ti != tl.end(); ti++) {
        if ((*ti) == baseTrackpoint) {

            // Add after baseTrackpoint
            Trackpoint * tp = new Trackpoint(posx, posy, 0);
            tp->setTransition(this);
            this->tl.insert(ti, tp);
            return tp;
        }
    }

    return baseTrackpoint;

}

Trackpoint * Trans::addTrackpointBefore(Trackpoint * baseTrackpoint,
        Trackpoint *newTrackpoint) {

    //-- Look for baseTrackPoint
    vector<Trackpoint*>::iterator ti;
    for (ti = tl.begin(); ti != tl.end(); ti++) {
        if ((*ti)  == baseTrackpoint ) {

            // Add after baseTrackpoint
            tl.insert(ti, newTrackpoint);
            newTrackpoint->setTransition(this);
            return newTrackpoint;
        }
    }

    return newTrackpoint;

}

Trackpoint * Trans::addFirstTrackpoint(double posx, double posy) {

    // Add front
    Trackpoint * tp = new Trackpoint(posx, posy, 0);
    tp->setTransition(this);
    this->tl.insert(this->tl.begin(),new Trackpoint(posx, posy, 0));
    return tp;
}

Trackpoint * Trans::deleteTrackpoint(unsigned int id) {

    Trackpoint * trackpoint = this->tl.at(id);
    this->tl.erase(this->tl.begin() + id);


    return trackpoint;

}

Trackpoint * Trans::deleteTrackpoint(Trackpoint * trackpoint) {

    //-- look for trackpoint
    vector<Trackpoint*>::iterator ti;
    unsigned count = 0;
    for (ti = tl.begin(); ti != tl.end(); ti++) {
        if ((*ti) == trackpoint)
            return this->deleteTrackpoint(count);
        count++;
    }

    return NULL;
}






