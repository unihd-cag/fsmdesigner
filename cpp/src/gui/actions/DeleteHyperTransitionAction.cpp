/**
 * \file DeleteHyperTransitionAction.cpp
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
#include <gui/actions/ItemFocusedAction.h>

//-- Core
#include <core/Fsm.h>
#include <core/Hypertrans.h>

//-- Gui Items
#include <gui/items/HyperTransition.h>
#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>

#include "DeleteHyperTransitionAction.h"

DeleteHyperTransitionAction::DeleteHyperTransitionAction(HyperTransition * item,QUndoCommand * parentCommand) :ItemFocusedAction<HyperTransition>(item,parentCommand) {

}

DeleteHyperTransitionAction::~DeleteHyperTransitionAction() {

}

int DeleteHyperTransitionAction::id() {

    // Call parent
    return ItemFocusedAction<HyperTransition>::id();

}
bool DeleteHyperTransitionAction::mergeWith(const QUndoCommand * command) {

    // Call parent
    return ItemFocusedAction<HyperTransition>::mergeWith(command);

}
void DeleteHyperTransitionAction::redo(){

    // Call parent
    ItemFocusedAction<HyperTransition>::redo();

    // Delete Model
    //--------------------
    this->getRelatedScene()->getFsm()->deleteHypertrans(this->item->getModel());

    // Remove from Scene
    //--------------
    this->relatedScene->removeItem(this->item);


}
void DeleteHyperTransitionAction::undo(){

    // Read model
    //-------------------
    if (!this->item->getModel()->isIdSet())
        this->relatedScene->getFsm()->addHypertrans(this->item->getModel());

    // Readd GUI Item
    //------------------------
    this->relatedScene->addItem(this->item);

    // Find back the target state
    //-------------
    StateItem * stateItem = this->relatedScene->findStateItem(this->item->getModel()->getTargetState());

    // Readd the transition line
    //----------
    Transline * transline = new Transline(NULL,this->item,stateItem);
    this->relatedScene->addItem(transline);
   // transline->preparePath();

    // Call parent
    ItemFocusedAction<HyperTransition>::undo();

}


