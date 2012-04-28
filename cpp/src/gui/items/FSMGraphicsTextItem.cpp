/*
 * FSMGraphicsTextItem.cpp
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */


// Include
//---------------

//-- Std
#include <iostream>
#include <cmath>
#include <typeinfo>

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

#include "FSMGraphicsTextItem.h"
FSMGraphicsTextItem::FSMGraphicsTextItem(QString &qs, QGraphicsItem* parent) :
		QGraphicsTextItem(qs, parent) {

	this->setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
	this->editorStartTime = NULL;
	this->setTextCursor(QTextCursor(this->document()));
	this->setTextInteractionFlags(Qt::NoTextInteraction);

}

/**
 * Starts editing the text
 */
void FSMGraphicsTextItem::startEditing() {

	//-- Store time
	this->editorStartTime = new QDateTime(QDateTime::currentDateTime());
	this->editorStartTime->time().start();


	qDebug() << " Start Editing " << endl;

	//-- Start Editing
	//QTextCursor cursor = this->textCursor();
	//cursor.beginEditBlock();
	//cursor.setPosition(1);
	//cursor.select(QTextCursor::Start);
	//this->setTextCursor(cursor);

	//-- Allow text editing
	this->setTextInteractionFlags(Qt::TextEditorInteraction);
	this->grabKeyboard();

}

/**
 * Stop editing the text
 */
void FSMGraphicsTextItem::stopEditing() {

	// Disable text editing
	// Check a minimum delay between editable mode entering and deselection
	if (this->editorStartTime !=NULL && this->editorStartTime->time().elapsed()>300) {

		qDebug() << " Stop editing " << endl;

		//-- Disable edition
		QTextCursor cursor = this->textCursor();
		cursor.clearSelection();
		//cursor.endEditBlock();
		this->setTextCursor(cursor);

		this->setTextInteractionFlags(Qt::NoTextInteraction);
		delete (this->editorStartTime);
		this->editorStartTime = NULL;

		this->ungrabKeyboard();


		//-- Record text
		this->recordText();
		this->adjustSize();

	}

}



void FSMGraphicsTextItem::keyPressEvent(QKeyEvent * event) {

	cout << "KEY Pressed IN FSMGTEXTITEM" << endl;

	// On Key Enter, stop editing
	if (((QKeyEvent*)event)->key()==Qt::Key_Enter || ((QKeyEvent*)event)->key()==Qt::Key_Return) {

		this->stopEditing();

	} else {
		// Let parent do
		QGraphicsTextItem::keyPressEvent(event);

		// Adjust size
		this->adjustSize();
		//this->update();
	}

}



QVariant FSMGraphicsTextItem::itemChange(GraphicsItemChange change,
		const QVariant & value) {

	// Selection Change
	//---------------------
	if (change == QGraphicsItem::ItemSelectedHasChanged && value.toBool()==false) {

		this->stopEditing();

	}

	return QGraphicsItem::itemChange(change, value);

}

void FSMGraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event ) {



	// Start Editing if not already
	//----------------------
	if (this->textInteractionFlags() == Qt::TextEditorInteraction) {
		//-- Parent
		QGraphicsTextItem::mouseDoubleClickEvent(event);
	} else {
		this->startEditing();
		//-- Simulate a Click at the position
		QGraphicsSceneMouseEvent * ev =  new QGraphicsSceneMouseEvent(QEvent::MouseButtonPress);
		ev->setButtonDownScenePos(Qt::LeftButton,event->scenePos());
		ev->setWidget(this->scene()->views().first());
		QGraphicsTextItem::mousePressEvent(ev);
	}





}

void FSMGraphicsTextItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {

	//-- Don't forget to call parent
	QGraphicsTextItem::mouseMoveEvent(event);

	// Clear selection if not editing
	if (this->textInteractionFlags() != Qt::TextEditorInteraction)
		this->scene()->clearSelection();

}

void FSMGraphicsTextItem::focusOutEvent ( QFocusEvent * event ) {

	//-- Parent call
	QGraphicsTextItem::focusOutEvent(event);
	//this->stopEditing();
	//qDebug() << "Focus Out call on text item ";


}


bool FSMGraphicsTextItem::recordPosition() {
	return false;
}

bool FSMGraphicsTextItem::recordText() {
	return false;
}

