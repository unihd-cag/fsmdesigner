/*
 * FSMRelatedObject.cpp
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

// Includes
//-------------

//-- Std
#include <typeinfo>
#include <stdexcept>
using namespace std;

//-- Core
#include <core/Fsm.h>

#include "FSMRelatedObject.h"

FSMRelatedObject::FSMRelatedObject(Fsm * fsm) {

    //-- Record
	this->relatedFsm = fsm;

	//-- Verify it is not NULL
	if (this->relatedFsm==NULL)
	    throw invalid_argument(string("Tried to pass a NULL FSM to an FSM Related object: ")+ typeid(this).name());

}

FSMRelatedObject::~FSMRelatedObject() {
	// TODO Auto-generated destructor stub
}

void FSMRelatedObject::setRelatedFsm(Fsm * fsm) {
	this->relatedFsm = fsm;
}
