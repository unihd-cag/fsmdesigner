/*
 * TrackpointBaseItem.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

// Includes
//-----------------

//-- Qt
#include <QtGui>

//-- Core
#include <core/Trackpoint.h>
#include <core/Trans.h>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/Transline.h>
#include <gui/items/TrackpointItem.h>

#include "TrackpointBaseItem.h"

TrackpointBaseItem::TrackpointBaseItem(Trackpoint * model,QGraphicsItem * startItem,
		QGraphicsItem * endItem) {

	// Set Model
	this->setModel(model);

	// Variable initialisation
	//-------------------------------
	this->previousTransline = NULL;
	this->nextTransline = NULL;
	this->startItem = startItem;
	this->endItem = endItem;

}

TrackpointBaseItem::~TrackpointBaseItem() {
	// TODO Auto-generated destructor stub
}




void TrackpointBaseItem::setPreviousTransline(Transline * previousTransline) {

	//-- If the actual previous line doesn't start on a trackpoint, it is our own, so delete it
	/*if (this->previousTransline!=NULL && previousTransline!=NULL && FSMGraphicsItem<>::isTrackPoint(previousTransline->getStartItem()) && !FSMGraphicsItem<>::isTrackPoint(this->previousTransline->getStartItem())) {
		delete this->previousTransline;
	}*/

	this->previousTransline = previousTransline;
}

Transline * TrackpointBaseItem::getPreviousTransline(){
	return this->previousTransline;
}

void TrackpointBaseItem::setNextTransline(Transline * nextTransline){
	this->nextTransline = nextTransline;
}

Transline * TrackpointBaseItem::getNextTransline(){
	return this->nextTransline;
}


void TrackpointBaseItem::setStartItem(QGraphicsItem * item) {



	// Create a transline between this trackpoint and the Start Item
	// If the start is a trackpointm there is no need, because the previous will care about that
	//----------------
	if (FSMGraphicsItem<>::isTrackPoint(item)) {

		//-- Our start is a trackpoint, if it was not one previously, we can remove our previous
		if (!FSMGraphicsItem<>::isTrackPoint(this->getStartItem())) {
			this->getPreviousTransline()->setVisible(false);
			this->setPreviousTransline(NULL);
		}

		this->setPreviousTransline(FSMGraphicsItem<>::toTrackPoint(item)->getNextTransline());

	} else if (this->getPreviousTransline()!=NULL) {

		this->getPreviousTransline()->setStartItem(item);

	}
	//-- Create a transline but only if the item is not NULL
	else if(item!=NULL) {
		Transline * transline = new Transline(this->getModel()->getTransition(),item,dynamic_cast<QGraphicsItem*>(this));
		this->setPreviousTransline(transline);


		// Add to scene if not already, and we can
			if (this->getScene()!=NULL && this->getPreviousTransline()!=NULL && this->getPreviousTransline()->scene()==NULL) {
				this->getScene()->addItem(this->getPreviousTransline());
			}
	}



	// Record
	this->startItem = item;


}
QGraphicsItem * TrackpointBaseItem::getStartItem() {
	return this->startItem;
}

void TrackpointBaseItem::setEndItem(QGraphicsItem * item) {

	// Record
	//----------------
	this->endItem = item;


	// Create a transline between this trackpoint and the End Item
	//----------------

	//-- The next trackpoint has as previous line, this ones next
	if (this->getNextTransline()!=NULL) {
		this->getNextTransline()->setEndItem(item);
	} else  {
		Transline * transline = new Transline(this->getModel()->getTransition(),dynamic_cast<QGraphicsItem*>(this),this->getEndItem());
		this->setNextTransline(transline);

	}

	//-- Line should be invisible if not pointing to and end item
	this->getNextTransline()->setVisible(!(this->endItem==NULL));

	//-- Add to scene if not already, and we can
	if (this->getScene()!=NULL  && this->getNextTransline()->scene()==NULL) {
			this->getScene()->addItem(this->getNextTransline());
	}





}
QGraphicsItem * TrackpointBaseItem::getEndItem() {
	return this->endItem;
}


