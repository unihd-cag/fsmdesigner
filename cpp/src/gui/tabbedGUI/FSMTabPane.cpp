/*
 * FSMTabPane.cpp
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Std
#include <iostream>
using namespace std;

//-- Core
#include <core/FSMDesigner.h>
#include <core/LoadFactory.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Gui
#include <gui/scene/FSMSceneView.h>

#include <gui/scene/Scene.h>
#include <gui/fsm/newfsm.h>
#include <gui/fsm/newfsmname.h>
#include <gui/settings/GuiSettings.h>

#include <gui/actions/DeleteFSMAction.h>

//-- Qt
#include <QtGui>

#include "FSMTabPane.h"

FSMTabPane::FSMTabPane(QWidget * parent) : QTabWidget(parent) {

	//-- Corner: Add Add button
	//----------------------------
	QPushButton * tabButton = new QPushButton(
			QIcon(QPixmap(":/icons/add.png")), "");
	this->connect(tabButton, SIGNAL(clicked()), SLOT(addFSM()));
	this->setCornerWidget(tabButton, Qt::TopLeftCorner);

	//-- Closing a Tab

	//-- Style parameters
	this->setTabPosition(QTabWidget::West);
	this->setDocumentMode(true);
	this->setTabsClosable(true);

	// Close request to remove a FSM
	//---------------------
	this->connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(deleteFSM(int)));

	// currentChanged -> tabSelectionChanged -> emit sceneSelectionChanged()
	//------------------
	this->connect(this,SIGNAL(currentChanged(int)),this,SLOT(tabSelectionChanged(int)));

	// Update depending on UI settings
	//--------------------
	this->connect(GuiSettings::getAsQObject(),SIGNAL(settingsChanged()),SLOT(settingsChanged()));
	this->settingsChanged();

}

FSMTabPane::~FSMTabPane() {
	// TODO Auto-generated destructor stub
}

Scene * FSMTabPane::getSelectedScene() {

    return this->selectedScene;

}

void FSMTabPane::settingsChanged() {


	// Tabs positions
	//-------------------------
	QVariant tabPos = GuiSettings::value(SETTINGS_UI_TABPOS);


	if (!tabPos.isValid() || tabPos.toString() == "Top") {
		this->setTabPosition(QTabWidget::North);
	} else if (tabPos.toString() == "Right") {
		this->setTabPosition(QTabWidget::East);
	} else if (tabPos.toString() == "Bottom") {
		this->setTabPosition(QTabWidget::South);
	} else if (tabPos.toString() == "Left") {
		this->setTabPosition(QTabWidget::West);
	}
	this->update();

}



void FSMTabPane::keyPressEvent(QKeyEvent * e) {

	//-- Call to parent
	QTabWidget::keyPressEvent(e);

	//--

}


void FSMTabPane::placeSetState() {

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->placeSetMode(FSMDesigner::STATE);
}

void FSMTabPane::placeSetTransition(){

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->placeSetMode(FSMDesigner::TRANS);
}

void FSMTabPane::placeSetHyperTransition(){

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->placeSetMode(FSMDesigner::HYPERTRANS);
}

void FSMTabPane::placeSetLink(){

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->placeSetMode(FSMDesigner::LINKDEPARTURE);

}

void FSMTabPane::placeSetJoin(){

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->placeSetMode(FSMDesigner::JOIN);

}

void FSMTabPane::generateVerilog() {

    if (this->currentIndex()>0)
        dynamic_cast<FSMSceneView*>(this->currentWidget())->generateVerilog();

}


void FSMTabPane::undo() {

    if (this->currentIndex()>0) {
        dynamic_cast<FSMSceneView*>(this->currentWidget())->getRelatedScene()->getUndoStack()->undo();
    }
}


void FSMTabPane::redo() {

    if (this->currentIndex()>0) {
        dynamic_cast<FSMSceneView*>(this->currentWidget())->getRelatedScene()->getUndoStack()->redo();
    }
}

void FSMTabPane::tabInserted(int index) {

	//-- New Tabs (not the welcome one) are closable
	if (index > 0) {
		//QPushButton * closeButton = new QPushButton(QIcon(QPixmap(":/icons/cancel.png")),"");
		//this->tabBar()->setTabButton(index,QTabBar::RightSide,closeButton);
		//this->tabBar()->setTabsClosable(true);

	}

	if (index==0) {

	    this->tabBar()->setTabButton(index,QTabBar::RightSide,NULL);
	   // this->tabBar()->tab
	}

}


void FSMTabPane::addFSM() {

	//-- Create a FSM
	Fsm * newFSM = new Fsm();

	//-- Call Initialisation
	//--------------------------------

	//-- Name Init
	FsmName * newFSMName = new FsmName(newFSM,this);
	if (newFSMName->exec() == QDialog::Rejected) {
		//-- Stop
		return;
	}

	//-- Get Created FSM and Add it to Project
	Core::getInstance()->getProject()->addFSM(newFSM);

	//-- I/O Init
	//NewFsm * newFSMDialog = new NewFsm(newFSM, this);
	//newFSMDialog->exec();

	//-- Add To Tab
	this->openFSM(newFSM);

}

void FSMTabPane::openFSM(Fsm * fsm) {

	//-- Create Scene Widget
	Scene * fsmScene = new Scene(fsm);

	//-- Create the view
	//TabbedGUISceneView * sceneView = new TabbedGUISceneView(fsmScene);
	FSMSceneView * sceneView = new FSMSceneView(fsmScene);
	this->insertTab(this->count(), sceneView,
			QString(fsm->getFsmName().c_str()));

}

void FSMTabPane::closeFSM(int index) {

	//-- Get the Widget at index
	QWidget * fsmWidget = this->widget(index);

	//-- Get the related fsm
	Fsm * fsm = dynamic_cast<FSMSceneView*>(fsmWidget)->getRelatedScene()->getFsm();

	//-- Close tab
    this->removeTab(index);


}

void FSMTabPane::deleteFSM(int index) {

   //-- Get the Widget at index
   QWidget * fsmWidget = this->widget(index);

   //-- Get the related fsm
   Fsm * fsm = dynamic_cast<FSMSceneView*>(fsmWidget)->getRelatedScene()->getFsm();

    //-- Ask confirmation
    if (QMessageBox::question(
            this,
            "Remove FSM",
            QString()+"Do you want to remove the fsm <b>"+fsm->getFsmName().c_str()+"</b>",
            QMessageBox::Yes,
            QMessageBox::No)==QMessageBox::Yes) {


        // Close
        //-----------------
        this->closeFSM(index);

        // Remove
        //-----------------

        //-- FIXME uses Undo/Redo Do Remove
        fsm->getProject()->deleteFSM(fsm);

    }
}


void FSMTabPane::tabSelectionChanged(int index) {

    // Propagate NULL if -1 or 0 (because 0 will be welcome Tab)
    //--------------------------------
    Scene * displayedScene = NULL;
    if (index>=1) {
        displayedScene = dynamic_cast<FSMSceneView*>(this->widget(index))->getRelatedScene();
    }

    // Maintain Pointer
    //--------------------
    this->selectedScene = displayedScene;

    // Send Signal
    //----------------
    this->sceneSelectionChanged(displayedScene);


}



