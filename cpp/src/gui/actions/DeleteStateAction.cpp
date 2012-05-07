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
#include <gui/items/JoinItem.h>
#include <gui/items/HyperTransition.h>
#include <gui/items/LinkArrival.h>

#include "DeleteStateAction.h"

DeleteStateAction::DeleteStateAction(StateItem * item,QUndoCommand * parentCommand) :ItemFocusedAction<StateItem>(item,parentCommand) {


    // Pre-delete :
    //  - all Transitions
    //  - all incoming hypertransitions
    //  - all incoming links
    //-------------------------------------

    //-- Outgoing
    while (!this->item->getOutgoingTransitions().isEmpty()) {
        this->item->getOutgoingTransitions().takeFirst()->remove(this);
    }

    //-- Incoming
    while (!this->item->getIncomingTransitions().isEmpty()) {



        Transline * t = this->item->getIncomingTransitions().takeFirst();


        //-- If coming from Link Arrival -> delete hypertransition
        if (t->getStartItem()->type()==LinkArrival::Type) {
            dynamic_cast<LinkArrival*>(t->getStartItem())->remove(this);
        }
        //-- If coming from Hypertransition -> delete hypertransition
        else if (t->getStartItem()->type()==HyperTransition::Type) {
            FSMGraphicsItem<>::toHyperTransition(t->getStartItem())->remove(this);

        }
        //-- If Incoming if also outgoing (loopback), then do not double remove
        else if (t->getModel()!=NULL && ((Trans*)t->getModel())->getStartState() != ((Trans*)t->getModel())->getEndState())
            t->remove(this);

        //-- if Coming from join, delete join
        else if (t->getStartItem()->type()==JoinItem::Type) {
            FSMGraphicsItem<>::toJoinItem(t->getStartItem())->remove(this);
        }



    }



}

DeleteStateAction::~DeleteStateAction() {

}

int DeleteStateAction::id() {
    return ItemFocusedAction<StateItem>::id();
}
bool DeleteStateAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void DeleteStateAction::redo(){


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
    ItemFocusedAction<StateItem>::undo();

}


