/**
 * \file DeleteFSMAction.cpp
 *
 * 
 *
 */

// Includes
//--------------------

//-- Std
#include <iostream>
#include <ostream>
#include <sstream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>

//-- Core
#include <core/Fsm.h>

#include "DeleteFSMAction.h"

DeleteFSMAction::DeleteFSMAction(Fsm * item,QUndoCommand * parentCommand) : ObjectFocusedAction<Fsm>(item,parentCommand) {

}

DeleteFSMAction::~DeleteFSMAction() {

}

int DeleteFSMAction::id() {
    // Call parent
    return ObjectFocusedAction<Fsm>::id();
}
bool DeleteFSMAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Fsm>::mergeWith(command);
}
void DeleteFSMAction::redo(){
    // Call parent
    ObjectFocusedAction<Fsm>::redo();
}
void DeleteFSMAction::undo(){
    // Call parent
    ObjectFocusedAction<Fsm>::undo();
}


