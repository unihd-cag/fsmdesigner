/*
 * UndoDeleteJoin.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: rleys
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
#include <core/Trans.h>
#include <core/State.h>
#include <core/Trackpoint.h>
#include <core/Join.h>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>
#include <gui/scene/Scene.h>
#include <gui/items/Transline.h>
#include <gui/items/TranslineText.h>
#include <gui/items/StateItem.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/JoinItem.h>

#include <gui/undo/FSMUndoCommand.h>

#include "UndoDeleteJoin.h"

UndoDeleteJoin::UndoDeleteJoin(JoinItem * transline,Join * transitionModel,QUndoCommand * parent) : FSMUndoCommand(transline, transitionModel,parent) {

	//-- Inits
	this->endItem = NULL;

	//-- Update Text
	if (this->deletedGuiItem != NULL) {
		stringstream ss;
		ss << "Delete Join Point " << endl;
		this->setText(ss.str().c_str());
	}

}

UndoDeleteJoin::~UndoDeleteJoin() {
	// TODO Auto-generated destructor stub
}


QGraphicsItem * UndoDeleteJoin::getEndItem() {
	return this->endItem;
}
void UndoDeleteJoin::setEndItem(QGraphicsItem * item) {
	this->endItem = item;
}

int UndoDeleteJoin::id() {

	return this->deletedModel->getId();
}
bool UndoDeleteJoin::mergeWith(const QUndoCommand * command) {

	return true;
}
void UndoDeleteJoin::redo() {

	//-- Do try to delete
	cout << "Redoing delete Join with id: " << this->id() << endl;


	//-- Deleted model gets a -1 as ID

}


void UndoDeleteJoin::undo() {

	qDebug() << "Replacing join point";

	// Model
	//----------------------
	this->getRelatedScene()->getFsm()->addJoin(this->deletedModel);

	// Gui


	// Readd GUI Item to the scene
	//---------------------------------
	this->getRelatedScene()->addItem(this->deletedGuiItem);

	// Rebuild the transition from Join Point to end item
	//------------

	//-- Calculate the start and end as a trackpoints

	// Start
	Trackpoint * start = new Trackpoint(this->deletedModel->getPosx(),
			this->deletedModel->getPosy(), 0);

	// End
	QGraphicsItem * endItem = this->getEndItem();

	// If no end item, don't place any transitions
	if (endItem==NULL)
		return;

	//-- Trace translines from start to end with trackpoints intermediaries
	Trackpoint * previous = start;
	QGraphicsItem * previousItem = this->deletedGuiItem;
	list<Trackpoint*> trackPoints = this->deletedModel->getTrackpoints();
	for (list<Trackpoint*>::iterator tpi = trackPoints.begin(); tpi
			!= trackPoints.end(); tpi++) {

		// Place trackpoint
		//----------------------

		//-- Trackpoint is a link
		if ((*tpi)->isLink()) {

			// Create Item
			LinkDeparture * LinkDeparture = new LinkDeparture((*tpi),previousItem);
			LinkDeparture->setPos((*tpi)->getPosition().first, (*tpi)->getPosition().second);
			this->getRelatedScene()->addItem(LinkDeparture);

			// Don't Record as previous to stop
			previous = NULL;
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
			previous = (*tpi);
			previousItem = trackpointItem;

		}
	} // END OF Trackpoints

	//-- Place last transline between previous and end if we this
	Transline * lastTransitionLine = NULL;
	// No previous means we don't want to trace between target and previous (like a link)
	if (previous != NULL) {


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

}
