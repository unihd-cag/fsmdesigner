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
//-----------------

//-- Std
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
#include <core/Condition.h>
#include <core/State.h>
#include <core/Trackpoint.h>

#include "Hypertrans.h"

Hypertrans::Hypertrans() :
        UniqueIDObject() {

    //-- Init
    this->color = 0;
    this->name="";
    this->position = make_pair(0,0);
    this->targetState = NULL;

}

Hypertrans::~Hypertrans() {

}


void Hypertrans::setColor(int color) {
    this->color = color;
}

int Hypertrans::getColor() {
    return this->color;
}

pair<double, double> Hypertrans::getPosition() {
    return this->position;
}

void Hypertrans::setPosition(pair<double, double> position) {
    this->position = position;
}

State* Hypertrans::getTargetState() {
    return this->targetState;
}
void Hypertrans::setTargetState(State* targetState) {
    this->targetState = targetState;
}

Hypertrans::TYPE Hypertrans::getType() {
    return this->type;
}


void Hypertrans::setType(Hypertrans::TYPE type) {
    this->type = type;
}

vector<Condition*>& Hypertrans::getConditions() {
    return this->conditions;
}

Condition* Hypertrans::getConditionByID(unsigned int id) {

    //-- Check
    if (id>=this->conditions.size())
        throw invalid_argument("Trying to get a condition at an index > size()");

    //-- Find pointer
    return  *(this->conditions.begin()+id);

}

Condition * Hypertrans::addCondition(int numberOfInputBits) {

    this->conditions.push_back(new Condition(numberOfInputBits));
    return *(this->conditions.end()-1);
}

void Hypertrans::deleteCondition(unsigned  int id) {

    // Delete
    this->conditions.erase(this->conditions.begin() + id);

}

unsigned int Hypertrans::deleteCondition(Condition * conditionToDelete) {


    //-- Search Condition index
    unsigned int cIndex = 0;
    bool found = false;
    FOREACH_HYPERTRANSITION_CONDITIONS(this)
        if (condition == conditionToDelete) {
            found = true;
            break;
        }
        cIndex++;
    END_FOREACH_HYPERTRANSITION_CONDITIONS

    //-- Throw an exception if not found
    if (!found)
        throw new invalid_argument("Trying to delete an inexisting condition");

    //-- Delete
    this->conditions.erase(this->conditions.begin() + cIndex);

    //-- Return deleted index
    return cIndex;

}


vector<Trackpoint*>& Hypertrans::getTrackpoints() {

    return this->tl;
}

Trackpoint * Hypertrans::appendTrackpoint(double posx, double posy) {

    // Add at the end
    Trackpoint * tp = new Trackpoint(posx, posy, 0);
    //tp->setTransition(this);
    this->tl.push_back(tp);
    return tp;

}

Trackpoint * Hypertrans::appendTrackpoint(Trackpoint * tp) {

    // Add at the end
    tl.push_back(tp);
    //tp->setTransition(this);
    return tp;

}

Trackpoint * Hypertrans::addTrackpointBefore(Trackpoint * baseTrackpoint,
        double posx, double posy) {

    vector<Trackpoint*>::iterator ti;

    for (ti = tl.begin(); ti != tl.end(); ti++) {
        if ((*ti) == baseTrackpoint) {

            // Add after baseTrackpoint
            Trackpoint * tp = new Trackpoint(posx, posy, 0);
           // tp->setTransition(this);
            this->tl.insert(ti, tp);
            return tp;
        }
    }

    return baseTrackpoint;

}

Trackpoint * Hypertrans::addTrackpointBefore(Trackpoint * baseTrackpoint,
        Trackpoint *newTrackpoint) {

    //-- Look for baseTrackPoint
    vector<Trackpoint*>::iterator ti;
    for (ti = tl.begin(); ti != tl.end(); ti++) {
        if ((*ti)  == baseTrackpoint ) {

            // Add after baseTrackpoint
            tl.insert(ti, newTrackpoint);
            //newTrackpoint->setTransition(this);
            return newTrackpoint;
        }
    }

    return newTrackpoint;

}

Trackpoint * Hypertrans::addFirstTrackpoint(double posx, double posy) {

    // Add front
    Trackpoint * tp = new Trackpoint(posx, posy, 0);
   // tp->setTransition(this);
    this->tl.insert(this->tl.begin(),new Trackpoint(posx, posy, 0));
    return tp;
}

Trackpoint * Hypertrans::deleteTrackpoint(unsigned int id) {

    Trackpoint * trackpoint = this->tl.at(id);
    this->tl.erase(this->tl.begin() + id);


    return trackpoint;

}


Trackpoint * Hypertrans::deleteTrackpoint(Trackpoint * trackpoint) {

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


