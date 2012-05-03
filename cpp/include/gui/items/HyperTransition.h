/*
 * HyperTransition.h
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */

#ifndef HYPERTRANSITION_H_
#define HYPERTRANSITION_H_

// Includes
//--------------

//-- Std
#include <iostream>
using namespace std;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
#include <core/State.h>
#include <core/Trans.h>
#include <core/Trackpoint.h>
#include <core/Hypertrans.h>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>

class HyperTransition: public QGraphicsEllipseItem, public FSMGraphicsItem<
        Hypertrans> {

    protected:

    public:

        enum {
            Type = FSMGraphicsItem<>::HYPERTRANS
        };

    protected:

        /// The transition line coming out of this join point
        Transline* outgoingTransition;

    public:
        HyperTransition(Hypertrans * model = NULL);
        virtual ~HyperTransition();

        int type() const {
            return Type;
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

        virtual bool recordPosition();

        virtual void modelChanged();


        void setOutgoingTransition(Transline *);


        virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL);


    protected:
        /// When mouse is released, activate selection, and validate new position
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

        /**
         * Overriden to detect selection changes and thus painting parameters
         */
        virtual QVariant itemChange(GraphicsItemChange change,
                const QVariant & value);
};

#endif /* HYPERTRANSITION_H_ */
