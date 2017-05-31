/*
 * TrackpointItem.h
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

#ifndef TRACKPOINTITEM_H_
#define TRACKPOINTITEM_H_

// Includes
//-------------

//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>
using namespace std;

//-- Core


//-- Qt
#include <QtGui>
#include <QtCore>

//-- GUI
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/TrackpointBaseItem.h>
class Trackpoint;
class Trans;


class TrackpointItem : public QGraphicsPathItem , public TrackpointBaseItem {

public:

    enum { Type = FSMGraphicsItem<>::TRACKPOINT };


    TrackpointItem(Trackpoint * model,QGraphicsItem * startItem = NULL,
    		QGraphicsItem * endItem = NULL);
    ~TrackpointItem();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type;}

    bool recordPosition();





	/// Propagate removal  and remove itself
	virtual QList<QUndoCommand*> remove();

	/// @overload
	virtual void modelChanged();

protected:



	/**
	 * Overriden to detect selection changes and thus painting parameters
	 */
	virtual QVariant	itemChange ( GraphicsItemChange change, const QVariant & value );


	/// When mouse is released, activate selection, and validate new position
	virtual void		mousePressEvent ( QGraphicsSceneMouseEvent * event );

	/// When mouse is released, activate selection, and validate new position
	virtual void		mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/// When mouse is moved, it should be the only one to be selected in movement
	virtual void		mouseMoveEvent ( QGraphicsSceneMouseEvent * event );


	/**
	 * Add Some keyboard shortcuts:
	 *
	 * Up/Down/Left/Right : Move Up/Down/Left/Right the item
	 *
	 * @param event
	 */
	virtual void		keyPressEvent ( QKeyEvent * event );

	/// Do some special stuff when selection state changes, like changing color and so on
	virtual void 		selected();

	/// Do some special stuff when selection state changes, like changing color and so on
	virtual void		deselected();

	/// Perform selection propagation
	virtual void 		propagateSelection();

	virtual QGraphicsScene * getScene();



};

#endif /* TRACKPOINTITEM_H_ */
