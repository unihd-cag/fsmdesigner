/*
 * LinkDeparture.h
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

#ifndef LinkDeparture_H_
#define LinkDeparture_H_

// Includes
//-------------

//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>
using namespace std;

//-- Core
class Link;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- GUI
#include <gui/items/TrackpointItem.h>
class Transline;
class Trackpoint;

/**
 * Departure to target another state using a link label
 */
class LinkDeparture : public TrackpointItem {

public:

    enum { Type = FSMGraphicsItem<>::LINKDEPARTURE };


    /// There is no end Item for a LinkDeparture
    LinkDeparture(Trackpoint * model = NULL,QGraphicsItem * startItem = NULL);
    virtual ~LinkDeparture();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type;}
    QColor LinkDepartureColor; // defines the color for each LinkDeparture

    bool recordPosition();

    /// Overriding of this method
    virtual void setPreviousTransline(Transline *);

    /// No end item is required
    virtual void setEndItem(QGraphicsItem * item);

    /**
	 * Reconstruct the path based on destination state position
	 */
	virtual void preparePath();

	/// Removing the link Item is the same as removing its or All transition we can find back (Through joins for example)
	virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL);

	/// @overload
    virtual void modelChanged();


protected:

    	/// The shape to be painted (without text)
    	QPainterPath shapePath;

        /// When mouse is released, activate selection, or update position
		virtual void		mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

		/// Position modification
		virtual void 		mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

		/**
		 * Overriden to detect selection changes and thus painting parameters
		 */
		virtual QVariant 	itemChange ( GraphicsItemChange change, const QVariant & value );

		/// Do some special stuff when selection state changes, like changing color and so on
		virtual void 		selected();

		/// Do some special stuff when selection state changes, like changing color and so on
		virtual void		deselected();



		virtual QGraphicsScene * getScene();

};

#endif /* LinkDeparture_H_ */
