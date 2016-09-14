/**
 * \file ChangeTransitionEndAction.cpp
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

#include "ChangeTransitionEndAction.h"

ChangeTransitionEndAction::ChangeTransitionEndAction(State * targetState,Transline * item,QUndoCommand * parentCommand) :ItemFocusedAction<Transline>(item,parentCommand) {

    this->targetState = targetState;



}

ChangeTransitionEndAction::~ChangeTransitionEndAction() {

}

int ChangeTransitionEndAction::id() {
    // Call parent
    return ItemFocusedAction<Transline>::id();
}
bool ChangeTransitionEndAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<Transline>::mergeWith(command);
}
void ChangeTransitionEndAction::redo(){


    //-- Remove
    DeleteTransitionAction * deleteTransition = new DeleteTransitionAction(this->item);
    deleteTransition->redo();
    this->item = deleteTransition->getItem();

    //-- Save Actual End State
    State * actualEndState = ((Trans*)this->item->getModel())->getEndState();

    //-- Set Target state on model
    ((Trans*)this->item->getModel())->setEndState(this->targetState);

    //-- Readd
    deleteTransition->undo();
    this->item = deleteTransition->getItem();

    //-- Record previous StateItem
    this->targetState = actualEndState;


}
void ChangeTransitionEndAction::undo(){

    //-- Remove
    DeleteTransitionAction * deleteTransition = new DeleteTransitionAction(this->item);
    deleteTransition->redo();
    this->item = deleteTransition->getItem();

    //-- Save Actual End State
    State * actualEndState = ((Trans*)this->item->getModel())->getEndState();

    //-- Set Target state on model
    ((Trans*)this->item->getModel())->setEndState(this->targetState);

    //-- Readd
    deleteTransition->undo();
    this->item = deleteTransition->getItem();

    //-- Record previous StateItem
    this->targetState = actualEndState;



}


