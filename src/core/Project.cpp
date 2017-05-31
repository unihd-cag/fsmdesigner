/*
 * Project.cpp
 *
 *  Created on: May 20, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <sstream>
#include <exception>
#include <stdexcept>

using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/Fsm.h>

#include "Project.h"

Project::Project() {


	// Default values
	//--------------
	this->selectedFSM = NULL;
	this->setName(string("New Project"));
}

Project::~Project() {
	// TODO Auto-generated destructor stub
}

QList<Fsm*> Project::getFSMs() {

	return this->fsmList;

}

Fsm * Project::findFSM(string searchedName) {

	//-- Go Through FSMs
	for (QList<Fsm*>::iterator it = this->fsmList.begin(); it != this->fsmList.end(); it++) {

		// Get FSM
		Fsm * currentFsm = *it;

		// Check it is the right one
		if (currentFsm->getFsmName() == searchedName) {
			return currentFsm;
		}

	}

	return NULL;
}

Fsm * Project::findFSM(int searchedId) {

	//-- Go Through FSMs
	for (QList<Fsm*>::iterator it = this->fsmList.begin(); it != this->fsmList.end(); it++) {

		// Get FSM
		Fsm * currentFsm = *it;

		// Check it is the right one
		if (currentFsm->getFsmID() == searchedId) {
			return currentFsm;
		}

	}

	return NULL;
}

Fsm * Project::addFSM() {

	//-- Create and FSM
	// Id is just managed by incrementing the total number of FSMS
	Fsm * newFSM = new Fsm(this->fsmList.size() + 1);
	newFSM->setProject(this);
	this->fsmList.append(newFSM);

	return newFSM;
}

Fsm * Project::addFSM(Fsm * newFsm) {

	//-- Add an FSM
	// Id is just managed by incrementing the total number of FSMS
	newFsm->setProject(this);
	newFsm->setId(this->fsmList.size() + 1);
	this->fsmList.append(newFsm);

	return newFsm;
}

void Project::deleteFSM(int id) {

	//-- Find an FSM
	Fsm * fsmToDelete = this->findFSM(id);
	if (fsmToDelete!=NULL) {
		this->deleteFSM(fsmToDelete);
	} else {
		throw new invalid_argument("Deleting FSM with provided id if failed, because the FSM was not found");
	}
}


void Project::deleteFSM(Fsm * fsmToDelete) {

	//-- Verify FSM exists
	if (this->fsmList.contains(fsmToDelete)) {
		this->fsmList.removeOne(fsmToDelete);
		fsmToDelete->setProject(NULL);
	} else {
		throw new invalid_argument("Deleting FSM failed, because the FSM does not exist");
	}


}
