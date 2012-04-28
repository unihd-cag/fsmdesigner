/*
 * UndoDeleteTransition.h
 *
 *  Created on: Mar 16, 2011
 *      Author: rleys
 */

#ifndef UNDODELETEJOIN_H_
#define UNDODELETEJOIN_H_

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

class UndoDeleteJoin : public FSMUndoCommand<JoinItem,Join> {

protected:


	/// The corresponding end Item
	/// Autodetected when deleting
	QGraphicsItem * endItem;

public:
	UndoDeleteJoin(JoinItem *,Join*,QUndoCommand * parent = NULL );
	virtual ~UndoDeleteJoin();

	QGraphicsItem * getEndItem();
	void setEndItem(QGraphicsItem *);

	virtual int	id ();
	virtual bool	mergeWith ( const QUndoCommand * command );
	virtual void	redo ();
	virtual void	undo ();

};

#endif /* UNDODELETEJOIN_H_ */
