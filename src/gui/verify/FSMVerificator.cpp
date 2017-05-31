/*
 * FSMVerificator.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: rleys
 */

// Includes
//-----------------


//--Std
#include <map>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtScript>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/StateItem.h>
#include <gui/scene/Scene.h>

#include <gui/common/GUIUtils.h>
#include <gui/common/SGC.h>

//-- Verification
#include <gui/verify/FSMVerifyError.h>


#include "FSMVerificator.h"

FSMVerificator::FSMVerificator(Scene * scene) {
	this->setRelatedScene(scene);

}

FSMVerificator::~FSMVerificator() {
	// TODO Auto-generated destructor stub
}


void FSMVerificator::verify() {

	//-- First reset
	this->reset();

	//-- Prepare Help variables
	//-------------------------

	QList<QGraphicsItem*> allItems = this->getRelatedScene()->items();
	QList<StateItem*> stateItems;

	//-- Sort All State Items
	for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

		if (FSMGraphicsItem<>::isStateItem(*it)) {
			stateItems.append(FSMGraphicsItem<>::toStateItem(*it));

		}
	}

	// Execute All Verification Scripts
	//-------------------------------------
	this->verifyStateUniqueOutput(stateItems);

}

void FSMVerificator::reset() {

	//-- Reset
	//-------------------------

	QList<QGraphicsItem*> allItems = this->getRelatedScene()->items();

	//-- Sort All FSM Items Items
	SGC::getInstance()->activate();
	for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

		if (FSMGraphicsItem<>::isStateItem(*it)) {
			//-- Reset Errors
			FSMGraphicsItem<>::toStateItem(*it)->verificationReset();
		}
	}
	SGC::getInstance()->collect();

}


void FSMVerificator::verifyStateUniqueOutput(QList<StateItem*> stateItems) {

	//-- Map with outputs and states
	map<string,StateItem*> outputsMap;

	//-- Go through All State Items
	for (QList<StateItem*>::iterator it = stateItems.begin();it!=stateItems.end();it++) {

		//-- Do we find Output in map
		if (outputsMap[(*it)->getModel()->getOutput()]==NULL) {

			//-- No, it is unique up to now
			outputsMap[(*it)->getModel()->getOutput()] = *it;

		} else {

			//-- Already found
			//--------------

			//-- Create new error, or use one already on the item in the map
			FSMVerifyError * error = outputsMap[(*it)->getModel()->getOutput()]->getVerificationError(FSMVerifyError::STATE_UNIQUE_OUTPUT);
			if (error==NULL) {
				error = new FSMVerifyError(FSMVerifyError::STATE_UNIQUE_OUTPUT);
				error->setMessage("Some states have an identical output");
			}

			//-- put an error to the one in the map, and the processed one
			outputsMap[(*it)->getModel()->getOutput()]->addVerificationError(error);
			(*it)->addVerificationError(error);

			//-- Register Items to the error
			error->addConcernedItem((*it));
			error->addConcernedItem(outputsMap[(*it)->getModel()->getOutput()]);


			//qDebug() << "Found two states with same output";
		}


	}

}


