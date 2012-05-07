/**
 * \file DeleteJoinAction.cpp
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
#include <core/Join.h>
#include <core/Trackpoint.h>
#include <core/Trans.h>
#include <core/Fsm.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
#include <gui/items/JoinItem.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>
#include "DeleteJoinAction.h"

DeleteJoinAction::DeleteJoinAction(JoinItem * item,QUndoCommand * parentCommand) :ItemFocusedAction<JoinItem>(item,parentCommand) {

    // Delete All transitions that target this Join Item
    //------------
//    QList<Transline*> incomingTransitions = this->item->getIncomingTransitions();
//    for (int i = 0 ; i < incomingTransitions.size();i++) {
//        incomingTransitions.at(i)->remove(this);
//    }

    while (!this->item->getIncomingTransitions().isEmpty()) {
        qDebug() << "Removing incoming line";
        this->item->getIncomingTransitions().takeFirst()->remove(this);
    }

}

DeleteJoinAction::~DeleteJoinAction() {

}

int DeleteJoinAction::id() {
    // Call parent
    return ItemFocusedAction<JoinItem>::id();
}
bool DeleteJoinAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<JoinItem>::mergeWith(command);
}
void DeleteJoinAction::redo(){



    // Call parent
    ItemFocusedAction<JoinItem>::redo();

    //-- Remove from GUI
    this->getRelatedScene()->removeItem(this->item);
    if (this->item->getOutgoingTransition()!=NULL) {
        this->getRelatedScene()->removeItem(this->item->getOutgoingTransition());
        delete this->item->getOutgoingTransition();
    }


    //-- Remove from Model
    this->getRelatedScene()->getFsm()->deleteJoin(this->item->getModel());


}
void DeleteJoinAction::undo(){


    // Readd GUI Item to the scene
    //---------------------------------
    this->getRelatedScene()->addItem(this->item);

    //-- Readd the model
    //------------------
    this->getRelatedScene()->getFsm()->addJoin(this->item->getModel());

    // Rebuild the transition from Join Point to end item
    //------------

    //-- Calculate the start and end as a trackpoints

    // Start
    Trackpoint * start = new Trackpoint(this->item->getModel()->getPosx(),
            this->item->getModel()->getPosy(), 0);

    // End
    StateItem * endItem = this->getRelatedScene()->findStateItem(this->item->getModel()->getTargetState());

    // If no end item, don't place any transitions
    if (endItem==NULL) {
        qDebug() << "Could not find StateItem to reach from Model";
        return;
    }

    //-- Trace translines from start to end with trackpoints intermediaries
    //Trackpoint * previous = start;
    QGraphicsItem * previousItem = this->item;
    list<Trackpoint*> trackPoints = this->item->getModel()->getTrackpoints();
    for (list<Trackpoint*>::iterator tpi = trackPoints.begin(); tpi
            != trackPoints.end(); tpi++) {

        // Place trackpoint
        //----------------------

        //-- Trackpoint is a link
        if ((*tpi)->isLink()) {

            // Create Item
            LinkDeparture * linkDeparture = new LinkDeparture((*tpi),previousItem);
            linkDeparture->setPos((*tpi)->getPosition().first, (*tpi)->getPosition().second);
            this->getRelatedScene()->addItem(linkDeparture);

            // Don't Record as previous to stop
            //previous = NULL;
            previousItem = NULL;
            break;

        } else {

            //-- Add trackpoint between previous and nothing (because we don't know yet)
            TrackpointItem * trackpointItem = new TrackpointItem((*tpi),
                    previousItem, NULL);
            trackpointItem->setPos((*tpi)->getPosition().first, (*tpi)->getPosition().second);
            this->getRelatedScene()->addItem(trackpointItem);

            //-- If previous is a trackpoint, this one is the next of previous
            if (FSMGraphicsItem<>::isTrackPoint(previousItem)) {
                FSMGraphicsItem<>::toTrackPoint(previousItem)->setEndItem(
                        trackpointItem);
            }

            //-- Record as previous
            //previous = (*tpi);
            previousItem = trackpointItem;

        }
    } // END OF Trackpoints

    //-- Place last transline between previous and end if we this
    Transline * lastTransitionLine = NULL;
    // No previous means we don't want to trace between target and previous (like a link)
    if (previousItem != NULL) {


        //-- If Previous is a JoinItem, simply place a transline
        if (FSMGraphicsItem<>::isJoinItem(previousItem)) {
            lastTransitionLine = new Transline(NULL, previousItem,
                    endItem);
            this->getRelatedScene()->addItem(lastTransitionLine);
        }
        //-- If previous is a trackpoint, the state is the end item
        else if (FSMGraphicsItem<>::isTrackPoint(previousItem)) {

            //-- Set state as next item of trackpoint
            FSMGraphicsItem<>::toTrackPoint(previousItem)->setEndItem(
                    endItem);

            //-- The last transline is the next transline of the trackpoint then
            lastTransitionLine
                    = FSMGraphicsItem<>::toTrackPoint(previousItem)->getNextTransline();

        }

    }

    // Call parent
    ItemFocusedAction<JoinItem>::undo();

}


