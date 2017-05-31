/*
 * UndoDeleteLinkJoin.h
 *
 *  Created on: Sep 12, 2011
 *      Author: rleys
 */

#ifndef UNDODELETELINKJOIN_H_
#define UNDODELETELINKJOIN_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
class Trans;

//-- Gui
#include <gui/undo/FSMUndoCommand.h>
class Transline;
class StateItem;

#include <gui/undo/UndoDeleteTransition.h>

/**
 * To Delete a link connection (From state to link)
 *
 * A Connection to a link is a transition from a Start Item (State, Join etc...) to a Link Item
 * The model is a transition with last trackpoint containing the link id
 *
 */
class UndoDeleteLinkConnection : public UndoDeleteTransition {

public:
	UndoDeleteLinkConnection(Transline *,Trans*,QUndoCommand * parent = NULL);
	virtual ~UndoDeleteLinkConnection();



};

#endif /* UNDODELETELINKJOIN_H_ */
