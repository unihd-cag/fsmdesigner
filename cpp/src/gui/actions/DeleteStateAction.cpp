/**
 * \file DeleteStateAction.cpp
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
#include <core/State.h>
#include <core/Trans.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>
#include <gui/actions/DeleteTransitionAction.h>

//-- Gui Items
#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>

#include "DeleteStateAction.h"

DeleteStateAction::DeleteStateAction(StateItem * item) :ItemFocusedAction(item) {

}

DeleteStateAction::~DeleteStateAction() {

}

int DeleteStateAction::id() {
    return this->item->getModel()->getId();
}
bool DeleteStateAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void DeleteStateAction::redo(){


    // Pre-delete all Transitions
    //-------------------------------------

    //-- Outgoing
    while (!this->item->getOutgoingTransitions().isEmpty()) {
        this->item->getOutgoingTransitions().takeFirst()->remove(this);
    }

    //-- Incoming
    while (!this->item->getIncomingTransitions().isEmpty()) {

        //-- If Incoming if also outgoing (loopback), then do not double remove
        Transline * t = this->item->getIncomingTransitions().takeFirst();
        if (t->getModel()!=NULL && t->getModel()->getStartState() != t->getModel()->getEndState())
            t->remove(this);

    }


    // Remove Children (transitions)
    //-------------------
    ItemFocusedAction<StateItem>::redo();

    // Remove from GUI
    //-----------------
    this->getRelatedScene()->removeItem(this->item);


    // Remove Model
    //----------------
    this->getRelatedScene()->getFsm()->deleteState(this->item->getModel());



}
void DeleteStateAction::undo(){

    // Restore Model
    //-----------------------
    this->getRelatedScene()->getFsm()->addState(this->item->getModel());

    // Restore GUI
    //-----------------
    this->getRelatedScene()->addItem(this->item);

    // Restore Children (transitions)
    //-------------------
    QUndoCommand::undo();

}


