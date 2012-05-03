/*
 * HyperTransition.cpp
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

//-- UI
#include <gui/scene/Scene.h>
#include <gui/settings/GuiSettings.h>
#include <gui/items/FSMGraphicsItem.h>

#include "HyperTransition.h"

HyperTransition::HyperTransition(Hypertrans * model) :
	QGraphicsEllipseItem() , FSMGraphicsItem() {

	// Item Flags
    //--------------------
	setFlags(ItemIsMovable | ItemIsSelectable);
	setToolTip("HyperTransition");

	// Set Model
	//--------------
	if (model==NULL) {
	    model = new Hypertrans();
	}
	this->setModel(model);

	// Default Rectangle view
	//------------------
	this->setRect(-10,-10,20,20);
}

HyperTransition::~HyperTransition() {

}

void HyperTransition::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setRenderHint(QPainter::Antialiasing);
	QGraphicsEllipseItem::paint(painter, option, widget);

	/*if (isSelected()) {
		painter->setPen(Qt::black);
		painter->setBrush(Qt::black);
		painter->drawRect(-(int) (rect().width() / 2) - 1, -1, 2, 2);
		painter->drawRect((int) (rect().width() / 2) - 1, -1, 2, 2);
		painter->drawRect(-1, -(int) (rect().height() / 2) - 1, 2, 2);
		painter->drawRect(-1, (int) (rect().height() / 2) - 1, 2, 2);
	}*/

}

void HyperTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {


	// Let parent do its stuff (placement and so on)
	QGraphicsEllipseItem::mouseReleaseEvent(event);


	// Record position modification
	if (this->recordPosition()==true) {

		// Rebuild
		//scene()->update();
		//((View*) this->scene()->views()[0])->redraw();

	}

}

QVariant HyperTransition::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged) {

		if (value.toBool() == true) {


		} else {

			this->setOpacity(1.0);
		}
	}

	return QGraphicsItem::itemChange(change, value);

}

bool HyperTransition::recordPosition() {

	// Test if we have to record
	this->getModel()->setPosition(make_pair(this->pos().x(), this->pos().y()));

	return true;
}
