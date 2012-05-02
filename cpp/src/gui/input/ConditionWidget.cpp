/*
 * ConditionWidget.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: rleys
 */

// Includes
//---------------------

//-- Std
#include <vector>
#include <sstream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Trans.h>

//-- GUI
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include <gui/input/inputwidget.h>
#include <gui/input/inputmodel.h>

#include <gui/items/Transline.h>

#include "conditionwidget.h"

ConditionWidget::ConditionWidget(QWidget * parent) : SceneRelatedObject(),QWidget(parent) {

	//-- Create VBar Layout
	QVBoxLayout * vlayout = new QVBoxLayout();
	this->setLayout(vlayout);

	//-- Add Combobox
	vlayout->addWidget(&(this->comboBox));
	this->connect(&(this->comboBox),SIGNAL(currentIndexChanged(int)),SLOT(conditionSelected(int)));

	//-- Add Input Widget
	vlayout->addWidget(&(this->inputwidget));

	//-- Init
	this->currentTransition = NULL;


}

ConditionWidget::~ConditionWidget() {
	// TODO Auto-generated destructor stub
}


void ConditionWidget::setRelatedScene(Scene * scene) {

	//-- Propagate
	this->inputwidget.setRelatedScene(scene);


	//-- Disconnect previous scene
	if (this->getRelatedScene()!=NULL) {
		this->getRelatedScene()->disconnect(this);
	}

	//-- Parent
	SceneRelatedObject::setRelatedScene(scene);

	//-- Connect for change in selection
	if (this->getRelatedScene()!=NULL)
	    QObject::connect(this->getRelatedScene(),SIGNAL(selectionChanged()),this,SLOT(sceneSelectionChanged()));
}

void ConditionWidget::sceneSelectionChanged() {

	//-- Clear
	this->comboBox.clear();
	this->currentTransition = NULL;

	//-- Selection Not updated if NULL
    if (this->getRelatedScene()==NULL)
        return;

	//-- Get First selected Item in list
	QList<QGraphicsItem*> selectedItems = this->getRelatedScene()->selectedItems();
	if (selectedItems.length() > 0) {

		QGraphicsItem* firstSelected = selectedItems.first()->topLevelItem();

		//-- Switch on type
		if (firstSelected->type()==Transline::Type && FSMGraphicsItem<>::toTransline(firstSelected)->getModel()!=NULL) {

			//-- Record transline
			//this->currentTransition = FSMGraphicsItem<>::toTransline(firstSelected);

			//-- Add All conditions to be selected
		    unsigned int count = 0;
		    FOREACH_TRANSITION_CONDITIONS(FSMGraphicsItem<>::toTransline(firstSelected)->getModel())
                    stringstream name;
                    name << "<" << count << "> " << condition->getName() ;
                    this->comboBox.addItem(name.str().c_str(),QVariant(count));
                    count++;
            END_FOREACH_TRANSITION_CONDITIONS


		} else {

			//-- Nothing Special, edit none
			this->comboBox.clear();

		}
	}

}


void ConditionWidget::conditionSelected(int index) {

	int newid = 0;

	//-- If it gets empty, return to 0 for security
	if (index!=-1)
		newid = index;

	//-- Set the right condition to edit on input widget
	dynamic_cast<InputModel*>(this->inputwidget.model())->setConditionID(newid);
	this->inputwidget.doItemsLayout();
}