/*
 * UndoDeleteLinkJoin.cpp
 *
 *  Created on: Sep 12, 2011
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
#include <gui/undo/UndoDeleteTransition.h>

#include "UndoDeleteLinkConnection.h"

UndoDeleteLinkConnection::UndoDeleteLinkConnection(Transline * transline,Trans * transitionModel,QUndoCommand * parent) :
				UndoDeleteTransition(transline,transitionModel,parent) {

	// Only change text
	this->setText(QString()+"Deleted Link Connection from "+transitionModel->getStartState()->getName().c_str()+" to "+transitionModel->getEndState()->getName().c_str());
}


UndoDeleteLinkConnection::~UndoDeleteLinkConnection() {
	// TODO Auto-generated destructor stub
}


