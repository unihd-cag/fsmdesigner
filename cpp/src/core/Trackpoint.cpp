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

#include "Trackpoint.h"

Trackpoint::Trackpoint(double posx, double posy, Trans * transition) {

    this->setPosition(make_pair(posx, posy));
    this->setTargetLink(0);

    this->joinid = false;
    this->color = 0;
    this->transition = transition;
}

Trackpoint::~Trackpoint() {

}

Trans * Trackpoint::getTransition() {
    return this->transition;
}

void Trackpoint::setTransition(Trans * transition) {
    this->transition = transition;
}

pair<double, double> Trackpoint::getPosition() {
    return this->position;
}

void Trackpoint::setPosition(pair<double, double> newPosition) {
    this->position = newPosition;
}

int Trackpoint::getColor() {
    return this->color;
}
void Trackpoint::setColor(int color) {
    this->color = color;
}

void Trackpoint::setTargetLink(unsigned int targetLink) {
    this->lid = targetLink;
    this->link = this->lid == 0 ? false : true;
}

unsigned int Trackpoint::getTargetLink() {
    return this->lid;
}

bool Trackpoint::isLink() {
    return this->link;
}

void Trackpoint::setJoinID(unsigned int joinId) {
    this->joinid = joinId;
}

unsigned int Trackpoint::getJoinID() {
    return this->joinid;
}

Trackpoint * Trackpoint::remove() {
    return this->getTransition()->deleteTrackpoint(this);
}

