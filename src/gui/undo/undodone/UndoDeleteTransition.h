/*
 * UndoDeleteTransition.h
 *
 *  Created on: Mar 16, 2011
 *      Author: rleys
 */

#ifndef UNDODELETETRANSITION_H_
#define UNDODELETETRANSITION_H_

// Includes
//--------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
class Trans;

//-- Gui
#include <gui/undo/FSMUndoCommand.h>
class Transline;
class StateItem;

class UndoDeleteTransition: public FSMUndoCommand <Transline,Trans> {

protected:

	/// If the transition was deleted along with a State Item
	/// Autodected when deleting
	StateItem * startItem;

	/// The corresponding end Item
	/// Autodetected when deleting
	QGraphicsItem * endItem;

public:
	UndoDeleteTransition(Transline *,Trans*,QUndoCommand * parent = NULL );
	virtual ~UndoDeleteTransition();

	StateItem * getStartItem();
	void setStartItem(StateItem *);

	QGraphicsItem * getEndItem();
	void setEndItem(QGraphicsItem *);

	virtual int		id ();
	virtual bool	mergeWith ( const QUndoCommand * command );
	virtual void	redo ();
	virtual void	undo ();

};

#endif /* UNDODELETETRANSITION_H_ */
