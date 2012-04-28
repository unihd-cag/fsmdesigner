/*
 * UndoDeleteLinkDestination.h
 *
 *  Created on: Sep 12, 2011
 *      Author: rleys
 */

#ifndef UNDODELETELINKDESTINATION_H_
#define UNDODELETELINKDESTINATION_H_

// Includes
//------------------

//-- Std

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
class LinkArrival;
class Link;

//-- Gui undo
#include <FSMUndoCommand.h>

/**
 * Deletes the destination of a link (Link -> target state), and all the joining transitions too
 */
class UndoDeleteLinkDestination : public FSMUndoCommand<LinkArrival,Link> {

public:
	UndoDeleteLinkDestination(LinkArrival * guiItem,Link * model,QUndoCommand * parentCommand=NULL);
	virtual ~UndoDeleteLinkDestination();


	virtual int		id ();
	virtual bool	mergeWith ( const QUndoCommand * command );
	virtual void	redo ();
	virtual void	undo ();

};

#endif /* UNDODELETELINKDESTINATION_H_ */
