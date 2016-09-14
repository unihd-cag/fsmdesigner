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
//---------------
//-- Std
#include <utility>
#include <string>
#include <cstdlib>

using namespace std;

//-- Core
#include <core/Trans.h>
#include <core/Join.h>
#include <core/Link.h>

#include "Trackpoint.h"

Trackpoint::Trackpoint(double posx, double posy, TransitionBase * transition) {

    this->setPosition(make_pair(posx, posy));
    this->setTargetLink(NULL);


    this->join = NULL;
    this->color = 0;
    this->transition = transition;
}

Trackpoint::~Trackpoint() {

}

TransitionBase * Trackpoint::getTransition() {
    return this->transition;
}

void Trackpoint::setTransition(TransitionBase * transition) {
    this->transition = transition;
}

pair<double, double> Trackpoint::getPosition() {
    return this->position;
}

void Trackpoint::setPosition(pair<double, double> newPosition) {
    this->position = newPosition;
}

unsigned int Trackpoint::getColor() {
    return this->color;
}
void Trackpoint::setColor(unsigned int color) {
    this->color = color;
}

void Trackpoint::setTargetLink(Link* targetLink) {
    this->targetLink = targetLink;
}

Link* Trackpoint::getTargetLink() {
    return this->targetLink;
}

bool Trackpoint::isLink() {
    return this->targetLink==NULL ? false : true;
}

void Trackpoint::setJoin(Join* join) {
    this->join = join;
}

Join * Trackpoint::getJoin() {
    return this->join;
}

Trackpoint * Trackpoint::remove() {
    return this->getTransition()->deleteTrackpoint(this);
}

