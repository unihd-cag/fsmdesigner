/*
 * UndoDeleteTrackpoint.cpp
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

//-- Core
#include <core/Trackpoint.h>
#include <core/Trans.h>

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/scene/SceneRelatedObject.h>
#include <gui/undo/FSMUndoCommand.h>
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/Transline.h>


#include "UndoDeleteTrackpoint.h"

UndoDeleteTrackpoint::UndoDeleteTrackpoint(TrackpointItem * deletedTrackpoint,Trackpoint * deletedModel,
		QUndoCommand * parent) :
	FSMUndoCommand(deletedTrackpoint,deletedModel,parent) {

	//-- Update Text
	stringstream ss;
	ss << "Deleted Trackpoint" << endl;
	this->setText(ss.str().c_str());

}

UndoDeleteTrackpoint::~UndoDeleteTrackpoint() {
	// TODO Auto-generated destructor stub
}

int UndoDeleteTrackpoint::id() {
	return this->deletedGuiItem->getModel()->getTransition()->getId();
}

bool UndoDeleteTrackpoint::mergeWith(const QUndoCommand * command) {

	cout << "* Undo: Trackpoint " << this->id() << " merge with: "
			<< command->id() << endl;

	if (command->id() == this->id())
		return true;
	else
		return false;
}

void UndoDeleteTrackpoint::redo() {

	//-- Do try to delete
	cout << "Redoing delete trackpoint with id: " << this->id() << endl;

	// Save Model
	//------------------------
	this->setRelatedScene(dynamic_cast<Scene*>(this->deletedGuiItem->scene()));

	// Gui
	//--------------------

	//-- Save Previous/Next information
	this->previousItem = this->deletedGuiItem->getStartItem();
	this->nextItem = this->deletedGuiItem->getEndItem();

	//-- Call remove out of scene (Gui is ok after that)
	this->deletedGuiItem->scene()->removeItem(
			this->deletedGuiItem);

	//-- FIXME Remove trackpoint from model
	//-- Remove function says who's next now. If no one, we can restore with append
	this->nextModel = this->deletedModel->remove();
}

void UndoDeleteTrackpoint::undo() {

	// Re-add Trackpoint to scene
	//-------------------------
	this->getRelatedScene()->addItem(this->deletedGuiItem);

	//-- Remove transline between

	//-- Add Transline from start Item to trackpoint
	//Transline * previousTransline = new Transline(this->deletedTrackpointItem->getModel()->getTransition(),this->previousItem,this->deletedTrackpointItem);
	//this->getRelatedScene()->addItem(previousTransline);

	//-- Add Transline from trackpoint to end item
	//Transline * nextTransline = new Transline(this->deletedTrackpointItem->getModel()->getTransition(),this->deletedTrackpointItem,this->nextItem);
	//this->getRelatedScene()->addItem(nextTransline);




	// Re-add to model
	//----------------------
	if (this->nextModel==NULL) {
		this->deletedGuiItem->getModel()->getTransition()->appendTrackpoint(this->deletedModel);
	} else {
		this->deletedGuiItem->getModel()->getTransition()->addTrackpointBefore(this->nextModel,this->deletedModel);
	}

	//cout << "Undoing deleted state" << endl;


}
