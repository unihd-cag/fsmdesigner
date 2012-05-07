/*
 * StateItem.h
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */

#ifndef STATEITEM_H_
#define STATEITEM_H_

// Includes
//-----------

//-- Std
using namespace std;

//-- Core
#include <core/State.h>
class Link;



//-- GUI
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/FSMGraphicsTextItem.h>
class Transline;
class LinkArrival;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Verification
class FSMVerifyError;

class StateItemEllipse : public QGraphicsEllipseItem, public FSMGraphicsItem<State> {

protected:

	/// defines the background color of the ellipse
	QColor color;

public:

	StateItemEllipse(const QRectF& rect);
	virtual ~StateItemEllipse();


public:

	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option, QWidget *widget);


	/// Match bounding rect if we have to draw a surrounding ellipse for the reset state
	QRectF boundingRect() const;


	/// Overriden from FSMGraphicsItem#remove()
	/// @return A list of Undo command to call to undo the operation
	virtual QList<QUndoCommand*> remove();

	void setColor(QColor color) {
		this->color = color;

		//-- Reprepare Graphics
		this->prepareGraphics();
	}

	QColor getColor() {
		return this->color;
	}

	/**
	 * Prepares the graphics of this item (reset appearance, gradient etc...)
	 */
	void prepareGraphics();

protected:

	/// Implemented to support delete
	virtual void keyReleaseEvent ( QKeyEvent * event );

	/// Update appaearance depending on model
	virtual void modelChanged();

};

/**
 * Text of the state with customised background (or not)
 */
class StateItemText : public FSMGraphicsTextItem {

public:


	StateItemText(QGraphicsItem * parent = NULL);
	virtual ~StateItemText();

	void paint(QPainter *painter,
	               const QStyleOptionGraphicsItem *option, QWidget *widget);


	/**
	 * Sets this Item as filter to catch all events that should go to it to edit text
	 */
	virtual void startEditing();

	/**
	 * Removes this item as filter
	 */
	virtual void stopEditing();


protected:
	 bool sceneEvent ( QEvent * event )  ;

	 bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event );

	 virtual bool recordText();

};


/**
 * State Item ( new version)
 */
class StateItem   : public FSMGraphicsItem<State> , public QGraphicsItemGroup {


protected:

	/// true = Move loopback transitions with state, false = don't
	/// When CTRL is holded and moving the state, loopback transitions are not moved with
	bool moveLoopBackTransitions;


	/// A GraphicsItem added to this state item if there is an error
	QGraphicsItem * verificationMark;

    /// The state field
    StateItemText * stateText;

    /// The ellipse
    StateItemEllipse * stateEllipse;

    /// The various possible elements connected

    /// The transitions starting from this State
    QList<Transline*> outgoingTransitions;

    /// The transitions coming in to this State
    QList<Transline*> incomingTransitions;



public:

	enum { Type = FSMGraphicsItem<>::STATEITEM };

    /// Constructor using a State Model
    StateItem(State * model = NULL,QGraphicsItem * parent = 0);

    virtual ~StateItem();

    /// overriden
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return FSMGraphicsItem<>::STATEITEM;}

    QString text();

    /// Called when the model changed (thus from setModel if pointers are different)
	virtual void 		modelChanged() ;

    void setText(QString s);

    void setStateItemColor(QColor color);

    QColor getStateItemColor();

    StateItemEllipse * getStateEllipse() {
    	return this->stateEllipse;
    }

    void addIncomingTransition(Transline *);

    void removeIncomingTransition(Transline *);

    /**
     * Returns a reference to the incoming transitions list
     * Convenient for deletion process
     * @return
     */
    QList<Transline*>& getIncomingTransitions();

    void addOutgoingTransition(Transline *);

	void removeOutgoingTransition(Transline *);

	/**
     * Returns a reference to the outgoing transitions list
     * Convenient for deletion process
     * @return
     */
    QList<Transline*>& getOutgoingTransitions();

    /// Match bounding rect if we have to draw a surrounding ellipse for the reset state
	QRectF boundingRect() const;

    virtual bool recordPosition();

    /// Overriden from FSMGraphicsItem#remove()
	virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL);

	/** \defgroup Items Utils */
	/**@{*/

	/**
	 * Finds a Link Arrival pointing to this state
	 *
	 */
	LinkArrival * findLinkArrival();

	/**@}*/



	/** \defgroup Verification */
	/**@{*/

	/**
	 * Event Method called when a Verification Error has been added to this item
	 * @param error The added error
	 */
	virtual void verificationErrorAdded(FSMVerifyError * error) ;

	virtual void verificationErrorRemoved(FSMVerifyError * error) ;

	/** @} */


protected:

	/**
	 * Overriden to detect selection changes and thus painting parameters
	 */
	virtual QVariant 	itemChange ( GraphicsItemChange change, const QVariant & value );

	/// When mouse is released, activate selection, or update position
	virtual void		mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/// Position modification
	virtual void 		mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

	/// Activate editor mode
	virtual void 		mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event );

	/// Implemented to support delete
	virtual void		keyReleaseEvent ( QKeyEvent * event );

	/**
	 * Add Some keyboard shortcuts:
	 *
	 * Up/Down/Left/Right : Move Up/Down/Left/Right the item
	 *
	 * @param event
	 */
	virtual void 		keyPressEvent ( QKeyEvent * event );

	virtual void	    focusOutEvent ( QFocusEvent * event );

	/// See parent
	virtual bool 		recordText();

};



#endif /* STATEITEM_H_ */
