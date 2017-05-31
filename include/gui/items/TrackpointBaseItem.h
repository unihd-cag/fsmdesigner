/*
 * TrackpointBaseItem.h
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

#ifndef TRACKPOINTBASEITEM_H_
#define TRACKPOINTBASEITEM_H_

// Include
//----------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>
class Transline;
class Trackpoint;


class TrackpointBaseItem  : public FSMGraphicsItem <Trackpoint> {
public:
	TrackpointBaseItem(Trackpoint * model = NULL,QGraphicsItem * startItem = NULL,
    		QGraphicsItem * endItem = NULL);
	virtual ~TrackpointBaseItem();


	virtual void setPreviousTransline(Transline *);
	virtual Transline * getPreviousTransline();

	virtual void setNextTransline(Transline *);
	virtual Transline * getNextTransline();

	void setStartItem(QGraphicsItem * item);
	QGraphicsItem * getStartItem();

	virtual void setEndItem(QGraphicsItem * item);
	QGraphicsItem * getEndItem();


protected:

	/// This trackpoint is between...
	QGraphicsItem * 	startItem;

	/// ...and this endItem
	QGraphicsItem * 	endItem;


	/**
	 * The previous transition line
	 */
	Transline * previousTransline;

	/**
	 * The next transition line
	 */
	Transline * nextTransline;

	/**
	 * To Be implemented by children to give access to the scene
	 * @return The scene on which the item is (through scene() call)
	 */
	virtual QGraphicsScene * getScene() = 0;



};

#endif /* TRACKPOINTBASEITEM_H_ */
