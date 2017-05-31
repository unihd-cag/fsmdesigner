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
        TransitionBase() {

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



bool Trans::isDefault() {
    return this->defaultTransition;
}

void Trans::setColor(int color) {
    this->color = color;
}

void Trans::setDefault(bool defaultTransition) {
    this->defaultTransition = defaultTransition;
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






