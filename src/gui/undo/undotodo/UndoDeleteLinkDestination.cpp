/*
 * UndoDeleteLinkDestination.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Link.h>
#include <core/Fsm.h>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/common/SGC.h>
#include <gui/items/LinkArrival.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>

#include "UndoDeleteLinkDestination.h"

UndoDeleteLinkDestination::UndoDeleteLinkDestination(LinkArrival * guiItem,
		Link * model, QUndoCommand * parentCommand) :
		FSMUndoCommand(guiItem, model, parentCommand) {

}

UndoDeleteLinkDestination::~UndoDeleteLinkDestination() {

}

int UndoDeleteLinkDestination::id() {
	return this->deletedModel->getId();
}

bool UndoDeleteLinkDestination::mergeWith(const QUndoCommand * command) {
	return false;
}

void UndoDeleteLinkDestination::redo() {

	// Remove Children
	//-------------------
	QUndoCommand::redo();

	// Redo delete
	//--------------------------
	SGC::getInstance()->activate();

	//-- Delete from Scene
	this->getRelatedScene()->removeItem(this->deletedGuiItem);

	//-- Delete from model
	this->getRelatedScene()->getFsm()->deleteLink(this->deletedModel->getId());


	SGC::getInstance()->collect();




}
void UndoDeleteLinkDestination::undo() {



	// Replace element
	//--------------------------

	//-- Readd model

	//-- Readd Gui
	this->getRelatedScene()->addItem(this->deletedGuiItem);

	//-- Create transition Line to state
	Transline * transLine = new Transline(NULL, (LinkArrival*)this->deletedGuiItem,
			this->deletedGuiItem->getTargetStateItem());
	this->getRelatedScene()->addItem(transLine);
	this->deletedGuiItem->setNextTransline(transLine);

	// Replace Children
	//-------------------
	QUndoCommand::undo();

}

