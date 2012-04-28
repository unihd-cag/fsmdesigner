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
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>

#include <gui/output/outputmodel.h>
#include <gui/output/outputdelegate.h>
#include <gui/output/renameoutput.h>
#include <gui/output/newoutput.h>

#include <gui/actions/DeleteOutputAction.h>

#include "outputwidget.h"

OutputWidget::OutputWidget(QWidget* parent) :
	QTreeView(parent) {

	//-- Create  Models
	this->setModel(new OutputModel());
	this->setItemDelegate(new OutputDelegate());

	//-- Actions
	createActions();

}

OutputWidget::~OutputWidget() {

}

void OutputWidget::keyPressEvent ( QKeyEvent * event )  {



	//-- If CTRL + UP : Move up
	//-------------------
	if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Up) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If < 0 or is the first one, nothing to do
		if (index > 0) {

			this->getRelatedScene()->getFsm()->moveupOutput(index);
			this->doItemsLayout();
		}
	}
	//-- If CTRL + DOWN : Move down
	//----------------------
	else if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Down) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If < 0 || is the last one, do nothing
		if (index>=0 && index < (this->getRelatedScene()->getFsm()->getNumberOfOutputs()-1)) {
			this->getRelatedScene()->getFsm()->movedownOutput(index);
			this->doItemsLayout();
		}

	}
	//-- If SUPPR, remove //// Needed to secure ?? : event->modifiers() == Qt::CTRL
	else if ( event->key() == Qt::Key_Delete) {

		//-- Get Index
		int index = this->currentIndex().row();

		//-- If < 0, nothing to do
		if (index >= 0) {

		    this->getRelatedScene()->getUndoStack()->push(
		                            new DeleteOutputAction(index,this->getRelatedScene()->getFsm(),NULL,this));
			this->doItemsLayout();
		}
	}

	//-- Call parent
	QTreeView::keyPressEvent(event);


}

void OutputWidget::contextMenuEvent(QContextMenuEvent* e) {
	QMenu contextMenu(this);

	//if(this->currentItem()) {
	if (this->indexAt(e->pos()).isValid()) {
		contextMenu.addAction(rename);
		contextMenu.addAction(outputup);
		contextMenu.addAction(outputdown);
		contextMenu.addAction(deleteselectedoutput);
		contextMenu.addSeparator();
	}
	contextMenu.addAction(newoutput);
	contextMenu.addAction(deleteoutput);
	contextMenu.exec(e->globalPos());

}

void OutputWidget::createActions() {

	//-- Rename
	rename = new QAction(this);
	rename->setObjectName(QString::fromUtf8("rename"));
	rename->setEnabled(true);
	rename->setText(QApplication::translate("OutputWidget", "Rename", 0,
			QApplication::UnicodeUTF8));
	rename->connect(rename,SIGNAL(activated()),this,SLOT(renameOutput()));

	//-- New output
	newoutput = new QAction(this);
	newoutput->setObjectName(QString::fromUtf8("newOutput"));
	newoutput->setEnabled(true);
	newoutput->setText(QApplication::translate("OutputWidget", "New Output", 0,
			QApplication::UnicodeUTF8));
	newoutput->connect(newoutput,SIGNAL(activated()),this,SLOT(newOutput()));


	//-- Delete  last output
	deleteoutput = new QAction(this);
	deleteoutput->setObjectName(QString::fromUtf8("deleteOutput"));
	deleteoutput->setEnabled(true);
	deleteoutput->setText(QApplication::translate("OutputWidget",
			"Delete last Output", 0, QApplication::UnicodeUTF8));
	deleteoutput->connect(deleteoutput,SIGNAL(activated()),this,SLOT(deleteLast()));


	//-- Delete selected output
	deleteselectedoutput = new QAction(this);
	deleteselectedoutput->setObjectName(QString::fromUtf8(
			"deleteSelectedOutput"));
	deleteselectedoutput->setEnabled(true);
	deleteselectedoutput->setText(QApplication::translate("OutputWidget",
			"Delete selected Output", 0, QApplication::UnicodeUTF8));
	deleteselectedoutput->connect(deleteselectedoutput,SIGNAL(activated()),this,SLOT(deleteSelected()));

	//-- Move up
	outputup = new QAction(this);
	outputup->setObjectName(QString::fromUtf8("moveupOutput"));
	outputup->setEnabled(true);
	outputup->setText(QApplication::translate("OutputWidget", "Move up", 0,
			QApplication::UnicodeUTF8));
	outputup->connect(outputup,SIGNAL(activated()),this,SLOT(moveUp()));

	//-- Move down
	outputdown = new QAction(this);
	outputdown->setObjectName(QString::fromUtf8("movedownOutput"));
	outputdown->setEnabled(true);
	outputdown->setText(QApplication::translate("OutputWidget", "Move down", 0,
			QApplication::UnicodeUTF8));
	outputdown->connect(outputdown,SIGNAL(activated()),this,SLOT(moveDown()));
}


void OutputWidget::newOutput() {
	NewOutput* Output;
	Output = new NewOutput(this->getRelatedScene()->getFsm(),this);
	Output->exec();
	this->doItemsLayout();
}

void OutputWidget::renameOutput() {

	//-- FIXME Use a redo command
	RenameOutput* ReOut;
	ReOut = new RenameOutput(this->getRelatedScene(),this->currentIndex().row(),this);
	ReOut->exec();
	this->doItemsLayout();
}

void OutputWidget::deleteLast() {



	//-- FIXME Use a redo command
	Fsm& f = *(this->getRelatedScene()->getFsm());
	int o = f.getNumberOfOutputs();
	if (o > 0) {

	    this->getRelatedScene()->getUndoStack()->push(
	                    new DeleteOutputAction(o--,this->getRelatedScene()->getFsm(),NULL,this));

	    this->getRelatedScene()->getFsm()->deleteOutput(o--);
	}

	//-- Update
	this->doItemsLayout();
}

void OutputWidget::deleteSelected() {

	//-- FIXME Use a redo command
	int i = this->currentIndex().row();
	//QModelIndex mi = this->outputeditorwidget->indexAt(e->pos() );

	this->getRelatedScene()->getUndoStack()->push(
	                new DeleteOutputAction(i,this->getRelatedScene()->getFsm(),NULL,this));

	//-- Update
	this->doItemsLayout();
}



void OutputWidget::moveDown() {

	//-- FIXME Use a redo command
	//QTreeWidgetItem* it = this->listofoutputs->currentItem();
	//int i = this->listofoutputs->indexOfTopLevelItem(it);
	int i = this->currentIndex().row();

	Fsm& f = *(this->getRelatedScene()->getFsm());
	f.movedownOutput(i);

	//-- Update
	this->doItemsLayout();
}

void OutputWidget::moveUp() {

	//-- FIXME Use a redo command
	//QTreeWidgetItem* it = this->listofoutputs->currentItem();
	//int i = this->listofoutputs->indexOfTopLevelItem(it);
	int i = this->currentIndex().row();

	Fsm& f = *(this->getRelatedScene()->getFsm());
	f.moveupOutput(i);

	//-- Update
	this->doItemsLayout();
}


void OutputWidget::setRelatedScene(Scene * scene) {

	//-- Disconnection
	if (this->getRelatedScene() != NULL) {
		QObject::disconnect(this->getRelatedScene(),
				SIGNAL(selectionChanged()),this,
				SLOT(selectionUpdated()));
	}

	//-- Set Scene
	SceneRelatedObject::setRelatedScene(scene);



	if (this->getRelatedScene() != NULL) {
		//-- Connection
		this->connect(this->getRelatedScene(), SIGNAL(selectionChanged()),SLOT(selectionUpdated()));
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

void OutputWidget::selectionUpdated() {

    //-- Selection Not updated if NULL
    if (this->getRelatedScene()==NULL) {
        return;
    }

	//-- Get First selected Item in list
	QList<QGraphicsItem*> selectedItems = this->getRelatedScene()->selectedItems();
	if (selectedItems.length() == 0) {

		//-- No Selection, edit none
		dynamic_cast<OutputModel*> (this->model())->editNone();
		dynamic_cast<OutputDelegate*> (this->itemDelegate())->editNone();

	} else {


		//--  No take! Analyse First
		for (QList<QGraphicsItem*>::iterator it = selectedItems.begin(); it!= selectedItems.end(); it++) {

			QGraphicsItem* firstSelected = (*it)->topLevelItem();

			//-- Switch on type
			if (firstSelected->type()==StateItem::Type && dynamic_cast<StateItem*>(firstSelected)->getModel()!=NULL) {

				dynamic_cast<OutputModel*> (this->model())->editState(dynamic_cast<StateItem*>(firstSelected));
				dynamic_cast<OutputDelegate*> (this->itemDelegate())->editState(dynamic_cast<StateItem*>(firstSelected));

			}  else {

				//-- Nothing Special, edit none
				dynamic_cast<OutputModel*> (this->model())->editNone();
				dynamic_cast<OutputDelegate*> (this->itemDelegate())->editNone();
			}

		}


	}



	//-- Update
	this->doItemsLayout();

}
