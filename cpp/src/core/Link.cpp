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
//------------------------

//-- Std
#include <utility>
#include <string>
#include <cstdlib>

using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
class State;

#include "Link.h"

Link::Link(State * targetState,double posx, double posy ) : UniqueIDObject() {

  this->position = make_pair(posx, posy);
  this->targetState = targetState;
  this->color = 0;

}

Link::~Link() {

}

State * Link::getTargetState() {
    return this->targetState;
}
void    Link::setTargetState(State * state) {
    this->targetState = state;
}

void Link::setColor(int color) {
    this->color = color;
}

int Link::getColor() {
    return this->color;
}

void Link::setPosition(pair<double,double> position) {
    this->position = position;
}

/**
 * @warning Not a reference! Modifying the result will not modify the source
 * @return Position pair (first = x, second = y)
 */
pair<double,double> Link::getPosition() {
    return this->position;
}
