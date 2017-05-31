/**
 * \file DeleteInputAction.cpp
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


#include "DeleteInputAction.h"

DeleteInputAction::DeleteInputAction(int position,Fsm * object,QUndoCommand * parentCommand,QTreeView * relatedWidget) : ObjectFocusedAction<Fsm>(object,parentCommand,relatedWidget) {
    this->position = position;


}

DeleteInputAction::~DeleteInputAction() {

}

int DeleteInputAction::id() {
    // Call parent
    return ObjectFocusedAction<Fsm>::id();
}
bool DeleteInputAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Fsm>::mergeWith(command);
}
void DeleteInputAction::redo(){

    // Save name and remove
    this->name = this->object->getInputName(this->position);
    this->object->deleteInput(this->position);


    dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();
}
void DeleteInputAction::undo(){


    // Re-Add and change position (will be the end now)
    this->object->addInput(this->name);
    this->position = this->object->getNumberOfInputs()-1;


    dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();
}


