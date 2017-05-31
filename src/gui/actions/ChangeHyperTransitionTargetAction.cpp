/**
 * \file ChangeHyperTransitionTargetAction.cpp
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
#include <core/Hypertrans.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
#include <gui/items/HyperTransition.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>


#include "ChangeHyperTransitionTargetAction.h"

ChangeHyperTransitionTargetAction::ChangeHyperTransitionTargetAction(State * targetState,HyperTransition * item,QUndoCommand * parentCommand) :ItemFocusedAction<HyperTransition>(item,parentCommand) {
    this->targetState = targetState;
}

ChangeHyperTransitionTargetAction::~ChangeHyperTransitionTargetAction() {

}

int ChangeHyperTransitionTargetAction::id() {
    // Call parent
    return ItemFocusedAction<HyperTransition>::id();
}
bool ChangeHyperTransitionTargetAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<HyperTransition>::mergeWith(command);
}
void ChangeHyperTransitionTargetAction::redo(){

    //-- Save Actual End State
    State * actualEndState = this->item->getModel()->getTargetState();

    //-- Set Target state on model
    this->item->getModel()->setTargetState(this->targetState);

    //-- Set EndItem on outgoing transition
    this->item->getOutgoingTransition()->setEndItem(this->getRelatedScene()->findStateItem(this->targetState));

    //-- Record previous StateItem
    this->targetState = actualEndState;

}
void ChangeHyperTransitionTargetAction::undo(){

    //-- Save Actual End State
    State * actualEndState = this->item->getModel()->getTargetState();

    //-- Set Target state on model
    this->item->getModel()->setTargetState(this->targetState);

    //-- Set EndItem on outgoing transition
    this->item->getOutgoingTransition()->setEndItem(this->getRelatedScene()->findStateItem(this->targetState));

    //-- Record previous StateItem
    this->targetState = actualEndState;
}


