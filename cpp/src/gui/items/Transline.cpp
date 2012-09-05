/*
 * Transline.cpp
 *
 *  Created on: Oct 1, 2010
 *      Author: rleys
 */

// Includes
//-------------

//-- STL
#include <iostream>
using namespace std;

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Trackpoint.h>
#include <core/Trans.h>
#include <core/State.h>
#include <core/Link.h>

//-- QT
#include <QtCore>
#include <QtGui>

//-- UI
#include <gui/scene/Scene.h>
#include <gui/settings/GuiSettings.h>
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/LinkArrival.h>
#include <gui/items/StateItem.h>
#include <gui/items/JoinItem.h>
#include <gui/items/TranslineText.h>
#include <gui/items/HyperTransition.h>

#include <common/SGC.h>

//-- Actions
#include <gui/actions/DeleteTransitionAction.h>
#include <gui/actions/DeleteTrackpointAction.h>

#include "Transline.h"

Transline::Transline(TransitionBase * model, QGraphicsItem * startItem,
		QGraphicsItem * endItem) :
	QGraphicsPathItem() {

	// Variable inits
	//------------------
	selectedTransline = false;
	this->lastSelected = false;
	this->drawArrow = true;


	// Trackpoint dynamic add
	this->addedTrackpoint = NULL;

	// Model
	//----------
	this->setModel(model);

	// Record start and stop
	//---------------
	this->startItem = NULL;
	this->endItem = NULL;
	if (startItem!=NULL) {
		this->setStartItem(startItem);
		this->endPoint = startItem->pos();
	}

	if (startItem!=endItem)
		this->setEndItem(endItem);
	else
	    this->endItem = endItem;

	//this->endPoint.setX(startItem!=NULL?startItem->x():0);
	//this->endPoint.setY(startItem!=NULL?startItem->y():0);



	// Ui Flags
	//--------------
	this->setFlag(ItemIsSelectable,this->getModel()!=NULL);
	QGraphicsDropShadowEffect * dse  = new QGraphicsDropShadowEffect();
	dse->setColor(Qt::lightGray);
	dse->setOffset(5,5);

}

Transline::~Transline() {

	//-- Ensure we can't be on the scene, so that all events are sent
	if (this->scene()!=NULL)
		this->scene()->removeItem(this);

}

void Transline::preparePath(bool propagate) {

	// End Item
	//---------------

	//-- End with an arrow ?
	if (this->getEndItem() != NULL &&
		 ( this->getEndItem()->type() == TrackpointItem::Type ||
		  this->getEndItem()->type() == LinkDeparture::Type
		 )
		)
		this->drawArrow = false;
	else if (this->getEndItem() != NULL) {
		this->drawArrow = true;
	}

	// Build Line
	//------------------


	QPainterPath linePath;

	if (this->startItem==NULL) {
		this->setPath(linePath);
		return;
		//return linePath;
	}


	// Size parameter for arrow
	qreal arrowSize = 15;

	// Check line ends not overlapping a state or something
	// (stick to start/end shapes)
	//---------
	QLineF lineToPaint;
	lineToPaint.setP1(this->startItem->pos());
	if (this->endItem != NULL && (this->startItem!=this->endItem)) {
		lineToPaint.setP2(this->endItem->pos());
	} else
		lineToPaint.setP2(this->endPoint);

	//-- Check start item
	if (this->startItem->type() == StateItem::Type  ||
		this->startItem->type() == TrackpointItem::Type ||
		this->startItem->type() == LinkArrival::Type ||
		this->startItem->type() == HyperTransition::Type) {

		int reduce = this->startItem->boundingRect().width() / 2;
		reduce += 3;

		//-- Reduce Length to stay on edge of start element
		QLineF workLine = lineToPaint;

		// Target length = length - reduce
		qreal targetLength = lineToPaint.length() - reduce;

		// Turn that to a 0:1 proportion of original line
		qreal lengthFraction = targetLength / lineToPaint.length();

		// The new p1 is at (1-fraction)
		QPointF newP1 = workLine.pointAt(1 - lengthFraction);

		// Move P1
		workLine.setP1(newP1);

		lineToPaint = workLine;
		//this->setLine(workLine);
	}

	//-- Check end item
	if (this->endItem != NULL &&
			(this->endItem->type() == StateItem::Type ||
			this->endItem->type()== TrackpointItem::Type ||
			this->endItem->type()== LinkDeparture::Type)) {

		int reduce = this->endItem->boundingRect().height() / 2;
		reduce += 3;

		// Reduce Length to stay on edge of target element
		QLineF workLine = lineToPaint;
		workLine.setLength(lineToPaint.length() - reduce);

		// Reduce additionaly from arrow lengt
		if (this->drawArrow == true) {
			workLine.setLength(workLine.length() - arrowSize);
		}

		lineToPaint = workLine;
		//this->setLine(workLine);

	} else if (this->endItem != NULL && this->endItem->type() == JoinItem::Type) {

		QLineF workLine = lineToPaint;

		//-- Determine incoming direction using p1 and endItem position
		QRectF endrect = this->endItem->boundingRect();
		FSMGraphicsItem<>::ApproachDirection dir = FSMGraphicsItem<>::TOP;
		if (workLine.p1().y() < endrect.y()) {
			dir = FSMGraphicsItem<>::TOP;
		} else if (workLine.p1().y() > endrect.y()+endrect.height()) {
			dir = FSMGraphicsItem<>::BOTTOM;
		} else if (workLine.p1().x() < endrect.x()) {
			dir = FSMGraphicsItem<>::LEFT;
		} else if (workLine.p1().x() > endrect.x()+endrect.width()) {
			dir = FSMGraphicsItem<>::RIGHT;
		}

		//-- Determine intersection of bounding rects
		QRectF inter = this->endItem->boundingRect().intersected(this->boundingRect());

		//qDebug() << "Transline joining join from " << dir;


		if (dir == FSMGraphicsItem<>::TOP) {

			//qDebug() << "JoinItem height is then " << endrect.height();
			//qDebug() << "Intersection height " << inter.height();

			//-- We take the position of the bounding rect as reference
			//-- And reduce from the difference with the end item position

			workLine.setLength(lineToPaint.length() - (endrect.height()+1));

			if (this->drawArrow == true) {
				workLine.setLength(workLine.length() - arrowSize);
			}

		} else if (dir == FSMGraphicsItem<>::BOTTOM) {

		}


		//-- Set usage of workline
		lineToPaint = workLine;

	}


	// Define line path to the line created or a special path for looping states
	//----------------
	//-- Looping on the same spot
	if (this->startItem==this->endItem) {

	    qDebug() << "Looping";

	    qDebug() << "Looping";

	    QPainterPath loop;
	    linePath = loop;
	    //loop.moveTo(this->startItem->pos());




	    /*loop.arcTo(QRectF(this->startItem->pos().x(),this->startItem->pos().y(),100,100),
	            45,360-45);*/
	    /*loop.arcMoveTo(QRectF(this->startItem->pos().x(),this->startItem->pos().y(),100,100),
	                    360-45);*/

//	    loop.quadTo(QPointF(this->startItem->pos().x()+100,this->startItem->pos().y()),
//	            QPointF(this->startItem->pos().x(),this->startItem->pos().y()+20));

	    //loop.closeSubpath();

	   // loop.translate(this->startItem->pos());

	   // loop.addEllipse(QRectF(0,0,100,100));

	    //loop.moveTo(this->startItem->pos().x()+100,this->startItem->pos().y());
	    //loop.moveTo(this->startItem->pos().x()+100,this->startItem->pos().y()+100);
	    //loop.quadTo(this->startItem->pos().x()+20,this->startItem->pos().y()+20,this->startItem->pos().x(),this->startItem->pos().y());

	    loop.moveTo(this->startItem->pos().x(),this->startItem->pos().y()-20);
	    loop.cubicTo(this->startItem->pos().x(),this->startItem->pos().y(),
	                  this->startItem->pos().x()+200,this->startItem->pos().y(),
	                  this->startItem->pos().x(),this->startItem->pos().y()+20);
	    loop.closeSubpath();

	    linePath = loop;

	}
	else {

		//-- Default design

		// Add Line to path
		linePath.moveTo(lineToPaint.p1());
		linePath.lineTo(lineToPaint.p2());
	}

	// Set Style of the transline
	//--------------------

    //-- Default style

    // Pen size
    this->setPen(QPen(Qt::black));
    int width = GuiSettings::value("uni.hd.ziti.fsmdesigner.ui.transition.lineWidth", QVariant(1)).toInt();
    QPen newpen(this->pen());
    newpen.setWidth(width);
    this->setPen(newpen);

    if (this->getModel() != NULL && ((Trans*)this->getModel())->isDefault() == true) {
        QPen np = QPen(this->pen());
        np.setStyle(Qt::DashDotDotLine);
        np.setWidth(this->pen().width() + 1);
        this->setPen(np);
    } else {
        QPen np = QPen(this->pen());
        np.setStyle(Qt::SolidLine);
        this->setPen(np);
    }


    //-- Arrow if necessary
    if (this->drawArrow) {

        // Add Brush to fill arrow (have a little bit fun here)
        //---------------
        this->setBrush(QBrush(Qt::black,Qt::Dense4Pattern));

        QPolygonF arrowHead;
        qreal arrowSize = 15;
        qreal arrowAngle = 20;

        // Duplicate line,shorten it, and invert p1 and 2
        QLineF traceLine(lineToPaint);
        traceLine.setLength(traceLine.length() + arrowSize);// restore length up to object to have arrow touch it
        QPointF p1 = traceLine.p1();
        traceLine.setP1(traceLine.p2());
        traceLine.setP2(p1);
        traceLine.setLength(arrowSize);

        // Start point is always the same and is top of arrow polygon
        arrowHead << traceLine.p1();

        // Deviate of 10° on both directions and  register ending point as new basis of polygon
        traceLine.setAngle(traceLine.angle() + arrowAngle); // |"\"
        arrowHead << traceLine.p2();
        traceLine.setAngle(traceLine.angle() - arrowAngle); // back to normal: |
        traceLine.setAngle(traceLine.angle() - arrowAngle);// "/"|
        arrowHead << traceLine.p2();
        arrowHead<<traceLine.p1();

        // And paint :)
        linePath.addPolygon(arrowHead);
    }

	// Set Line path to path to be painted
	this->setPath(linePath);

}

void Transline::setStartItem(QGraphicsItem * item) {

	// Record
	this->startItem = item;

	//-- If start is a  State item, add to list of outgoing Transitions
	if (startItem != NULL && startItem->type() == StateItem::Type) {

	    //-- Add to outgoing
		qgraphicsitem_cast<StateItem*> (startItem)->addOutgoingTransition(this);
	}
	//-- If start is a Join Point, set as Outgoing transition
	else if (startItem != NULL && startItem->type() == JoinItem::Type) {
			qgraphicsitem_cast<JoinItem*> (startItem)->setOutgoingTransition(this);
	}
	//-- If start is a Hypertransition, set as Outgoing transition
    else if (startItem != NULL && startItem->type() == HyperTransition::Type) {
            qgraphicsitem_cast<HyperTransition*> (startItem)->setOutgoingTransition(this);
    }

	// If no Start and no end, we have to delete ourselves
	// Otherwise draw
	//------------------------
	this->preparePath();

}
QGraphicsItem * Transline::getStartItem() {
	return this->startItem;
}

void Transline::setEndItem(QGraphicsItem * item) {

	// Record
	//----------------
	this->endItem = item;

	//-- If end was a trackpoint, this transline is the previous
	/*if (endItem != NULL && endItem->type() == TrackpointItem::Type) {
		FSMGraphicsItem<>::toTrackPoint(endItem)->setPreviousTransline(this);
	}*/
	//-- If end is a link item, set as previous
	if (endItem != NULL && endItem->type() == LinkDeparture::Type) {
		((LinkDeparture*) qgraphicsitem_cast<LinkDeparture*> (endItem))->setPreviousTransline(
				this);
	}
	//-- If end was a state, add to list of incoming Transitions
	else if (endItem != NULL && endItem->type() == StateItem::Type) {
		qgraphicsitem_cast<StateItem*> (endItem)->addIncomingTransition(this);
	}
	//-- If end was a Join, add to list of incoming Transitions
	else if (endItem != NULL && endItem->type() == JoinItem::Type) {
			qgraphicsitem_cast<JoinItem*> (endItem)->addIncomingTransition(this);
	}

	// If no Start and no end, we have to delete ourselves
	// Otherwise draw
	//------------------------
	this->preparePath();


}
QGraphicsItem * Transline::getEndItem() {
	return this->endItem;
}

void Transline::setEndPoint(QPointF point) {
	this->endPoint = point;
	this->preparePath();
}

QPointF Transline::getEndPoint() {
	return this->endPoint;
}

void Transline::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {

   /* painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setOpacity(this->effectiveOpacity());


	painter->setPen(this->pen());
	painter->setBrush(this->brush());

	painter->drawPath(this->path());

	if (this->startItem!=this->endItem)
	    painter->fillPath(this->path(),this->brush());

	painter->restore();*/

    QGraphicsPathItem::paint(painter,option,widget);

	return;
}

void Transline::modelChanged() {

	// Not selectable without model
    //-------------
	if (this->getModel()==NULL) {
		this->setFlag(ItemIsSelectable,false);
	} else {
		this->setFlag(ItemIsSelectable,true);

		// Try to Update text
        //--------------------
        if (this->scene()!=NULL && this->getModel()!=NULL) {
            Scene * scene = dynamic_cast<Scene*>(this->scene());
            TranslineText * text= scene->findTranslineText(((Trans*)this->getModel()));
            if (text!=NULL) {
                text->modelChanged();
            } else {
                qDebug() << "No Transline Text with matching transition model";
            }
        }

	}



}

QVariant Transline::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged) {

		// Propagate selection change
		// Only to trackpoints
		//-----------


		if (value.toBool() == true) {

			// Selection propagation
			this->propagateSelection();

		}

		// Local painting change
		//-------------
		int width =
				GuiSettings::value("uni.hd.ziti.fsmdesigner.ui.transition.lineWidth", QVariant(1)).toInt();
		if (value.toBool() == true) {

			QPen newPen(this->pen());
			//QPen newPen(Qt::red);
			newPen.setColor(Qt::red);
			newPen.setWidth(width+1);
			this->setPen(newPen);
			this->setBrush(QBrush(Qt::red));
		} else {

//			QPen newPen(Qt::black);
			QPen newPen(this->pen());
			newPen.setColor(Qt::black);
			newPen.setWidth(width);
			this->setPen(newPen);
			this->setBrush(QBrush(Qt::black));
			this->setOpacity(1.0);
		}

	} else if (change== QGraphicsItem::ItemSceneHasChanged && this->scene()==NULL ) {

		//-- If removed from scene
		//-- Ensure the Previous Item forget about us
		if (this->getStartItem()!=NULL && this->isStateItem(this->getStartItem())) {

			this->toStateItem(this->getStartItem())->removeOutgoingTransition(this);

		}

		//-- Ensure the Next Item forgets about us
		//----------------------
		if (this->getEndItem()!=NULL && this->isStateItem(this->getEndItem())) {

			this->toStateItem(this->getEndItem())->removeIncomingTransition(this);

		} else if (this->getEndItem()!=NULL && FSMGraphicsItem<>::isJoinItem(this->getEndItem())) {

			FSMGraphicsItem<>::toJoinItem(this->getEndItem())->removeIncomingTransition(this);
		}
		//-- If the end is a Link Item, we can delete it
		else if (this->getEndItem()!=NULL && this->getEndItem()->type()==FSMGraphicsItem<>::LINKDEPARTURE) {
		    //this->getEndItem()->scene()->removeItem(this->getEndItem());
			//SGC::getInstance()->requestDelete(this->getEndItem());
		}

	}

	return QGraphicsPathItem::itemChange(change, value);

}

// Perform selection propagation
void Transline::propagateSelection() {

	// To start Item
	//-----------------------
	if (this->startItem->type() == TrackpointItem::Type) {
		//FSMGraphicsItem<>::toTrackPoint(this->startItem)->getPreviousTransline()->propagateSelection();
		if (this->startItem->isSelected() != true)
			this->startItem->setSelected(true);
	}

	// To End Item
	//----------------
	if (this->endItem != NULL && this->endItem->type() == TrackpointItem::Type) {
		//FSMGraphicsItem<>::toTrackPoint(this->endItem)->getNextTransline()->propagateSelection();
		if (this->endItem->isSelected() != true)
			this->endItem->setSelected(true);
	}


}

void Transline::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {

	// Reset added trackpoint
	//---------
	if (this->addedTrackpoint != NULL) {
		this->scene()->sendEvent(this->addedTrackpoint, event);


	}


	this->addedTrackpoint = NULL;



	// Propagate selection change
	//-----------
	// Selection propagation
	this->propagateSelection();

}

void Transline::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {

	// If trackpoint already added, then forward to it
	//------------------
	if (this->addedTrackpoint != NULL) {
		this->scene()->sendEvent(this->addedTrackpoint, event);
		return;
	}

	// If no model, can't derive new trackpoints
	//-------------
	if (this->getModel() == NULL)
		return;

	// Forbid if next Item is a LinkDeparture
	//------------------
    if (this->getEndItem()!=NULL && this->getEndItem()->type()==FSMGraphicsItem<>::LINKDEPARTURE) {
        return;
    }

	// Check there is nothing too close
	//----------------------

	// Trace a selection area
	qreal selectionArea = 15;
	QList<QGraphicsItem *> foundItems = this->scene()->items(QRectF(
			event->scenePos().x() - selectionArea / 2, event->scenePos().y()
					- selectionArea / 2, selectionArea, selectionArea));

	// If we find something else than the line into it, do nothing
	if (foundItems.size() > 1)
		return;

	// The Mouse move must be large enough, to improve usability
	//-------------------------------------------
	unsigned int requiredMoveAmplitude = 2; // 15 pixels
	unsigned int xAmplitude = abs(event->pos().x() - event->lastPos().x());
	unsigned int yAmplitude = abs(event->pos().y() - event->lastPos().y());
	if (xAmplitude <= requiredMoveAmplitude && yAmplitude <= requiredMoveAmplitude) {
	    return;
	}

	// Otherwise, Create Trackpoint
	//-------------------
	Fsm& f = *(dynamic_cast<Scene *>(this->scene())->getFsm());
	f.getTransbyID(this->getModel()->getId());
	Trackpoint * newTrackpointModel = NULL;




	//-- Trackpoint position
	bool after = true;
	if (this->getStartItem()->type() == StateItem::Type) {
		// After the start item (add First
		newTrackpointModel = this->getModel()->addFirstTrackpoint(event->scenePos().x(), event->scenePos().y());

	} else if (this->getEndItem()->type() == StateItem::Type) {
		// Before the end item
		newTrackpointModel = this->getModel()->appendTrackpoint(event->scenePos().x(), event->scenePos().y());

	} else if (this->getEndItem()->type() == JoinItem::Type) {
		// Before the end trackpoint (which is the join marker)
		newTrackpointModel = this->getModel()->addTrackpointBefore(this->getModel()->getTrackpoints().back(),event->scenePos().x(), event->scenePos().y());

	} else {
		// Just add
		newTrackpointModel = this->getModel()->addTrackpointBefore(FSMGraphicsItem<>::toTrackPoint(this->getEndItem())->getModel(),event->scenePos().x(), event->scenePos().y());
	}

	// Add to UI
	//----------------

	//-- Add Trackpoint between this transline start and end item
	TrackpointItem * addedTrackpoint = new TrackpointItem(newTrackpointModel,this->getStartItem(),this->getEndItem());
	this->scene()->addItem(addedTrackpoint);
	addedTrackpoint->setPos(event->scenePos());
	//this->addedTrackpoint = addedTrackpoint;


	addedTrackpoint->grabMouse();

	//-- At that point, the transline might not be visible anymore
	// If not visible anymore, then delete ourselves
	// If our start was not a trackpoint, we may leave
	//---------------
	if (!this->isVisible() ||!FSMGraphicsItem<>::isTrackPoint(this->getStartItem())) {
		delete this;
	}

	return;

	//-- Set Selected item as the trackpoint
	//addedTrackpoint->scene()->clearSelection();
	//addedTrackpoint->setSelected(true);

	//-- This transline simply dissapears if the start item is not a trackpoint
//	if (!FSMGraphicsItem<>::isTrackPoint(this->getStartItem()))
//		delete this;

}

QList<QUndoCommand*> Transline::remove(QUndoCommand * parentComand) {



	//-- Prepare Undo
	QList<QUndoCommand*> undoCommands;

	//-- Don't remove if not on the scene
	if (this->scene()==NULL)
	    return undoCommands;


	//-- FIXME If the far end is a LinkDeparture, uses a delete Link Item object not delete Transition

	//-- Remove itself
	DeleteTransitionAction * undoDeleteThis = new DeleteTransitionAction(this,parentComand);
	undoCommands.append(undoDeleteThis);


	return undoCommands;

}
