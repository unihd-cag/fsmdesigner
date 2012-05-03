/**
 * \file DeleteTrackpointAction.cpp
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
#include <core/Trackpoint.h>
#include <core/Trans.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
#include <gui/items/TrackpointItem.h>
#include <gui/items/Transline.h>

#include "DeleteTrackpointAction.h"

DeleteTrackpointAction::DeleteTrackpointAction(TrackpointItem * item,QUndoCommand * parentCommand) :ItemFocusedAction(item,parentCommand) {

}

DeleteTrackpointAction::~DeleteTrackpointAction() {

}

int DeleteTrackpointAction::id() {
    return ItemFocusedAction<TrackpointItem>::id();
}
bool DeleteTrackpointAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<TrackpointItem>::mergeWith(command);
}
void DeleteTrackpointAction::redo(){

    //-- Reversing?
    if (this->isReversed()) {
        this->setReversed(false);
        this->undo();
        this->setReversed(true);
        return;
    }

    //-- Do try to delete

    // Gui
    //--------------------

    //-- Save Previous/Next information
    this->previousItem = this->item->getStartItem();
    this->nextItem = this->item->getEndItem();

    //-- Call remove out of scene (Gui is ok after that)
    this->getRelatedScene()->removeItem(this->item);

    TransitionBase * transition =  this->item->getModel()->getTransition();

    //-- FIXME Remove trackpoint from model
    //-- Remove function says who's next now. If no one, we can restore with append
    this->nextModel = this->item->getModel()->remove();

    // IF there are no trackpoints anymore, re add a transition line
   //---------------
   if (transition->getTrackpoints().size()==0) {

       this->getRelatedScene()->addItem(new Transline(transition,this->previousItem,this->nextItem));

   }

}

void DeleteTrackpointAction::undo(){

    //-- Reversing?
    if (this->isReversed()) {
        this->setReversed(false);
        this->redo();
        this->setReversed(true);
        return;
    }

    // If Re-Adding, but already on the scene -> don't proceed
    //----------------
    if (this->item->scene()!=NULL)
        return;

    // Re-add to model
    //----------------------
    if (this->item->getModel()!=NULL) {
        this->item->getModel()->getTransition()->appendTrackpoint(this->item->getModel());
    } else {
        this->item->getModel()->getTransition()->addTrackpointBefore(this->nextModel,this->item->getModel());
    }

    // If we have trackpoint here, there was probably a transline before -> remove it

    // Re-add Trackpoint to scene
    //-------------------------
    this->getRelatedScene()->addItem(this->item);



    //cout << "Undoing deleted state" << endl;
}


