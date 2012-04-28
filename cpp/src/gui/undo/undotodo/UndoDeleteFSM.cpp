/*
 * UndoDeleteFSM.cpp
 *
 *  Created on: Jul 29, 2011
 *      Author: rleys
 */

// Includes
//-------------

//-- Core
#include <core/Project.h>
#include <core/Fsm.h>

//-- Qt
#include <QtGui>
#include <QtCore>


#include "UndoDeleteFSM.h"

UndoDeleteFSM::UndoDeleteFSM(Project * project,Fsm * fsmToDelete,QUndoCommand * parent) : QUndoCommand(parent){
	this->project= project;
	this->fsmToDelete = fsmToDelete;

}

UndoDeleteFSM::~UndoDeleteFSM() {
	// TODO Auto-generated destructor stub
}


int		UndoDeleteFSM::id (){

	return this->fsmToDelete->getFsmID();
}

bool	UndoDeleteFSM::mergeWith ( const QUndoCommand * command ){

}

void	UndoDeleteFSM::redo (){

	//-- use project to delete the fsm
	this->project->deleteFSM(this->fsmToDelete);

}

void 	UndoDeleteFSM::undo () {

	//-- Readd FSM
	this->project->addFSM(this->fsmToDelete);


}
