/*
 * StateItem.cpp
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */

//-- Std
#include <iostream>
using namespace std;

//-- Core
#include <core/Fsm.h>
#include <core/Trans.h>
#include <core/State.h>
#include <core/Link.h>

//-- Qt
#include <QtGui>
#include <QtCore>

//-- GUI
#include <gui/scene/Scene.h>

#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/FSMGraphicsTextItem.h>
#include <gui/items/Transline.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/LinkArrival.h>
#include <gui/items/JoinItem.h>

#include <gui/settings/GuiSettings.h>

//-- Actions
#include <gui/actions/DeleteStateAction.h>
#include <gui/actions/ChangeStateNameAction.h>
#include <gui/actions/MoveStateAction.h>

#include <common/SGC.h>

//-- Verification
#include <gui/verify/FSMVerifyError.h>

#include "StateItem.h"

StateItem::StateItem(State * model, QGraphicsItem * parent) :
	QGraphicsItemGroup(parent) {

	// TODO: Set state size parameterizable through preferences

	this->setZValue(200000);

	// Init variables
	//---------------------
	this->verificationMark = NULL;
	this->moveLoopBackTransitions = true;

	// Create Representation
	//-------------------------------

	// Create TextItem
	this->stateText = new StateItemText();
	this->stateText->setVisible(true);
	this->stateText->setZValue(2);
	this->addToGroup(this->stateText);

	// Create Ellipse
	this->stateEllipse = new StateItemEllipse(QRectF(0, 0, 50, 50));
	this->stateEllipse->setVisible(true);
	this->stateEllipse->setZValue(1);
	this->addToGroup(this->stateEllipse);

	// Set movable and selectable
	this->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable);
	this->show();
	this->setVisible(true);

	// Translate to have upper-left corner not be 0,0 but 0,0 be in the middle of the bounding area rect
	this->setTransform(QTransform::fromTranslate(-25, -25));


	// Parameters from model
	//------------
	this->setModel(model);

}

StateItem::~StateItem() {

		delete this->stateText;
		delete this->stateEllipse;
}


void StateItem::modelChanged() {

    FSMGraphicsItem<State>::modelChanged();

	//-- Propagate to ellipse
	this->stateEllipse->setModel(this->getModel());


	if (this->getModel() != NULL) {

	    //-- Position from model
        this->setPos(this->model->getPosition().first, this->model->getPosition().second);
        this->itemChange(QGraphicsItem::ItemPositionChange,QVariant(QPointF(this->pos())));

		//-- Update reset states and so on
	    this->stateText->setPlainText(QString::fromStdString(this->getModel()->getName()));
        //this->stateText->update();
		this->setToolTip("Output: "+QString::fromStdString(this->getModel()->getOutput()));

	}

	//this->update();

}

void StateItem::verificationErrorAdded(FSMVerifyError * error)  {

	if (this->verificationMark!=NULL)
		return;

	//-- There is now a verification error

	//-- Add an exclamation mark button
	QGraphicsPixmapItem * exclamation = new QGraphicsPixmapItem(QPixmap(
			":/icons/ExclamationMark2").scaled(24,24));
	exclamation->setToolTip(error->getMessage());
	exclamation->setParentItem(this);
	exclamation->setZValue(1000);

	exclamation->setTransform(QTransform::fromTranslate(-16, -16));
	this->addToGroup(exclamation);

	//-- Register Item as verification mark
	this->verificationMark = exclamation;


}

void StateItem::verificationErrorRemoved(FSMVerifyError * error)  {

	//-- If there is a verification mark, and no errors anymore, then remove
	if (this->verificationMark!=NULL && this->verificationErrors.size()==0) {

		//this->scene()->removeItem(this->verificationMark);
		//this->removeFromGroup(this->verificationMark);
		SGC::getInstance()->requestDelete(this->verificationMark);
		this->verificationMark = NULL;
	}

}


QString StateItem::text() {
    return this->stateText->toPlainText();
}


bool StateItem::recordText() {
	return false;
}

void StateItem::setText(QString s) {

	// Record text
	this->model->setName(s.toStdString());
	this->stateText->setPlainText(s);

    //this->setFont(QFont("Sans Serif",-1,QFont::Bold));

	//this->stateText->update();

	//-- Reevaluate and reposition the text in the middle

	// text size
	/*QFontMetrics fontMetrics = QFontMetrics(this->stateText->font());
	 int textWidth = fontMetrics.width(text());
	 textWidth += 5;
	 int textHeight = fontMetrics.height();
	 textHeight += 5;

	 // Rectmiddle
	 int middlex = this->stateEllipse->rect().width()/2;
	 int middley = this->stateEllipse->rect().height()/2;

	 // Pos
	 this->stateText->setPos(middlex-textWidth/2,middley-textHeight/2);
	 */

}

void StateItem::setStateItemColor(QColor color) {
	// Set Color
	this->model->setColor((unsigned int) color.value());
	this->stateEllipse->setColor(color);
}

QColor StateItem::getStateItemColor() {
	return this->stateEllipse->getColor();
}

void StateItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {

	// Antialiasing
	painter->setRenderHint(QPainter::Antialiasing);

	// Opacity propagation
	painter->setOpacity(this->effectiveOpacity());

	// Just delegate normaly for now
	QGraphicsItemGroup::paint(painter, option, widget);

	//cout << "Repainting item " << endl;

	return;
}

QRectF StateItem::boundingRect() const {

	//-- Get Parent Bounding rect
	QRectF parent = QGraphicsItemGroup::boundingRect();

	//-- Increase to match reset radius?
	if (this->isVisible() && this->stateEllipse!=NULL && model!=NULL && model->isReset()) {
	    parent.adjust(-7,-7,7,7);
	}

	//-- If text is bigger than ourselves, adjust boundingRec
	/*if (textrect.width() > parent.width()) {
	    parent.setWidth(textrect.width());
	    parent.moveLeft(textrect.width()/2);
	}*/

	return parent;

}

QVariant StateItem::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedChange) {

		// Propagate to text
		//this->stateText->setSelected(value.toBool());

		if (value.toBool() == true) {
			// Do nothing at the moment
		} else {
			// Unselected, assure opacity to 1
			this->setOpacity(1);
		}

	} else if (change == QGraphicsItem::ItemTransformHasChanged) {

		// Geometry has changed
		// Update Gradient

		this->stateEllipse->prepareGraphics();


	} else if (change == QGraphicsItem::ItemPositionChange) {

		//-- New position
		QPointF newPosition = value.toPointF();

		//-- Move loop-back transitions with state
		//------------------------------------------------
		if (this->moveLoopBackTransitions)
			for (int i = 0; i < this->outgoingTransitions.size(); i++) {

				//-- Get Transition
				Transline * tr = this->outgoingTransitions.at(i);


				//-- Look if model loops
				//-- Transition might not have a model if we are adding it
				if (tr->getModel()==NULL || (((Trans*)tr->getModel()))->getStartState() !=  (((Trans*)tr->getModel()))->getEndState())
					continue;

				//-- Calculate Dx/Dy
				qreal dx = newPosition.x() - this->pos().x();
				qreal dy = newPosition.y() - this->pos().y();

				//-- Yes, go through connections to move items
				QGraphicsItem * currentItem = tr->getEndItem();
				do {

					if (currentItem != NULL && currentItem->type()
							== TrackpointItem::Type) {


						//-- Move Trackpoint
						currentItem->moveBy(dx, dy);

						//-- Go to next trackpoint if there is one
						currentItem
								= dynamic_cast<TrackpointItem*> (currentItem)->getNextTransline()->getEndItem();

					} else {
						break;
					}

				} while (currentItem != this);

				//-- Move text with it
				QGraphicsItem* textItem = this->scene()->itemAt(
				        (((Trans*)tr->getModel()))->getTextPosition().first,
				        (((Trans*)tr->getModel()))->getTextPosition().second);
				if (textItem != NULL) {
					textItem->moveBy(dx, dy);
				}

			}
		//--- EOF Loopback move with


		//-- Update incoming-outgoing transitions
		for (int i = 0; i < this->incomingTransitions.size(); i++) {
			this->incomingTransitions.at(i)->preparePath();
		}
		for (int i = 0; i < this->outgoingTransitions.size(); i++) {
			this->outgoingTransitions.at(i)->preparePath();
		}

	} else if (change == QGraphicsItem::ItemSceneHasChanged && this->scene()
			== NULL) {

		//-- If removed from scene
		//-- Ensure the incoming transitions forget about us
		for (QList<Transline*>::iterator it = this->incomingTransitions.begin(); it
				< this->incomingTransitions.end(); it++)
			(*it)->setEndItem(NULL);
		this->incomingTransitions.clear();

		//-- Ensure the outgoing transitions forget about us
		for (QList<Transline*>::iterator it = this->outgoingTransitions.begin(); it
				< this->outgoingTransitions.end(); it++)
			(*it)->setStartItem(NULL);
		this->outgoingTransitions.clear();

	} else if (change==QGraphicsItem::ItemScaleHasChanged && this->scene()!=NULL) {

		//-- Added to scene
		//----------------------
		//-- Save position
		if (this->recordPosition() == true) {

		}

	}

	return QGraphicsItemGroup::itemChange(change, value);

}

void	  StateItem::focusOutEvent ( QFocusEvent * event ) {

	QGraphicsItemGroup::focusOutEvent(event);

	if (this->stateText->textInteractionFlags() & Qt::TextEditorInteraction)
	    this->stateText->stopEditing();

}

void StateItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) {

	QGraphicsItemGroup::mouseDoubleClickEvent(event);


	//-- Start Editing text if we are under it
	if (this->scene()->items(event->scenePos()).contains(this->stateText) && !(this->stateText->textInteractionFlags() & Qt::TextEditorInteraction)) {
	    this->setSelected(false);
	    this->stateText->startEditing();
		event->accept();
	}


}

void StateItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {

	// Let parent do its stuff (placement and so on)
	QGraphicsItemGroup::mouseReleaseEvent(event);


	//-- Save position when mouse is released to avoid too fine graining
    if (this->recordPosition() == true) {

    }

}

void StateItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {

	//-- Let parent do its work
	QGraphicsItemGroup::mouseMoveEvent(event);

	//-- If we have CTRL activated, don't move looping transitions with
	if ((event->modifiers() & Qt::CTRL)) {
		this->moveLoopBackTransitions = false;
	} else
		this->moveLoopBackTransitions = true;


}

void StateItem::keyReleaseEvent(QKeyEvent * event) {

	//-- Call Parent
	QGraphicsItemGroup::keyReleaseEvent(event);


	// Start Editing on F2
	//--------------------------
	if (event->key() == Qt::Key_F2) {
	    this->setSelected(false);
	    this->stateText->startEditing();
	}

	/*if (event->isAccepted())
		return;

	if (this->stateText->hasFocus()) {
		// Don't do anything if text is active
	} else {
		FSMGraphicsItem<State>::keyRelease(event);
	}
*/


}

void StateItem::keyPressEvent ( QKeyEvent * event ) {

	//-- Call parent
	QGraphicsItemGroup::keyPressEvent(event);

	if (this->stateText->textInteractionFlags() & Qt::TextEditorInteraction) {
		return;
	}

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

	event->accept();



}

bool StateItem::recordPosition() {

	// Get FSM
	if (this->scene() == NULL || this->getModel() == NULL)
		return false;

	// If position is already the one in the model -> don't do anything (to avoid loop on actions)
	//-----------
	if (this->x()==this->getModel()->getPosition().first && this->y()==this->getModel()->getPosition().second)
	    return false;

	// Record using an action
	///------------------
	MoveStateAction * moveState = new MoveStateAction(this->pos(),this);
	dynamic_cast<Scene*>(this->scene())->getUndoStack()->push(moveState);


	// Modified
    return true;


}

QList<QUndoCommand*> StateItem::remove(QUndoCommand * parentCommand) {

	//-- Prepare Undo list
	QList<QUndoCommand*> undoCommands;

	// Remove itself
	//----------------------------
	DeleteStateAction * deleteThis = new DeleteStateAction(this);
	undoCommands.append(deleteThis);

	// Remove transitions
	//-------------------------
//	QList<QUndoCommand*> transitionUndoCommands;

//	//-- Outgoing
//	while (!this->outgoingTransitions.isEmpty()) {
//		this->outgoingTransitions.takeFirst()->remove(deleteThis);
//	}
//
//	//-- Incoming
//	while (!this->incomingTransitions.isEmpty()) {
//
//		//-- If Incoming if also outgoing (loopback), then do not double remove
//		Transline * t = this->incomingTransitions.takeFirst();
//		if (t->getModel()!=NULL && t->getModel()->getStartState() != t->getModel()->getEndState())
//			t->remove(deleteThis);
//
//	}


	//-- Add All transitions undo to the list
//	undoCommands.append(transitionUndoCommands);

	return undoCommands;

}

void StateItem::addIncomingTransition(Transline * transition) {
	this->incomingTransitions.append(transition);
}

void StateItem::removeIncomingTransition(Transline * transition) {
	this->incomingTransitions.removeOne(transition);
}

QList<Transline*>& StateItem::getIncomingTransitions() {
    return this->incomingTransitions;
}

void StateItem::addOutgoingTransition(Transline * transition) {

    // If another transline in outgoing if for the same transition model -> remove it
    for (int i=0;i<this->outgoingTransitions.size();i++) {

        Transline * otransition = this->outgoingTransitions[i];
        if (
                (otransition->getEndItem() !=NULL)
                && (otransition->getEndItem()->type()==StateItem::Type || otransition->getEndItem()->type()==JoinItem::Type)
                && (otransition->getStartItem()!=otransition->getEndItem())
                && otransition->getModel()!=NULL
                && transition->getModel()!=NULL
                && otransition->getModel()==transition->getModel()) {
            this->outgoingTransitions.removeAll(otransition);
            //otransition->setVisible(false);
            this->scene()->removeItem(otransition);
            //delete otransition;

        }

    }

    // Add to outgoing
	this->outgoingTransitions.append(transition);
}

void StateItem::removeOutgoingTransition(Transline * transition) {
	this->outgoingTransitions.removeOne(transition);
}

QList<Transline*>& StateItem::getOutgoingTransitions() {
    return this->outgoingTransitions;
}

/** \defgroup Items Utils */
/**@{*/

LinkArrival * StateItem::findLinkArrival() {

	//-- Prepare result
	LinkArrival * result = NULL;

	// Go through incoming transitions
	//-------------------
	for (QList<Transline*>::iterator it = this->incomingTransitions.begin(); it
					< this->incomingTransitions.end(); it++) {

		//-- If start is a LinkArrival it is found
		if ((*it)->getStartItem()->type()==LinkArrival::Type) {
			return dynamic_cast<LinkArrival*>((*it)->getStartItem());
		}

	}
	return result;


}

/**@}*/


StateItemEllipse::StateItemEllipse(const QRectF& rect) :
	QGraphicsEllipseItem(rect) {

	//this->color = Qt::red;
	setFlag(ItemIsSelectable, false);

	// Default Color is Red
	//----------
	this->setColor(Qt::red);
}

StateItemEllipse::~StateItemEllipse() {

}

void StateItemEllipse::modelChanged() {

    if (this->getModel()==NULL)
        return;

	//-- Propagate Color
	this->setColor(QColor::fromRgb(this->model->getColor()));


	//-- Update a full radius as if it were reset, so that if reset was just removed, no traces of the second ellipse stays
	QRectF updateRect = QGraphicsEllipseItem::boundingRect();
	updateRect.adjust(-7, -7, 7, 7);
	this->update(updateRect);

}

QRectF StateItemEllipse::boundingRect() const {

	//-- Get Parent Bounding rect
	QRectF parent = QGraphicsEllipseItem::boundingRect();

	//-- Increase to match reset radius?
	/*if (model!=NULL && model->isReset()) {
		parent.adjust(-7, -7, 7, 7);
	}*/

	return parent;

}

/**
 * Prepares the graphics of this item (reset appearance, gradient etc...)
 */
void StateItemEllipse::prepareGraphics() {

	bool futureDisplay = GuiSettings::value(SETTINGS_STATE_FUTURE, QVariant(
				SETTINGS_STATE_FUTURE_DEFAULT)).toBool();

	futureDisplay = true;


	//-- Set Brush with item color
	if (!futureDisplay) {

		this->setBrush(QBrush(this->getColor()));
		this->setPen(QPen(Qt::black));


	}

	// Future Display
	//------------------------
	else {

		// Ball Like Gradient
		//		QRadialGradient gradient(-this->rect().width()/4, -this->rect().height()/4, (this->rect().width() / 2) + 6.5);
		//		gradient.setColorAt(0, Qt::white);
		//		gradient.setColorAt(0.5, this->stateItemColor);
		//gradient.setColorAt(1, Qt::white);

		// Button relief effect
		//---------------------------------

		QRadialGradient gradient((this->rect().width() / 2)
				+ 5, (this->rect().height() / 2) + 5,
				(this->rect().width() / 2) + 6.5);
		gradient.setColorAt(0, this->getColor());
		gradient.setColorAt(0.8, this->getColor());
		gradient.setColorAt(1, Qt::white);

		QBrush gradientBrush(gradient);
		this->setBrush(gradient);
		this->setPen(QPen(Qt::black));

	}



}


void StateItemEllipse::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->save();

	// Antialiasing
	painter->setRenderHint(QPainter::Antialiasing);

	// Apply opacity
	painter->setOpacity(this->effectiveOpacity());

	bool futureDisplay = GuiSettings::value(SETTINGS_STATE_FUTURE, QVariant(
			SETTINGS_STATE_FUTURE_DEFAULT)).toBool();

	//bool futureDisplay = true;


	//-- handle the reset state double ellipse
	//------------------------
	if (this->getModel()!=NULL && this->getModel()->isReset()) {



		painter->setPen(this->pen());
		painter->drawEllipse(QPoint(rect().width() / 2, rect().height() / 2),
				(int) ((rect().width() / 2) + 7), (int) ((rect().height() / 2)
						+ 7));

	}

	//-- Paint Ellipse

	// We want to paint ellipse from center
	painter->translate(-this->transform().dx(), -this->transform().dy());

	//-- Draw Ellipse
	painter->setBrush(this->brush());
	painter->setBackground(this->brush());
	painter->setPen(this->pen());
	painter->drawEllipse(QPoint(rect().width() / 2, rect().height() / 2),
			(int) ((rect().width() / 2)), (int) ((rect().height() / 2)));

	/*// Standard Style
	//-------------------------------
	if (!futureDisplay) {

		//-- Paint ellipse
		painter->setBrush(this->brush());
		painter->setBackground(this->brush());
		painter->setPen(this->pen());
		painter->drawEllipse(QPoint(rect().width() / 2, rect().height() / 2),
				(int) ((rect().width() / 2)), (int) ((rect().height() / 2)));

	}
	// Future Display Style
	//---------------------------
	else {





	}*/

	painter->restore();
	// First let parent do paint;
	//QGraphicsEllipseItem::paint(painter,option,widget);
}

void StateItemEllipse::keyReleaseEvent(QKeyEvent * event) {


}

QList<QUndoCommand*> StateItemEllipse::remove() {

	return QList<QUndoCommand*>();

}

StateItemText::StateItemText(QGraphicsItem* parent) : FSMGraphicsTextItem(*(new QString("String")),parent) {

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    this->setFlag(QGraphicsItem::ItemIsFocusable,true);

     QFont font = this->font();
     font.setBold(true);
     this->setFont(font);


}

StateItemText::~StateItemText() {

}

bool StateItemText::recordText() {

    // Check scene
    if (this->scene() == NULL)
            return false;

    // Record using action
    //---------------
    ChangeStateNameAction * changeName = new ChangeStateNameAction(this->toPlainText(),dynamic_cast<StateItem*>(this->parentItem()));
    dynamic_cast<Scene*>(this->scene())->getUndoStack()->push(changeName);


	return true;
}

void StateItemText::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->save();

	// Antialiasing
	painter->setRenderHint(QPainter::Antialiasing);

	// Apply opacity
	painter->setOpacity(this->effectiveOpacity());

	// Draw text background
	//---------------------

	// Translate to middle of bounding rect

	painter->setOpacity(0.5);
	painter->translate(this->boundingRect().width() / 2,
			this->boundingRect().height() / 2);

	int aboveBorder = 20;

	// Background to text depending on text size
	// Estimate text size
	QFontMetrics fontMetrics = painter->fontMetrics();
	int textWidth = fontMetrics.width(this->toPlainText());
	textWidth += 10;
	int textHeight = fontMetrics.height();
	textHeight += 5;

	// Draw Rect
	QColor settingsTextColor = GuiSettings::value(SETTINGS_STATE_TEXT_BG,
			QVariant(SETTINGS_STATE_TEXT_BG_DEFAULT)).value<QColor> ();
	if (settingsTextColor != painter->brush().color())
		painter->setBrush(QBrush(settingsTextColor));

	//		painter->setPen(QPen(Qt::DotLine));
	painter->drawRoundRect(QRect(-textWidth / 2, -textHeight / 2, textWidth,
			textHeight), 10, 10);

	// Points pattern
	painter->setBrush(QBrush(Qt::lightGray, Qt::Dense4Pattern));
	painter->drawRoundRect(QRect(-textWidth / 2, -textHeight / 2, textWidth,
			textHeight), 10, 10);


	painter->restore();


	// Replace the text if necessary
	//-----------
	int middlex = 50 / 2;
	int middley = 50 / 2;
	QPointF correctPosition(middlex - textWidth / 2, middley - textHeight / 2);
	if (this->pos().x() != correctPosition.x() || this->pos().y() != correctPosition.y() ) {
		this->setPos(correctPosition);
	}


	// Delegate Text painting to parent
	FSMGraphicsTextItem::paint(painter, option, widget);

}

void StateItemText::stopEditing() {

    if (this->textInteractionFlags() & Qt::TextEditorInteraction) {

        //-- Let parent do its job first
        FSMGraphicsTextItem::stopEditing();



        //-- Remove as filter
        if (this->parentItem()!=NULL) {
            this->parentItem()->removeSceneEventFilter(this);
            this->parentItem()->setFlag(ItemIsMovable,true);
            this->parentItem()->setFlag(ItemIsSelectable,true);
        }

        //-- Do a scene repaint for Artefacts
        this->scene()->update();

    }
}

void StateItemText::startEditing() {

    if (!(this->textInteractionFlags() & Qt::TextEditorInteraction)) {

        //-- Let parent do its job first
        FSMGraphicsTextItem::startEditing();


        //-- Set ourselves as filter
        this->parentItem()->installSceneEventFilter(this);
        this->parentItem()->setFlag(ItemIsMovable,false);
        this->parentItem()->setFlag(ItemIsSelectable,false);
    }
}

bool StateItemText::sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) {

	//event->setAccepted(true);

	//this->sceneEvent(event);
	event->accept();

	return true;
}

bool StateItemText::sceneEvent(QEvent * event) {

	bool res = FSMGraphicsTextItem::sceneEvent(event);


	 if (this->textInteractionFlags() & Qt::TextEditorInteraction) {


		// If we are editing, we can only act on the text field for mouse and keyboard
		// So we have to propagate events
		switch (event->type()) {


		case QEvent::GraphicsSceneMouseMove:
			((QGraphicsSceneMouseEvent*) event)->setPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->pos()));
						((QGraphicsSceneMouseEvent*) event)->setLastPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->lastPos()));
			this->mouseMoveEvent((QGraphicsSceneMouseEvent*) event);
			event->accept();
			return true;
			break;

		case QEvent::GraphicsSceneMousePress:

			((QGraphicsSceneMouseEvent*) event)->setPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->pos()));
			((QGraphicsSceneMouseEvent*) event)->setLastPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->lastPos()));
			event->accept();
			this->mousePressEvent((QGraphicsSceneMouseEvent*) event);
			return true;
			break;

		case QEvent::GraphicsSceneMouseRelease:
			((QGraphicsSceneMouseEvent*) event)->setPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->pos()));
						((QGraphicsSceneMouseEvent*) event)->setLastPos(this->mapFromItem(this->parentItem(),((QGraphicsSceneMouseEvent*) event)->lastPos()));
			this->mouseReleaseEvent((QGraphicsSceneMouseEvent*) event);
			event->accept();
			return true;
			break;

		case QEvent::KeyPress:
			this->keyPressEvent((QKeyEvent*) event);
			event->setAccepted(true);
			event->accept();
			return true;
			break;
		case QEvent::KeyRelease:
			QGraphicsItem::keyReleaseEvent((QKeyEvent*) event);
			event->accept();
			return true;
			break;

		default:
			break;
		}
	}

	//  Let parent do
	return res;

}



