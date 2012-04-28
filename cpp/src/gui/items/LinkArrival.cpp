/*
 * LinkArrival.cpp
 *
 *  Created on: Sep 27, 2010
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>
using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/State.h>
#include <core/Trans.h>
#include <core/Trackpoint.h>
#include <core/Link.h>
#include <core/Fsm.h>
#include <core/Core.h>

//-- UI
#include <gui/scene/Scene.h>

#include <gui/common/SGC.h>

#include <gui/settings/GuiSettings.h>
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/TrackpointBaseItem.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>
#include <gui/items/LinkDeparture.h>

#include <gui/actions/DeleteLinkArrivalAction.h>


#include "LinkArrival.h"

// Static color things
QList<QColor> LinkArrival::linkDefaultColors;

QColor& LinkArrival::getDefaultLinkColor(int index) {

	//-- Shall we init
	if (LinkArrival::linkDefaultColors.size() == 0) {
		LinkArrival::linkDefaultColors << Qt::red;
		LinkArrival::linkDefaultColors << Qt::blue;
		LinkArrival::linkDefaultColors << Qt::darkGreen;
		LinkArrival::linkDefaultColors << Qt::darkBlue;
		LinkArrival::linkDefaultColors << Qt::cyan;
		LinkArrival::linkDefaultColors << Qt::darkYellow;
		LinkArrival::linkDefaultColors << Qt::darkRed;
		LinkArrival::linkDefaultColors << Qt::darkMagenta;
		LinkArrival::linkDefaultColors << Qt::magenta;
	}

	//-- Return requested color, first one if index is to big
	if (index >= LinkArrival::linkDefaultColors.size()) {
		return LinkArrival::linkDefaultColors.first();
	} else {
		return LinkArrival::linkDefaultColors[index];
	}

}

LinkArrival::LinkArrival(Link * model,StateItem * targetState) :
	QGraphicsPathItem() {

	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
	this->nextTransline = NULL;
	this->targetState = targetState;

	// Prepare Painting path
	//---------
	qreal height = 15;
	qreal width = 20;
	this->shapePath.moveTo(0, 0);
	this->shapePath.quadTo(width - width / 8, 0, width, height / 2);
	this->shapePath.quadTo(width - width / 8, height, 0, height);
	this->shapePath.closeSubpath();
	//this->shapePath.translate(0,-height/2);

	QPainterPath rectPath;
	rectPath.addRect(0, 0, width, height);
	QPainterPath final = rectPath.subtracted(this->shapePath.translated(-width
			/ 4, 0));
	final.translate(-width / 2, -height / 2);

	this->setPath(final);

	// Data from model ?
	//-----------------------
	this->setModel(model);
	if (this->getModel() != NULL) {

		//-- Color ?
		if (this->getModel()->getColor() > 0) {
			this->setBrush(QBrush(QColor(this->getModel()->getColor())));
		} else {
			// Take in predefined list
			this->setBrush(QBrush(LinkArrival::getDefaultLinkColor(
					this->getModel()->getId())));
		}

		//-- Position
		this->setPos(this->getModel()->getPosition().first,this->getModel()->getPosition().second);
	}

}

LinkArrival::~LinkArrival() {

}

QList<QUndoCommand*> LinkArrival::remove(QUndoCommand * parentComand) {


	//-- Prepare Undo
	QList<QUndoCommand*> undoCommands;


	// Remove itself (Link Departures are removed from this action)
	DeleteLinkArrivalAction * undoDeleteThis = new DeleteLinkArrivalAction(this,parentComand);
	undoCommands.append(undoDeleteThis);

	return undoCommands;

}


void LinkArrival::setNextTransline(Transline * line) {

	this->nextTransline = line;

	// Update direction
	// Determine orientation using next transition
	//---------

	// Rotate if we are right of first point of next transline
	qreal
			nextX =
					this->getNextTransline()->getEndItem() != NULL ? this->getNextTransline()->getEndItem()->x()
							: this->getNextTransline()->getEndPoint().x();
	if (this->x() > nextX) {
		this->setRotation(180);
	} else
		this->setRotation(0);
}

Transline * LinkArrival::getNextTransline() {
	return this->nextTransline;
}


void LinkArrival::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {

	// Let parent do its stuff (placement and so on)
	QGraphicsPathItem::mouseReleaseEvent(event);

}

QVariant LinkArrival::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged) {

		if (value.toBool() == true) {

		} else {

			this->setOpacity(1.0);
		}

	} else if (change == QGraphicsItem::ItemPositionHasChanged) {

		//qDebug() << "LinkArrival moved " << this->getNextTransline() ;

		//-- Moved, propagate to translines around
		if (this->getNextTransline() != NULL) {
			this->getNextTransline()->preparePath();

			qreal nextX = this->getNextTransline()->getEndItem() != NULL ? this->getNextTransline()->getEndItem()->x()
										: this->getNextTransline()->getEndPoint().x();
			if (this->x() > nextX) {
				this->setRotation(180);
			} else
				this->setRotation(0);
		}


		//-- Record
		this->recordPosition();

	} else if (change == QGraphicsItem::ItemVisibleHasChanged) {

		//-- Propagate visibility to next
		this->getNextTransline()->setVisible(this->isVisible());

	} else if (change == QGraphicsItem::ItemSceneHasChanged && this->scene()==NULL) {

		//-- Removed from scene, propagate to Transline
		if (this->getNextTransline()!=NULL) {
			SGC::getInstance()->requestDelete(this->getNextTransline());
		}

	}

	return QGraphicsPathItem::itemChange(change, value);

}

void LinkArrival::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {

	// Always call parent
	QGraphicsPathItem::mouseMoveEvent(event);

	// Rotate if we are right of first point of next transline


}

bool LinkArrival::recordPosition() {


	this->model->setPosition(make_pair(this->pos().x(),this->pos().y()));

	// Default is not Modified
	return true;

}
