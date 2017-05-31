/*
 * SelectionAnimator.cpp
 *
 *  Created on: May 10, 2010
 *      Author: rleys
 */

#include <QObject>
#include <QtGui>


#include "selectionanimator.h"

SelectionAnimator::SelectionAnimator(QGraphicsScene * scene) {

	// Connect to graphics scene
	//---------
	this->scene = scene;
	this->connect(this->scene,SIGNAL(selectionChanged()),SLOT(sceneSelectionChanged()));

	// Selection timer
	//-------------------------
	this->selectionEffectTimeline = new QTimeLine(2000,this);
	this->connect(this->selectionEffectTimeline, SIGNAL(valueChanged(qreal)), SLOT(selectionTimelineValue(qreal)));
	this->selectionEffectTimeline->setFrameRange(0, 100);
	this->selectionEffectTimeline->setLoopCount(0);
	this->selectionEffectTimeline->setCurveShape(QTimeLine::LinearCurve);
   // this->selectionEffectTimeline->setUpdateInterval(20);

	this->selectionEffectTimeline->start();

}

SelectionAnimator::~SelectionAnimator() {
	// TODO Auto-generated destructor stub
}

void SelectionAnimator::selectionTimelineValue(qreal value) {


	// Direction Togelling
	if (value>=0.7 && this->selectionEffectTimeline->direction()==QTimeLine::Forward ) {
			this->selectionEffectTimeline->toggleDirection();
	} else if (value<=0.35 && this->selectionEffectTimeline->direction()==QTimeLine::Backward) {
		this->selectionEffectTimeline->toggleDirection();
	}

	// Propagate to selected Items
	//-------------
	QList<QGraphicsItem*> items = this->scene->selectedItems();
	for (int i=0;i<items.size();i++) {

		// Get Item
		QGraphicsItem * item = items.at(i)->topLevelItem();

		// Change opacity
		item->setOpacity(value);

	}


}

void SelectionAnimator::sceneSelectionChanged() {

}


