/**
 * \file DeleteLinkDepartureAction.cpp
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
#include <gui/actions/DeleteTransitionAction.h>

//-- Gui Items
#include <gui/items/LinkDeparture.h>
#include <gui/items/Transline.h>

#include "DeleteLinkDepartureAction.h"

DeleteLinkDepartureAction::DeleteLinkDepartureAction(LinkDeparture * item,QUndoCommand * parentCommand) :ItemFocusedAction<LinkDeparture>(item,parentCommand) {


    // Rely on a Transition Deletion action
    //----------------------
    DeleteTransitionAction * deleteTransition = new DeleteTransitionAction(this->item->getPreviousTransline(),this);




}

DeleteLinkDepartureAction::~DeleteLinkDepartureAction() {

}

int DeleteLinkDepartureAction::id() {
    // Call parent
    return ItemFocusedAction<LinkDeparture>::id();
}
bool DeleteLinkDepartureAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<LinkDeparture>::mergeWith(command);
}
void DeleteLinkDepartureAction::redo(){

    // Call parent
    ItemFocusedAction<LinkDeparture>::redo();
}
void DeleteLinkDepartureAction::undo(){


    // Call parent
    ItemFocusedAction<LinkDeparture>::undo();
}


