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
        TransitionBase() {

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



