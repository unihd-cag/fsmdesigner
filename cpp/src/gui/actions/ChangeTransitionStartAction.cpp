/**
 * \file ChangeTransitionStartAction.cpp
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
#include <core/State.h>
#include <core/Trans.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>
#include <gui/actions/DeleteTransitionAction.h>

//-- Gui Items
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>

#include "ChangeTransitionStartAction.h"

ChangeTransitionStartAction::ChangeTransitionStartAction(State * targetState,Transline * item,QUndoCommand * parentCommand) :ItemFocusedAction<Transline>(item,parentCommand) {
    this->targetState = targetState;
}

ChangeTransitionStartAction::~ChangeTransitionStartAction() {

}

int ChangeTransitionStartAction::id() {
    // Call parent
    return ItemFocusedAction<Transline>::id();
}
bool ChangeTransitionStartAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<Transline>::mergeWith(command);
}
void ChangeTransitionStartAction::redo(){

    //-- Remove
    DeleteTransitionAction * deleteTransition = new DeleteTransitionAction(this->item);
    deleteTransition->redo();
    this->item = deleteTransition->getItem();

    //-- Save Actual End State
    State * actualStartState = this->item->getModel()->getStartState();

    //-- Set Target state on model
    this->item->getModel()->setStartState(this->targetState);

    //-- Readd
    deleteTransition->undo();
    this->item = deleteTransition->getItem();

    //-- Record previous StateItem
    this->targetState = actualStartState;


}
void ChangeTransitionStartAction::undo(){

    //-- Remove
    DeleteTransitionAction * deleteTransition = new DeleteTransitionAction(this->item);
    deleteTransition->redo();
    this->item = deleteTransition->getItem();

    //-- Save Actual End State
    State * actualStartState = this->item->getModel()->getStartState();

    //-- Set Target state on model
    this->item->getModel()->setStartState(this->targetState);

    //-- Readd
    deleteTransition->undo();
    this->item = deleteTransition->getItem();

    //-- Record previous StateItem
    this->targetState = actualStartState;
}


