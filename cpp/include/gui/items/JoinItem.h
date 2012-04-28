/*
 * JoinItem.h
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */

#ifndef JoinItem_H_
#define JoinItem_H_

// Includes
//--------------

//-- Std
#include <iostream>
using namespace std;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
class Join;
class Transline;

//-- Gui
#include <gui/items/FSMGraphicsItem.h>

class JoinItem : public QGraphicsRectItem , public FSMGraphicsItem<Join>{

protected:

	/// The transitions coming in to this JoinItem
	QList<Transline*> 	incomingTransitions;

	/// The transition line coming out of this join point
	Transline* 			outgoingTransition;

public:

    enum { Type = FSMGraphicsItem<>::JOINITEM };


    JoinItem(Join * model = NULL);
    ~JoinItem();

    void addIncomingTransition(Transline *);

    void removeIncomingTransition(Transline *);

    /**
     * Return a copy of the incoming transitions internal list
     * @return
     */
    QList<Transline*> getIncomingTransitions();

    void setOutgoingTransition(Transline *);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type;}

    virtual bool recordPosition();

    /// Called when the model changed (thus from setModel if pointers are different)
	virtual void modelChanged() ;

protected:

	/// When mouse is released, activate selection, or update position
	virtual void		mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/**
	 * Overriden to detect selection changes and thus painting parameters
	 */
	virtual QVariant 	itemChange ( GraphicsItemChange change, const QVariant & value );
};

#endif /* JoinItem_H_ */
