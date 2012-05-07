/**
 * \file DeleteLinkArrivalAction.cpp
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
#include <core/Link.h>
#include <core/Fsm.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Common
#include <common/SGC.h>

//-- Gui Items
#include <gui/items/LinkArrival.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>

#include "DeleteLinkArrivalAction.h"

DeleteLinkArrivalAction::DeleteLinkArrivalAction(LinkArrival * item,QUndoCommand * parentCommand) :ItemFocusedAction<LinkArrival>(item,parentCommand) {

    // If no related scene already -> Ignore this
    if (this->getRelatedScene()==NULL)
        return;

    // Also Delete all the LinkDepartures targeting this Arrival (The link indeed)
    //--------------------
    QList<LinkDeparture*> departures = this->getRelatedScene()->findLinkDepartures(this->item->getModel());
    while (!departures.isEmpty()) {

        // Take departure
        LinkDeparture * departure = departures.takeFirst();

        // Delete
        departure->remove(this);

    }


}

DeleteLinkArrivalAction::~DeleteLinkArrivalAction() {

}

int DeleteLinkArrivalAction::id() {
    // Call parent
    return ItemFocusedAction<LinkArrival>::id();
}
bool DeleteLinkArrivalAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<LinkArrival>::mergeWith(command);
}
void DeleteLinkArrivalAction::redo(){

    // Remove Children (the Link Departures)
    //-------------------
    ItemFocusedAction<LinkArrival>::redo();

    // Redo delete
    //--------------------------
    SGC::getInstance()->activate();

    //-- Delete from Scene
    this->getRelatedScene()->removeItem(this->item);

    //-- Delete from model
    this->getRelatedScene()->getFsm()->deleteLink(this->item->getModel());


    SGC::getInstance()->collect();


}
void DeleteLinkArrivalAction::undo(){


    // Replace element
    //--------------------------

    //-- Readd model
    this->getRelatedScene()->getFsm()->addLink(this->item->getModel());

    //-- Readd Gui
    this->getRelatedScene()->addItem(this->item);

    //-- Create transition Line to state
    Transline * transLine = new Transline(NULL, this->item,
            this->item->getTargetStateItem());
    this->getRelatedScene()->addItem(transLine);
    this->item->setNextTransline(transLine);


    // Replace Children (The LinkDepartures)
    //-------------------
    ItemFocusedAction<LinkArrival>::undo();
}


