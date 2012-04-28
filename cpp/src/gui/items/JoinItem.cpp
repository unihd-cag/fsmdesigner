/*
 * JoinItem.cpp
 *
 *  Created on: Oct 21, 2010
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
#include <QtGui/QPainter>
#include <QtGui/QGraphicsScene>
#include <QtCore/QRect>
#include <QtGui/QPainterPath>
#include <QtGui/QPainterPathStroker>

//-- Core
#include <core/Fsm.h>
#include <core/Core.h>
#include <core/State.h>
#include <core/Trackpoint.h>
#include <core/Join.h>

//-- UI
#include <gui/scene/Scene.h>
#include <gui/settings/GuiSettings.h>
#include <gui/items/Transline.h>
#include <gui/items/FSMGraphicsItem.h>

#include "JoinItem.h"

JoinItem::JoinItem(Join * model) : QGraphicsRectItem() {

	// Init
	//---------
	this->outgoingTransition = NULL;

	// Model
	//------------
	if (model!=NULL)
		this->setModel(model);
	else
		this->setModel(new Join());

	// UI Flags
	//--------------
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);



	// Form
	//----------------
	qreal w = 50;
	qreal h = 5;
	this->setRect(-w/2,-h/2,w,h);
	this->setBrush(QBrush(Qt::black,Qt::SolidPattern));
}

JoinItem::~JoinItem() {
}



void JoinItem::modelChanged() {

	if (this->getModel()==NULL)
		return;

	//-- Update position
	this->setPos(this->getModel()->getPosx(),this->getModel()->getPosy());

	qDebug()  << "Placing JoinItem at: " <<this->pos();

	//- Update tooltip
	this->setToolTip(QString("JoinItem")+QString::number(this->getModel()->getId()));

}

void JoinItem::addIncomingTransition(Transline * transition) {
	this->incomingTransitions.append(transition);
}

QList<Transline*> JoinItem::getIncomingTransitions() {
	return this->incomingTransitions;
}

void JoinItem::removeIncomingTransition(Transline * transition) {
	this->incomingTransitions.removeOne(transition);
}

void JoinItem::setOutgoingTransition(Transline *transition) {
	this->outgoingTransition = transition;
}

void JoinItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHint(QPainter::Antialiasing);


	QGraphicsRectItem::paint(painter, option, widget);
}


void JoinItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {


	// Let parent do its stuff (placement and so on)
	QGraphicsRectItem::mouseReleaseEvent(event);


	// Record position modification
	if(this->recordPosition()==true) {
		// Rebuild
		//((View*) this->scene()->views()[0])->redraw();
	}
//	scene()->update();



}

QVariant JoinItem::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged) {

		if (value.toBool() == true) {


		} else {

			this->setOpacity(1.0);
		}
	} else if (change ==  QGraphicsItem::ItemPositionHasChanged) {

		//-- Record new position
		this->recordPosition();

		//-- Propagate to incoming-outgoing transitions
		for (int i = 0; i < this->incomingTransitions.size(); i++) {
			this->incomingTransitions.at(i)->preparePath();
		}
		if (this->outgoingTransition!=NULL)
			this->outgoingTransition->preparePath();

	}

	return QGraphicsItem::itemChange(change, value);

}

bool JoinItem::recordPosition() {

	this->getModel()->setPosx(this->pos().x());
	this->getModel()->setPosy(this->pos().y());

	return true;

	/*Fsm& f = *(dynamic_cast<Scene *>(this->scene())->getFsm());
	f.getTransbyID(transid);
	f.getTransTrackpointbyID(id);
	int goal = f.getTransEnd();
	double jposx = f.getTransTrackpointPosx();
	double jposy = f.getTransTrackpointPosy();
	bool modified = false;

	if (jposx==this->pos().x() && jposy == this->pos().y())
		return modified;

	if (f.getFirstTrans()) {
		do {
			if (f.getTransEnd() == goal) {
				if (f.getTransFirstTrackpoint()) {
					do {
						if (f.getTransTrackpointJoin()) {
							if ((f.getTransTrackpointPosx() == jposx)
									&& (f.getTransTrackpointPosy() == jposy)) {
								int curtransid = f.getTransID();
								int curid = f.getTransTrackpointID();

								modified=true;

								f.modifyTrans(curtransid, curid, this->pos().x(), this->pos().y());

								while (f.getTransNextTrackpoint()) {
								}
							}
						}
					} while (f.getTransNextTrackpoint());
				}
			}
		} while (f.getNextTrans());
	}

	return modified;
	*/

}
