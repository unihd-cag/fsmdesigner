/**
 * \file DeleteOutputAction.cpp
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

//-- Core
#include <core/Fsm.h>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>


#include "DeleteOutputAction.h"

DeleteOutputAction::DeleteOutputAction(int position,Fsm * object,QUndoCommand * parentCommand,QTreeView * relatedWidget) : ObjectFocusedAction<Fsm>(object,parentCommand,relatedWidget) {
    this->position = position;
}

DeleteOutputAction::~DeleteOutputAction() {

}

int DeleteOutputAction::id() {
    // Call parent
    return ObjectFocusedAction<Fsm>::id();
}
bool DeleteOutputAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Fsm>::mergeWith(command);
}
void DeleteOutputAction::redo(){

    // Save name and remove
    this->name = this->object->getOutputName(this->position);
    this->object->deleteOutput(this->position);


    dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();

}
void DeleteOutputAction::undo(){

    // Re-Add and change position (will be the end now)
    this->object->addOutput(this->name);
    this->position = this->object->getNumberOfOutputs()-1;


    dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();
}


