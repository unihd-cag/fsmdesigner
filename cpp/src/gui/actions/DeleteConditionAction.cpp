/**
 * \file DeleteConditionAction.cpp
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
#include <core/TransitionBase.h>
#include <core/Condition.h>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>


//-- Gui Items
#include <core/Condition.h>

#include "DeleteConditionAction.h"

DeleteConditionAction::DeleteConditionAction(TransitionBase * transition,Condition * object,QUndoCommand * parentCommand) :ObjectFocusedAction<Condition>(object,parentCommand) {
    this->object = object;
    this->transition = transition;
    this->position = 0;
}

DeleteConditionAction::~DeleteConditionAction() {

}

int DeleteConditionAction::id() {
    // Call parent
    return ObjectFocusedAction<Condition>::id();
}
bool DeleteConditionAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Condition>::mergeWith(command);
}
void DeleteConditionAction::redo(){

    //-- Remove and save position
    this->position = this->transition->deleteCondition(this->object);


}
void DeleteConditionAction::undo(){


    //-- Readd
    this->transition->addCondition(this->object,this->position);


}


