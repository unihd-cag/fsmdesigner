/*
 * FSMVerifyError.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: rleys
 */

// Includes
//-----------------


//-- Std

//-- Gui
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/StateItem.h>

#include "FSMVerifyError.h"

FSMVerifyError::FSMVerifyError(VerifyErrorType type) {
	this->type = type;

}

FSMVerifyError::~FSMVerifyError() {



}

void FSMVerifyError::remove() {

	//-- Remove itself from all Concerned Items
	for (QList<QGraphicsItem*>::iterator it = this->concernedItems.begin(); it != this->concernedItems.end(); it++) {

		if (FSMGraphicsItem<>::isStateItem(*it)) {
			FSMGraphicsItem<>::toStateItem(*it)->removeVerificationError(this);
		}

	}
	this->concernedItems.clear();

}


bool FSMVerifyError::addConcernedItem(QGraphicsItem * item) {
	if (this->concernedItems.contains(item))
		return false;
	this->concernedItems+= item;
	return true;
}
