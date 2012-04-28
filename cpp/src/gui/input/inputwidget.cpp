/***********************************************************************
 *                                                                      *
 * (C) 2009, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/


// Includes
//-----------------

//-- Qt
#include <QtGui>

//-- Core
#include <core/State.h>
#include <core/Trans.h>
#include <core/Fsm.h>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/scene/SceneRelatedObject.h>

#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>

#include <gui/input/inputmodel.h>
#include <gui/input/inputdelegate.h>
#include <gui/input/newinput.h>
#include <gui/input/renameinput.h>

#include <gui/actions/DeleteInputAction.h>


#include "inputwidget.h"


InputWidget::InputWidget(QWidget* parent) :
	QTreeView(parent) {

	//-- Create  Models
	this->setModel(new InputModel());
	this->setItemDelegate(new InputDelegate());
	this->setFocusPolicy(Qt::StrongFocus);

	//-- Actions
	createActions();
}

InputWidget::~InputWidget() {

}

void InputWidget::contextMenuEvent(QContextMenuEvent* e) {
	QMenu contextMenu(this);

	//if(this->currentItem()) {
	if (this->indexAt(e->pos()).isValid()) {
		contextMenu.addAction(renameAction);
		contextMenu.addAction(inputupAction);
		contextMenu.addAction(inputdownAction);
		contextMenu.addAction(deleteselectedinputAction);
		contextMenu.addSeparator();
	}
	contextMenu.addAction(newinputAction);
	contextMenu.addAction(deleteinputAction);
	contextMenu.exec(e->globalPos());

}

void InputWidget::keyPressEvent ( QKeyEvent * event )  {



	//-- If CTRL + UP
	if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Up) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If 0 or is the first one, nothing to do
		if (index > 0) {

			this->getRelatedScene()->getFsm()->moveupInput(index);
			this->doItemsLayout();
			event->accept();
		}
	}
	//-- If CTRL + DOWN
	else if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Down) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If < 0 || is the last one, do nothing
		if (index>=0 && index < (this->getRelatedScene()->getFsm()->getNumberOfInputs()-1)) {
			this->getRelatedScene()->getFsm()->movedownInput(index);
			this->doItemsLayout();
			event->accept();
		}

	}

	//-- Call parent
	QTreeView::keyPressEvent(event);

}

void InputWidget::keyReleaseEvent ( QKeyEvent * event )  {

	//-- Call parent
	QTreeView::keyReleaseEvent(event);

	//qDebug() << "Krelease in input widget";


	//-- If SUPPR, remove //// Needed to secure ?? : event->modifiers() == Qt::CTRL
	if ( event->key() == Qt::Key_Delete) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If < 0, nothing to do
		if (index >= 0) {

		    this->getRelatedScene()->getUndoStack()->push(
		                        new DeleteInputAction(index,this->getRelatedScene()->getFsm(),NULL,this));
			this->doItemsLayout();
			event->accept();
		}
	}




}



void InputWidget::createActions() {

	//-- Rename input
	renameAction = new QAction(this);
	renameAction->setObjectName(QString::fromUtf8("rename"));
	renameAction->setEnabled(true);
	renameAction->setText(QApplication::translate("InputWidget", "Rename", 0,
			QApplication::UnicodeUTF8));
	renameAction->connect(renameAction,SIGNAL(activated()),this,SLOT(rename()));

	//-- New input
	newinputAction = new QAction(this);
	newinputAction->setObjectName(QString::fromUtf8("newInput"));
	newinputAction->setEnabled(true);
	newinputAction->setText(QApplication::translate("InputWidget", "New Input", 0,
			QApplication::UnicodeUTF8));
	newinputAction->connect(newinputAction,SIGNAL(activated()),this,SLOT(newInput()));

	//-- Delete last
	deleteinputAction = new QAction(this);
	deleteinputAction->setObjectName(QString::fromUtf8("deleteInput"));
	deleteinputAction->setEnabled(true);
	deleteinputAction->setText(QApplication::translate("InputWidget",
			"Delete last Input", 0, QApplication::UnicodeUTF8));
	deleteinputAction->connect(deleteinputAction,SIGNAL(activated()),this,SLOT(deleteLast()));

	//-- Delete selected
	deleteselectedinputAction = new QAction(this);
	deleteselectedinputAction->setObjectName(QString::fromUtf8("deleteSelectedInput"));
	deleteselectedinputAction->setEnabled(true);
	deleteselectedinputAction->setText(QApplication::translate("InputWidget",
			"Delete selected Input", 0, QApplication::UnicodeUTF8));
	deleteselectedinputAction->connect(deleteselectedinputAction,SIGNAL(activated()),this,SLOT(deleteSelected()));

	//-- Input Up
	inputupAction = new QAction(this);
	inputupAction->setObjectName(QString::fromUtf8("moveupInput"));
	inputupAction->setEnabled(true);
	inputupAction->setText(QApplication::translate("InputWidget", "Move up", 0,
			QApplication::UnicodeUTF8));
	inputupAction->connect(inputupAction,SIGNAL(activated()),this,SLOT(moveUp()));

	//-- Input Down
	inputdownAction = new QAction(this);
	inputdownAction->setObjectName(QString::fromUtf8("movedownInput"));
	inputdownAction->setEnabled(true);
	inputdownAction->setText(QApplication::translate("InputWidget", "Move down", 0,
			QApplication::UnicodeUTF8));
	inputdownAction->connect(inputdownAction,SIGNAL(activated()),this,SLOT(moveDown()));
}

void InputWidget::newInput() {
	NewInput* Input = new NewInput(this->getRelatedScene()->getFsm(),this);
	Input->exec();
	this->doItemsLayout();
}

void InputWidget::rename() {

	RenameInput* ReIn = new RenameInput(this->getRelatedScene(),this->currentIndex().row(),this);
	ReIn->exec();
	this->doItemsLayout();
}

void InputWidget::deleteLast() {

	int i = this->getRelatedScene()->getFsm()->getNumberOfInputs();
	if (i > 0) {

	    this->getRelatedScene()->getUndoStack()->push(
	            new DeleteInputAction(i--,this->getRelatedScene()->getFsm(),NULL,this));

	}
	this->doItemsLayout();
}

void InputWidget::deleteSelected() {

	int i = this->currentIndex().row();

	this->getRelatedScene()->getUndoStack()->push(
	                new DeleteInputAction(i,this->getRelatedScene()->getFsm(),NULL,this));

	this->doItemsLayout();
}

void InputWidget::moveUp() {


	int i = this->currentIndex().row();
	this->getRelatedScene()->getFsm()->moveupInput(i);
	this->doItemsLayout();
}

void InputWidget::moveDown() {

	int i =this->currentIndex().row();
	this->getRelatedScene()->getFsm()->movedownInput(i);
	this->doItemsLayout();
}



void InputWidget::setRelatedScene(Scene * scene) {

	//-- Disconnection
	if (this->getRelatedScene() != NULL) {
		QObject::disconnect(this->getRelatedScene(),
				SIGNAL(selectionChanged()), this, SLOT(selectionUpdated()));
	}

	//-- Set Scene
	SceneRelatedObject::setRelatedScene(scene);

	if (this->getRelatedScene() != NULL) {
		//-- Connection
		this->connect(this->getRelatedScene(), SIGNAL(selectionChanged()),
				SLOT(selectionUpdated()));
	}

	//-- Propagate
	dynamic_cast<SceneRelatedObject*> (this->model())->setRelatedScene(scene);
	dynamic_cast<SceneRelatedObject*> (this->itemDelegate())->setRelatedScene(
			scene);

	//-- No selection to startup
	this->selectionUpdated();


    //-- Update
    this->doItemsLayout();


}

void InputWidget::selectionUpdated() {

    //-- Selection Not updated if NULL
    if (this->getRelatedScene()==NULL)
        return;

	//-- Get First selected Item in list
	QList<QGraphicsItem*> selectedItems = this->getRelatedScene()->selectedItems();
	if (selectedItems.length() == 0) {

		//-- No Selection, edit none
		dynamic_cast<InputModel*> (this->model())->editNone();
		dynamic_cast<InputDelegate*> (this->itemDelegate())->editNone();

	} else {

		for (QList<QGraphicsItem*>::iterator it = selectedItems.begin(); it!= selectedItems.end(); it++) {

			QGraphicsItem* firstSelected = (*it)->topLevelItem();

			//-- Switch on type
			if (firstSelected->type()==Transline::Type && FSMGraphicsItem<>::toTransline(firstSelected)->getModel()!=NULL) {

				dynamic_cast<InputModel*> (this->model())->editTransline(dynamic_cast<Transline*>(firstSelected));
				dynamic_cast<InputDelegate*> (this->itemDelegate())->editTransline(dynamic_cast<Transline*>(firstSelected));

				break;
			} else {

				//-- Nothing Special, edit none
				dynamic_cast<InputModel*> (this->model())->editNone();
				dynamic_cast<InputDelegate*> (this->itemDelegate())->editNone();

			}
		}

	}

	//-- Update
    this->doItemsLayout();


}

