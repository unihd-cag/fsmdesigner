/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
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
//---------------



//-- Std
#include <iostream>
using namespace std;

//-- Core
#include <core/FSMDesigner.h>
#include <core/Project.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Trans.h>
#include <core/Condition.h>
#include <core/Hypertrans.h>


//-- Qt
#include <QtCore>
#include <QtGui>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/scene/SceneRelatedObject.h>

#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/HyperTransition.h>

#include <gui/common/propertywidget.h>
#include <gui/common/propertiesmodel.h>
#include <gui/common/propertiesdelegate.h>

//-- Actions
#include <gui/actions/DeleteConditionAction.h>



#include "propertywidget.h"

PropertyWidget::PropertyWidget(QWidget* w) :
	QTreeView(w) {


	// Base initialisation
	//--------------------------
	PropertiesModel * propmodel = new PropertiesModel();
	PropertiesDelegate * propdelegate = new PropertiesDelegate();
	this->setModel(propmodel);
	this->setItemDelegate(propdelegate);
	this->setAlternatingRowColors(true);

	// Actions
	//---------
	this->createActions();
}

void PropertyWidget::contextMenuEvent(QContextMenuEvent* e) {

	QMenu * contextMenu = new QMenu(this);

	//-- Transition or hypertrans (conditions)
	if (dynamic_cast<PropertiesModel*> (this->model())->getEditingItemType()
			== FSMDesigner::TRANS || dynamic_cast<PropertiesModel*> (this->model())->getEditingItemType()
            == FSMDesigner::HYPERTRANS) {

		//-- Transition
		Trans
				* transition =
						(Trans*) dynamic_cast<PropertiesModel*> (this->model())->getEditingItem();

		//-- "Add"
		contextMenu->addAction(addCondiToTrans);

		//-- Add Condition Delete only if over a condition row
		QModelIndexList selected = this->selectedIndexes();
        if (selected.isEmpty())
            return;

		//-- Get Row
        int row = selected.first().row();

        //-- There are 5 rows + 2 per condition
        int conditionposition = -1;
        if (row>4) {
            conditionposition = ((row-4)-1)/2;
        }

        if (conditionposition!=-1) {
            contextMenu->addAction(deleteCondiOfTrans);
        }

        //-- Show Menu
		contextMenu->exec(e->globalPos());

	}
}

void PropertyWidget::createActions() {

	addCondiToTrans = new QAction(this);
	addCondiToTrans->setObjectName(QString::fromUtf8("CondiToTrans"));
	addCondiToTrans->setEnabled(true);
	addCondiToTrans->setText(QApplication::translate("PropertyWidget",
			"Add Condition", 0, QApplication::UnicodeUTF8));


	connect(this->addCondiToTrans, SIGNAL(triggered()), this,
				SLOT(addConditionToTransitionBase()));


	deleteCondiOfTrans = new QAction(this);
	deleteCondiOfTrans->setObjectName(QString::fromUtf8("deleteCondiOfTrans"));
	deleteCondiOfTrans->setEnabled(true);
	deleteCondiOfTrans->setText(QApplication::translate("PropertyWidget",
			"Delete Condition", 0, QApplication::UnicodeUTF8));

	connect(this->deleteCondiOfTrans, SIGNAL(triggered()), this,
					SLOT(deleteConditionFromTransitionBase()));


}

void PropertyWidget::setRelatedScene(Scene * scene) {

	//-- Disconnection
	if (this->getRelatedScene() != NULL) {
		QObject::disconnect(this->getRelatedScene(),
				SIGNAL(selectionChanged()),this,SLOT(selectionUpdated()));

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
	this->doItemsLayout();

}

void PropertyWidget::selectionUpdated() {

    //-- Selection Not updated if NULL
    if (this->getRelatedScene()==NULL)
        return;



	//-- Get First selected Item in list
	QList<QGraphicsItem*> selectedItems =
			this->getRelatedScene()->selectedItems();

	if (selectedItems.length() == 0) {

		//-- No Selection, edit none
		dynamic_cast<PropertiesModel*> (this->model())->editNone();
		dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editNone();

	} else {

		//--  No take! Analyse First
		for (QList<QGraphicsItem*>::iterator it = selectedItems.begin(); it!= selectedItems.end(); it++) {

			QGraphicsItem* firstSelected = (*it)->topLevelItem();

            //-- Switch on type
            if (firstSelected->type() == StateItem::Type
                    && FSMGraphicsItem<>::toStateItem(firstSelected)->getModel()!=NULL) {

                //&& FSMGraphicsItem<>::toStateItem(firstSelected)->getModel()!=dynamic_cast<PropertiesModel*> (this->model())->getEditingItem()
                dynamic_cast<PropertiesModel*> (this->model())->editState(
                            dynamic_cast<StateItem*> (firstSelected));
                dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editState(
                            dynamic_cast<StateItem*> (firstSelected));
                this->doItemsLayout();
                break;

            } else if (firstSelected->type() == Transline::Type && FSMGraphicsItem<>::toTransline(firstSelected)->getModel()!=NULL) {

                dynamic_cast<PropertiesModel*> (this->model())->editTransline(
                            dynamic_cast<Transline*> (firstSelected));
                dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editTransline(
                            dynamic_cast<Transline*> (firstSelected));

                this->doItemsLayout();

                break;
            } else if (firstSelected->type() == TrackpointItem::Type) {

                Transline* transline = dynamic_cast<TrackpointItem*>(firstSelected)->getNextTransline();
                if(transline != NULL)
                {
                    dynamic_cast<PropertiesModel*> (this->model())->editTransline(transline);
                    dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editTransline(transline);

                    this->doItemsLayout();

                    break;
                }
            } else if (firstSelected->type() == LinkDeparture::Type && FSMGraphicsItem<>::toLinkDeparture(firstSelected)->getModel()!=NULL) {

                dynamic_cast<PropertiesModel*> (this->model())->editLinkDeparture(
                            dynamic_cast<LinkDeparture*> (firstSelected));
                dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editLinkDeparture(
                            dynamic_cast<LinkDeparture*> (firstSelected));

                this->doItemsLayout();

                break;

            } else if (firstSelected->type() == HyperTransition::Type && FSMGraphicsItem<>::toHyperTransition(firstSelected)->getModel()!=NULL) {

                dynamic_cast<PropertiesModel*> (this->model())->editHypertrans(
                            dynamic_cast<HyperTransition*> (firstSelected));
                dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editHypertrans(
                            dynamic_cast<HyperTransition*> (firstSelected));

                this->doItemsLayout();

                break;
            } else {

                //-- Nothing Special, edit none
                dynamic_cast<PropertiesModel*> (this->model())->editNone();
                dynamic_cast<PropertiesDelegate*> (this->itemDelegate())->editNone();


			}



		} // EOF Selection loop
	} // EOF Selection

	//-- Update
	this->doItemsLayout();


}

void PropertyWidget::addConditionToTransitionBase() {

    //-- Get model
    TransitionBase * transitionBase = NULL;
    if (dynamic_cast<PropertiesModel*> (this->model())->getEditingItemType()
            == FSMDesigner::TRANS) {
        transitionBase = ((Transline*)(dynamic_cast<PropertiesModel*> (this->model())->getEditingItem()))->getModel();
    } else {
        transitionBase = ((HyperTransition*)(dynamic_cast<PropertiesModel*> (this->model())->getEditingItem()))->getModel();
    }

	//-- Add
    transitionBase->addCondition(this->getRelatedScene()->getFsm()->getNumberOfInputs());

	//-- Update
	this->doItemsLayout();

}
void PropertyWidget::deleteConditionFromTransitionBase() {

    //-- the number of rows depends on item
    int numberofBaserows = 0;

    //-- Get model
    TransitionBase * transitionBase = NULL;
    if (dynamic_cast<PropertiesModel*> (this->model())->getEditingItemType()
            == FSMDesigner::TRANS) {
        transitionBase = ((Transline*)(dynamic_cast<PropertiesModel*> (this->model())->getEditingItem()))->getModel();
        numberofBaserows = 5;
    } else {
        transitionBase = ((HyperTransition*)(dynamic_cast<PropertiesModel*> (this->model())->getEditingItem()))->getModel();
        numberofBaserows = 4;
    }


	//-- Get ID of condition to be deleted
	QModelIndexList selected = this->selectedIndexes();
	if (selected.isEmpty())
		return;

	//-- Get Row
	int row = selected.first().row();



	//-- There are 5 rows + 2 per condition
	int conditionposition = -1;
	if (row>(numberofBaserows-1)) {
		conditionposition = ((row-(numberofBaserows-1))-1)/2;
	}

	if (conditionposition==-1)
	    return;


	//-- Delete
	DeleteConditionAction * deleteCondition = new DeleteConditionAction(transitionBase,transitionBase->getConditionByID(conditionposition));
	getRelatedScene()->getUndoStack()->push(deleteCondition);

	//-- Update
	this->doItemsLayout();


}

