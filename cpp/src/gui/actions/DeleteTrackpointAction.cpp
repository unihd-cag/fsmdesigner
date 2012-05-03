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

#include "DeleteTrackpointAction.h"

DeleteTrackpointAction::DeleteTrackpointAction(TrackpointItem * item,QUndoCommand * parentCommand) :ItemFocusedAction(item,parentCommand) {

}

DeleteTrackpointAction::~DeleteTrackpointAction() {

}

int DeleteTrackpointAction::id() {
    return this->item->getModel()->getTransition()->getId();
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
    //cout << "Redoing delete trackpoint with id: " << this->id() << endl;

    // Gui
    //--------------------

    //-- Save Previous/Next information
    this->previousItem = this->item->getStartItem();
    this->nextItem = this->item->getEndItem();

    //-- Call remove out of scene (Gui is ok after that)
    this->getRelatedScene()->removeItem(this->item);

    //-- FIXME Remove trackpoint from model
    //-- Remove function says who's next now. If no one, we can restore with append
    this->nextModel = this->item->getModel()->remove();
}

void DeleteTrackpointAction::undo(){

    //-- Reversing?
    if (this->isReversed()) {
        this->setReversed(false);
        this->redo();
        this->setReversed(true);
        return;
    }

    qDebug() << "undoing delete tp";

    // If Re-Adding, but already on the scene -> don't proceed
    //----------------
    if (this->item->scene()!=NULL)
        return;

    // Re-add Trackpoint to scene
    //-------------------------
    this->getRelatedScene()->addItem(this->item);

    // Re-add to model
    //----------------------
    if (this->item->getModel()!=NULL) {
        this->item->getModel()->getTransition()->appendTrackpoint(this->item->getModel());
    } else {
        this->item->getModel()->getTransition()->addTrackpointBefore(this->nextModel,this->item->getModel());
    }

    //cout << "Undoing deleted state" << endl;
}


