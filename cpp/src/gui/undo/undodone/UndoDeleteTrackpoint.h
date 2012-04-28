/*
 * UndoDeleteState.h
 *
 *  Created on: Mar 16, 2011
 *      Author: rleys
 */

#ifndef UNDODELETETRACKPOINT_H_
#define UNDODELETETRACKPOINT_H_

// Includes
//--------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
class Trackpoint;

//-- Gui
#include <gui/undo/FSMUndoCommand.h>
class TrackpointItem;

class UndoDeleteTrackpoint: public FSMUndoCommand<TrackpointItem,Trackpoint> {

private:


	/**
	 * The previous item where the trackpoint was
	 */
	QGraphicsItem * previousItem;

	/**
	 * The Item after the trackpoint
	 */
	QGraphicsItem * nextItem;


	/**
	 * The Trackpoint after in the model (to readd at the right position
	 */
	Trackpoint * nextModel;



public:
	UndoDeleteTrackpoint(TrackpointItem * deletedTrackpoint,Trackpoint * deletedModel, QUndoCommand * parent = 0 );
	virtual ~UndoDeleteTrackpoint();


	virtual int	id ();
	virtual bool	mergeWith ( const QUndoCommand * command );
	virtual void	redo ();
	virtual void	undo ();
};

#endif /* UNDODELETETRACKPOINT_H_ */
