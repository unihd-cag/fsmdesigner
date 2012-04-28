/*
 * Transline.h
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

#ifndef TRANSLINE_H_
#define TRANSLINE_H_

// Includes
//-------------

//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>
using namespace std;

//-- Core
class Trans;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- GUI
#include <gui/items/FSMGraphicsItem.h>
class TrackpointItem;

class Transline: public QGraphicsPathItem, public FSMGraphicsItem<Trans> {

    protected:

        bool lastSelected;
        bool drawArrow;

        bool selectedTransline;

    public:

        enum {
            Type = FSMGraphicsItem<>::TRANSLINE
        };

        /**
         *
         * @param model
         * @param startItem
         * @param endItem
         */
        Transline(Trans * model = NULL, QGraphicsItem * startItem = NULL,
                QGraphicsItem * endItem = NULL);
        virtual ~Transline();


        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

        int type() const {
            return Type;
        }

        /// No Implementation
        virtual bool recordPosition() {
            return false;

        }

        /// Propagate removal of transline and remove itself
        virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL);

        /**
         * Reconstruct the path based on start/end items positions/types
         *
         * @param propagate Propagate to previous <-> next
         */
        virtual void preparePath(bool propagate = false);

        void setStartItem(QGraphicsItem * item);
        QGraphicsItem * getStartItem();

        void setEndItem(QGraphicsItem * item);
        QGraphicsItem * getEndItem();

        void setEndPoint(QPointF point);
        QPointF getEndPoint();

        /// @overload
        virtual void modelChanged();

    protected:

        /// This transline goes from this startItem...
        QGraphicsItem * startItem;

        /// ...until this endItem
        QGraphicsItem * endItem;

        /// ...oder this end point (Usefull for Line following mouse position when adding items)
        QPointF endPoint;

        /// Temporary added trackpoint
        TrackpointItem * addedTrackpoint;

        /**
         * Overriden to detect selection changes and thus painting parameters
         */
        virtual QVariant itemChange(GraphicsItemChange change,
                const QVariant & value);

        /// When mouse is released, activate selection
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

        /// When mouse is moved, add a trackpoint
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        /// Performs selection propagation
        virtual void propagateSelection();

};
#endif /* TRANSLINE_H_ */
