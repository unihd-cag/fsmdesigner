/*
 * TrackpointItem.cpp
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */


//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
#include <core/Fsm.h>
#include <core/Core.h>
#include <core/State.h>
#include <core/Trackpoint.h>
#include <core/Trans.h>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/items/Transline.h>
#include <gui/settings/GuiSettings.h>

#include <gui/actions/DeleteTrackpointAction.h>

#include "TrackpointItem.h"

TrackpointItem::TrackpointItem(Trackpoint * model,QGraphicsItem * startItem,
		QGraphicsItem * endItem) :
	TrackpointBaseItem(model,startItem,endItem) {

	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable);
	setToolTip("TrackpointItem");
	this->setBrush(Qt::black);

	this->setZValue(200000);

	//-- Init Rect
	//-- Create QPAinterPath
	QPainterPath rectPath;
	rectPath.addRect(-3,-3,6,6);
	this->setPath(rectPath);



}

TrackpointItem::~TrackpointItem() {
	//hide();
}

QGraphicsScene * TrackpointItem::getScene() {
	return this->scene();
}

void TrackpointItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {


	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setOpacity(this->opacity());
	painter->setBrush(this->brush());

	//-- Call Paint
	QGraphicsPathItem::paint(painter,option,widget);

	painter->restore();

}

void TrackpointItem::modelChanged() {

	// Not selectable without model
	if (this->getModel() == NULL) {
		this->setFlag(ItemIsSelectable, false);
	} else {

		//-- Propagate Transition model to transitions
		if (this->getPreviousTransline()!=NULL &&  this->getModel()->getTransition()!=NULL) {
			this->getPreviousTransline()->setModel(this->getModel()->getTransition());
		}
		if (this->getNextTransline()!=NULL &&  this->getModel()->getTransition()!=NULL) {
				this->getNextTransline()->setModel(this->getModel()->getTransition());
		}


		this->setFlag(ItemIsSelectable, true);
	}
}

QVariant TrackpointItem::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	QGraphicsItem::itemChange(change, value);

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged) {

		if (value.toBool() == true) {

			//-- Selection propagation
			this->propagateSelection();

			this->selected();
		} else
			this->deselected();


	} else if (change == QGraphicsItem::ItemPositionHasChanged) {

		//-- Trackpoint moved, propagate to translines around
		// Propagate movement to previous-next transline to ensure correct redraw
		if (this->getPreviousTransline() != NULL)
			this->getPreviousTransline()->preparePath();
		if (this->getNextTransline() != NULL)
			this->getNextTransline()->preparePath();

		//-- Record
		this->recordPosition();

	} else if (change == QGraphicsItem::ItemSceneHasChanged) {


		//-- If there is no scene, we are going to be removed
		if (this->scene() == NULL) {

			// Start Item
			//-----------------

			//-- If trackpoint, its end is our present end
			if (this->getStartItem()!=NULL && FSMGraphicsItem<>::isTrackPoint(this->getStartItem())) {

				FSMGraphicsItem<>::toTrackPoint(this->getStartItem())->setEndItem(this->getEndItem());

			}
			//-- If no trackpoint, the transline is ours, so remove it
			else {

				//-- Delete Previous transline
			    this->getPreviousTransline()->scene()->removeItem(this->getPreviousTransline());
				//delete this->getPreviousTransline();
				this->setPreviousTransline(NULL);
			}

			// End Item
			//-----------------

			//-- Next transline is anyway ours, so remove it
			this->getNextTransline()->scene()->removeItem(this->getNextTransline());
			//delete this->getNextTransline();
			this->setNextTransline(NULL);

			//-- If trackpoint, its start is our present start
			if (this->getEndItem()!=NULL && FSMGraphicsItem<>::isTrackPoint(this->getEndItem())) {

				//-- Reset start
				FSMGraphicsItem<>::toTrackPoint(this->getEndItem())->setPreviousTransline(NULL);
				FSMGraphicsItem<>::toTrackPoint(this->getEndItem())->setStartItem(this->getStartItem());

			}

		}
		//-- Added to scene
		else {

			//-- Then record our start and end
			//------------------
			this->setStartItem(this->getStartItem());
			this->setEndItem(this->getEndItem());

			// First set ourselves in between previous and next if they are trackpoints
			//-------------------------

			//-- If previous is a trackpoint, it now ends on the new one
			if (this->getStartItem() != NULL && FSMGraphicsItem<>::isTrackPoint(this->getStartItem())) {
				FSMGraphicsItem<>::toTrackPoint(this->getStartItem())->setEndItem(this);
			}
			//-- If next is a trackpoint, it now starts on the new one
			if (this->getEndItem() != NULL && FSMGraphicsItem<>::isTrackPoint(this->getEndItem())) {
				FSMGraphicsItem<>::toTrackPoint(this->getEndItem())->setStartItem(this);
			}


			// Add A reversed delete (i.e add) action to allow to undo the add
			// !! only if not placing elements on scene! So when added to an existing transition
			//-------------------
			if (dynamic_cast<Scene*>(this->scene())->getPlaceMode()==FSMDesigner::CHOOSE) {
			    //-- Delete to fake Add action on command stack
                qDebug() << "Deleting for reverse";
                DeleteTrackpointAction * delAction = new DeleteTrackpointAction(this);
                delAction->setReversed(true);
                dynamic_cast<Scene *>(this->scene())->getUndoStack()->push(delAction);
			}






		}

	} else if (change == QGraphicsItem::ItemEnabledHasChanged) {

		//-- If not enabled, hide lines, it so, show them
		if (this->getPreviousTransline() != NULL)
				this->getPreviousTransline()->setVisible(this->isEnabled());
		if (this->getNextTransline() != NULL)
				this->getNextTransline()->setVisible(this->isEnabled());

		//-- Trackpoint moved, propagate to translines around
		// Propagate movement to previous-next transline to ensure correct redraw
		if (this->getPreviousTransline() != NULL)
			this->getPreviousTransline()->preparePath();
		if (this->getNextTransline() != NULL)
			this->getNextTransline()->preparePath();


	}

	return value;

}


void TrackpointItem::selected() {

	QPen newPen(Qt::red);
	newPen.setWidth(this->pen().width() + 1);
	this->setPen(newPen);
	this->setBrush(QBrush(Qt::red));
}

void TrackpointItem::deselected() {

	int width = GuiSettings::value("ui.TransLineWidth", QVariant(1)).toInt();
	this->setPen(QPen(Qt::black));
	this->setBrush(QBrush(Qt::black));
	this->pen().setWidth(width);
	this->setOpacity(1.0);
}


void TrackpointItem::mousePressEvent(QGraphicsSceneMouseEvent * event) {

	// Don't forget to call parent
	QGraphicsPathItem::mousePressEvent(event);

	// Clear selection
	//this->scene()->clearSelection();

}

void TrackpointItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {

	// Don't forget to call parent
	QGraphicsPathItem::mouseReleaseEvent(event);
	this->ungrabMouse();

	// Selection propagation
	//this->propagateSelection();

	// Validate new position
	if (this->recordPosition() == true) {

		// Rebuild scene
		//((View*) this->scene()->views()[0])->redraw();

	}

}

void TrackpointItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {

	// Don't forget to call parent
	QGraphicsPathItem::mouseMoveEvent(event);

	// Clear selection (to avoid propagated selection to come with)
	this->scene()->clearSelection();

}


void TrackpointItem::keyPressEvent ( QKeyEvent * event ) {

	// Move using arrows
	//-----------------------------
	if (event->key()==Qt::Key_Up) {
		this->moveBy(0,-1);
	} else if (event->key()==Qt::Key_Down) {
		this->moveBy(0,1);
	}else if (event->key()==Qt::Key_Right) {
		this->moveBy(1,0);
	}else if (event->key()==Qt::Key_Left) {
		this->moveBy(-1,0);
	}

	//-- Call parent
	QGraphicsPathItem::keyPressEvent(event);


}

void TrackpointItem::propagateSelection() {
	// Propagate selection change
	//-----------
	//cout << "Trackpoint Item selected: " << value.toBool() << "// "<< this->isSelected()<< endl;

	// To previous line
	//-----------------------
	if (this->getPreviousTransline() != NULL) {
		if (this->getPreviousTransline()->isSelected() != true)
			this->getPreviousTransline()->setSelected(true);
	}

	// To End Item
	//----------------
	if (this->getNextTransline() != NULL) {
		if (!this->getNextTransline()->isSelected()) {
			this->getNextTransline()->setSelected(true);
		}
	}
}

bool TrackpointItem::recordPosition() {

	//-- Check
	if (this->getModel() == NULL)
		return false;

	//-- Record
	this->model->setPosition(make_pair(this->pos().x(),this->pos().y()));


	return true;

}

QList<QUndoCommand*> TrackpointItem::remove() {

	//-- Prepare Undo list
	QList<QUndoCommand*> undoCommands;

	//-- Delete itself
	DeleteTrackpointAction * deleteAction = new DeleteTrackpointAction(this);
	undoCommands.append(deleteAction);


	return undoCommands;

}

