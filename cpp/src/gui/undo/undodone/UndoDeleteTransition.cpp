/*
 * UndoDeleteTransition.cpp
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

#include <gui/common/SGC.h>

#include <gui/undo/FSMUndoCommand.h>


#include "UndoDeleteTransition.h"

UndoDeleteTransition::UndoDeleteTransition(Transline * transline,Trans * transitionModel,QUndoCommand * parent) : FSMUndoCommand(transline, transitionModel,parent) {

	//-- Inits
	this->startItem = NULL;
	this->endItem = NULL;

	//-- Update Text
	if (this->deletedGuiItem != NULL) {
		stringstream ss;
		ss << "Delete Transition: " << this->deletedGuiItem->getModel()->getName()
				<< endl;
		this->setText(ss.str().c_str());
	}

}

UndoDeleteTransition::~UndoDeleteTransition() {
	// TODO Auto-generated destructor stub
}

StateItem * UndoDeleteTransition::getStartItem() {
	return this->startItem;
}
void UndoDeleteTransition::setStartItem(StateItem * item) {
	this->startItem = item;
}

QGraphicsItem * UndoDeleteTransition::getEndItem() {
	return this->endItem;
}
void UndoDeleteTransition::setEndItem(QGraphicsItem * item) {
	this->endItem = item;
}

int UndoDeleteTransition::id() {

	int id = -1;

	if (this->startItem == NULL)
		id = this->deletedModel->getId();
	else
		id = this->startItem->getModel()->getId();

	//id = this->deletedTransition->getModel()->tid;

	cout << "Asked for transition ID: " << id << endl;

	return id;
}
bool UndoDeleteTransition::mergeWith(const QUndoCommand * command) {
	cout << "Merging with a State probably" << endl;
	return true;
}
void UndoDeleteTransition::redo() {



	//-- Do try to delete
	cout << "Redoing deleted transition with id: " << this->id() << endl;

	//-- Activate Garbage collector
	SGC::getInstance()->activate();



	// Remove from gui by propagating
	//----------------------

	//-- First find the start of the transition
	Transline * first = this->deletedGuiItem;
	qDebug() << "-- *I: first type: " << first->getStartItem();
	while (first->getStartItem()->type() != StateItem::Type) {
	//while (!FSMGraphicsItem<>::isStateItem(first->getStartItem())) {

		//-- Prev is a trackpoint, jump to trackpoint prev transition
		if (FSMGraphicsItem<>::isTrackPoint(first->getStartItem())) {
			first
					= FSMGraphicsItem<>::toTrackPoint(first->getStartItem())->getPreviousTransline();
		}

		qDebug() << "*I: first type: " << first->getStartItem()->type();
	}


	//-- get the associated state from first transline
	//	if (this->startItem==NULL) {
	this->startItem = FSMGraphicsItem<>::toStateItem(first->getStartItem());

	//	}

	//-- Forward and use Garbage Collector to safe delete trackpoints and such
	Transline * nextTransline = first;
	do {

		//-- If transline is linked to a trackpoint, delete the trackpoint
		if (FSMGraphicsItem<>::isTrackPoint(nextTransline->getEndItem())) {

			//-- Delete
			TrackpointItem * trackPoint = FSMGraphicsItem<>::toTrackPoint(
					nextTransline->getEndItem());
			this->getRelatedScene()->removeItem(trackPoint);
			SGC::getInstance()->requestDelete(trackPoint);

			//-- Next transline to consider is the prev of the next trackpoint, or none if no next trackpoint
			if (FSMGraphicsItem<>::isTrackPoint(trackPoint->getEndItem())) {
				nextTransline = FSMGraphicsItem<>::toTrackPoint(
						trackPoint->getEndItem())->getPreviousTransline();
			} else {

				//-- Record end State item if we are on one
				//				if (this->endItem==NULL && FSMGraphicsItem<>::isStateItem(trackPoint->getEndItem())) {
//				this->endItem = FSMGraphicsItem<>::toStateItem(
//						trackPoint->getEndItem());
				this->endItem = trackPoint->getEndItem();
				//				}

				//-- No more next transline
				nextTransline = NULL;

			}
		}
		//-- If transline if linked to a Link Item, delete it too if there are no more transline going to it then
		else if (nextTransline->getEndItem()->type()==LinkDeparture::Type) {

			//-- Remove Link Item
			//SGC::getInstance()->requestDelete(nextTransline->getEndItem());
			this->endItem = NULL;

			//-- Delete transline and stop there
			SGC::getInstance()->requestDelete(nextTransline);
			nextTransline = NULL;
		}
		else {
			//-- No more next transline because no trackpoint.
			//-- Only delete transline and end there
			this->endItem = nextTransline->getEndItem();
			SGC::getInstance()->requestDelete(nextTransline);
			nextTransline = NULL;

		}
	} while (nextTransline != NULL);


	// Remove text
	//------------------------
	QGraphicsItem * textItem = this->getRelatedScene()->itemAt(QPointF(this->deletedModel->getTextPosition().first,this->deletedModel->getTextPosition().second));
	if (textItem!=NULL) {
		this->getRelatedScene()->removeItem(textItem);
		SGC::getInstance()->requestDelete(textItem);
	}

	//-- Collect all removed items
	SGC::getInstance()->collect();

	// Remove Model
	//------------------------
	this->getRelatedScene()->getFsm()->deleteTrans(
		this->deletedModel->getId());


	//-- Remove itself
	/*this->setRelatedScene(dynamic_cast<Scene*>(this->deletedTransition->scene()));
	 this->deletedTransition->scene()->removeItem(this->deletedTransition);

	 // Remove itself from outgoing or incoming StateItem's list
	 //-----------------------------
	 if (this->deletedTransition->getStartItem() != NULL && this->deletedTransition->getStartItem()->type()
	 == StateItem::Type)
	 dynamic_cast<StateItem*> (this->deletedTransition->getStartItem())->removeOutgoingTransition(this->deletedTransition);
	 if (this->deletedTransition->getEndItem() != NULL && this->deletedTransition->getEndItem()->type()
	 == StateItem::Type)
	 dynamic_cast<StateItem*> (this->deletedTransition->getEndItem())->removeIncomingTransition(this->deletedTransition);
	 */
}

void UndoDeleteTransition::undo() {

	// Model
	//----------------------
	this->getRelatedScene()->getFsm()->addTrans(this->deletedModel);

	// Gui
	// Rebuild the complete transition
	//------------

	// Get Trans & start state
	Trans * transition = this->deletedModel;
	Transline * lastTransitionLine = NULL;

	State * currentState = this->startItem->getModel();
	StateItem * stateItem = this->startItem;

	Fsm * currentFSM = this->getRelatedScene()->getFsm();

	//-- Calculate the start and end as a trackpoints

	// Start
	Trackpoint * start = new Trackpoint(currentState->getPosition().first,
			currentState->getPosition().second, 0);

	// End
	State * endState = transition->getEndState();


	//-- Trace translines from start to end with trackpoints intermediaries
	Trackpoint * previous = start;
	QGraphicsItem * previousItem = stateItem;
	FOREACH_TRANSITION_TRACKPOINTS(transition)

		// Place trackpoint
		//----------------------

		//-- Trackpoint is a link
		if (trackpoint->isLink()) {

			//-- Create Item If there is none
			LinkDeparture * LinkDeparture = NULL;
			if (this->endItem!=NULL && this->endItem->type() == FSMGraphicsItem<>::LinkDeparture)
				LinkDeparture = dynamic_cast<LinkDeparture*>(this->endItem);
			else
				LinkDeparture = new LinkDeparture(trackpoint,previousItem);

			//-- Position in space and on Scene
			LinkDeparture->setPos(trackpoint->getPosition().first, trackpoint->getPosition().second);
			this->getRelatedScene()->addItem(LinkDeparture);

			//-- Stop going further by nulling previous thing
			previous = NULL;
			previousItem = NULL;

			//-- Record as End to place last transition line
			this->endItem = LinkDeparture;

			break;

		}
		//-- Trackpoint points to a join
		else if (trackpoint->getJoinID()>0) {

			//---- Don't add a trackpoint, but set End Item to the join
			//---------------------

			//-- Get Join, position, and find the graphic Item
			Join * join = currentFSM->getJoin(trackpoint->getJoinID());
			JoinItem* JoinItem = this->getRelatedScene()->findJoinItem(join);

			if (JoinItem==NULL) {
				qDebug() << "Looking for join point Graphics Item return NULL";
			}
			this->endItem = JoinItem;


		}
		//-- Normal case
		else {

			//-- Add trackpoint between previous and nothing (because we don't know yet)
			TrackpointItem * trackpointItem = new TrackpointItem(trackpoint,
					previousItem, NULL);
			trackpointItem->setPos(trackpoint->getPosition().first, trackpoint->getPosition().second);
			this->getRelatedScene()->addItem(trackpointItem);

			//-- If previous is a trackpoint, this one is the next of previous
			if (FSMGraphicsItem<>::isTrackPoint(previousItem)) {
				FSMGraphicsItem<>::toTrackPoint(previousItem)->setEndItem(
						trackpointItem);
			}

			//-- Record as previous
			previous = trackpoint;
			previousItem = trackpointItem;

		}
	END_FOREACH_TRANSITION_TRACKPOINTS


	//-- Place last transline between previous and end if we this
	// No previous means we don't want to trace between target and previous (like a link)
	if (previous != NULL) {

		//-- If Previous is a state, simply place a transline
		if (FSMGraphicsItem<>::isStateItem(previousItem)) {
			lastTransitionLine = new Transline(transition, previousItem,
					this->endItem);
			this->getRelatedScene()->addItem(lastTransitionLine);

		}
		//-- If previous is a trackpoint, the state is the end item
		else if (FSMGraphicsItem<>::isTrackPoint(previousItem)) {

			//-- Set state as next item of trackpoint
			FSMGraphicsItem<>::toTrackPoint(previousItem)->setEndItem(
					this->endItem);

			//-- The last transline is the next transline of the trackpoint then
			lastTransitionLine
					= FSMGraphicsItem<>::toTrackPoint(previousItem)->getNextTransline();

		}

		//-- Add transline text
		TranslineText * transitionLineText = new TranslineText(QString(
				transition->getName().c_str()),lastTransitionLine->getModel());
		transitionLineText->setPos(transition->getTextPosition().first,
				transition->getTextPosition().second);
		this->getRelatedScene()->addItem(transitionLineText);

	}

	this->deletedGuiItem = lastTransitionLine;


}
