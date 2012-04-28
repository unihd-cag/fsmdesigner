/*
 * LinkArrival.h
 *
 *  Created on: Sep 27, 2010
 *      Author: rleys
 */

#ifndef LinkArrival_H_
#define LinkArrival_H_

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
#include <QtCore>
#include <QtGui>

//-- GUI
#include <gui/items/FSMGraphicsItem.h>
class Transline;
class StateItem;


/**
 * LinkArrival is the Item that displays the arrival of a Link into a state
 */
class LinkArrival : public QGraphicsPathItem, public FSMGraphicsItem<Link>  {

private :

	// To know if the link was moved because we can't deduce it from background
	bool linkMoved;

protected:



public:


    enum { Type = FSMGraphicsItem<>::LINKARRIVAL };

    // Static color list
    static QList<QColor> linkDefaultColors;


    LinkArrival(Link* model=NULL,StateItem * targetState = NULL);
    virtual ~LinkArrival();

    int type() const { return Type;}


    bool recordPosition();


	/// Get A Link color
	/// Up to 12 different colors
	static QColor& getDefaultLinkColor(int index);

	virtual void setNextTransline(Transline *);

	virtual Transline * getNextTransline();

	virtual StateItem * getTargetStateItem() {
		return this->targetState;
	}

	virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL);




protected:

    /// The shape to be painted
	QPainterPath shapePath;

	/**
	 * The next transition line
	 */
	Transline * nextTransline;

	/**
	 * The state Item we want to target
	 */
	StateItem * targetState;

    /// When mouse is released, activate selection, and validate new position
	virtual void		mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/// Sets the linkmoved variable to true
	virtual void 		mouseMoveEvent(QGraphicsSceneMouseEvent * event) ;

	/// Overriden to detect selection changes and thus painting parameters
	virtual QVariant 	itemChange ( GraphicsItemChange change, const QVariant & value );

};

#endif /* LinkArrival_H_ */
