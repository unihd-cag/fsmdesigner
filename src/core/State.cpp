/***********************************************************************
*                                                                      *
* (C) 2006, Frank Lemke, Computer Architecture Group,                  *
* University of Mannheim, Germany                                      *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                            *
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
#include <sstream>
#include <map>
#include <iostream>
using namespace std;

//-- Core
#include <core/Utils.h>


#include "State.h"

State::State(int numberOfOutputs)  : UniqueIDObject() {
  this->position.first = 10;
  this->position.second = 10;
  this->hlevel = 0;
  this->name = "State";
  this->output = "";
  this->output.assign(numberOfOutputs, '0');
  this->color = 4294901760;
  this->reset = false;
}

State::~State() {

}


void State::setName(string name) {

    this->name.clear();
    this->name.append(name);

}

string State::getName() {
    return this->name;
}

void State::setPosition(pair<double, double> position) {
    this->position = position;
}


pair<double, double> State::getPosition() {
    return this->position;
}

bool State::isReset() {
        return this->reset;
}

void State::setReset(bool reset) {
    this->reset = reset;
}

string State::getOutput() {
    return this->output;
}

void State::setOutput(string output) {
    this->output = output;
}

void State::addOutput(unsigned int value) {

    if (value > 1) {
        throw invalid_argument(string("Trying to add and output to a signal with a value different than 0 or 1 :")+Utils::itos(value));
    }

    this->output = this->output.append(Utils::itos(value));
}

unsigned int State::getColor() {
    return this->color;
}

void State::setColor(unsigned int newColor) {
    this->color = newColor;
}



void State::setOutputAt(int outputIndex, char val) {

    //-- Check argument
    if (outputIndex >= this->output.length()) {
        throw new invalid_argument(
                "Provided output index exceeds output length in setOutputAt");
    }

    //-- Set
    this->output[outputIndex] = val;

}

char State::getOutputAt(int outputIndex) {

    //-- Check argument
    if (outputIndex >= this->output.length()) {
        throw new invalid_argument(
                "Provided output index exceeds output length in getOutputAt ");
    }

    //-- Return
    return this->output[outputIndex];

}

list<Trans*>& State::getStartingTransitions() {
    return this->startingTransitions;
}


void State::addStartingTransition(Trans * transition) {
    this->startingTransitions.push_back(transition);
}

void State::removeStartingTransition(Trans * transition) {
    this->startingTransitions.remove(transition);
}
