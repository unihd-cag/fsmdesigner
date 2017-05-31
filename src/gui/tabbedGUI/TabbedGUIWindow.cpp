/*
 * TabbedGUIWindow.cpp
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

//-- Gui
#include <gui/tabbedGUI/FSMTabPane.h>
#include <gui/tabbedGUI/WelcomeTab.h>
#include <gui/tabbedGUI/TabbedGUISceneView.h>

#include <gui/scene/Scene.h>

#include <gui/settings/GuiSettings.h>

//-- Core
#include <core/Core.h>
#include <core/LoadFactory.h>
#include <core/Utils.h>
#include <core/Project.h>
#include <core/Fsm.h>

//-- Qt
#include <QtGui>


#include "TabbedGUIWindow.h"

TabbedGUIWindow::TabbedGUIWindow() {

	//-- Initialize
	this->initialize();

}

TabbedGUIWindow::~TabbedGUIWindow() {
	// TODO Auto-generated destructor stub
}


void TabbedGUIWindow::initialize() {

	// Window Global parameters
	//-----------------------
	this->setWindowTitle(QString("FSM Designer ")+FSMDesigner_VERSION_MAJOR_STR+"."+FSMDesigner_VERSION_MINOR_STR);

	//-- Logo
	//Q_INIT_RESOURCE(icons);
	//QPixmap logo(":/icons/FSMDesignerlogosmall.png");
	//this->setWindowIcon(QIcon(logo));

	// Restore Geometry
	//---------------------
	QVariant savedGeometry = GuiSettings::value("TabbedGUIWindow.geometry");
	if (savedGeometry.isValid()) {
		this->restoreGeometry(savedGeometry.toByteArray());
	}

	// D&D Open
	//--------------
	this->setAcceptDrops(true);

	// Tab pane as central widget
	//-----------------
	FSMTabPane * tabPane = new FSMTabPane();
	this->setCentralWidget(tabPane);


	// Add Welcome Tab
	//-----------------------
	WelcomeTab * welcomeTab = new WelcomeTab(tabPane);
	tabPane->insertTab(0,welcomeTab,"Welcome");

	//-- Receive open signals from Welcome Tab
	this->connect(welcomeTab,SIGNAL(projectOpened()),SLOT(openProject()));
	this->connect(welcomeTab,SIGNAL(projectClosed()),SLOT(closeProject()));


	// Update depending on UI settings
	//-------------------------
	this->connect(GuiSettings::getAsQObject(),SIGNAL(settingsChanged()),SLOT(settingsChanged()));
	this->settingsChanged();

}

void TabbedGUIWindow::settingsChanged() {

}

void TabbedGUIWindow::resizeEvent ( QResizeEvent * event ) {

	//-- Parent handling
	QMainWindow::resizeEvent(event);

	//-- Save Geometry state
	GuiSettings::setValue("TabbedGUIWindow.geometry",QVariant(this->saveGeometry()));


}

void TabbedGUIWindow::keyReleaseEvent ( QKeyEvent * event ) {

	QMainWindow::keyReleaseEvent(event);

	//-- F11 == Fullscreen
	if (event->key() == Qt::Key_F11) {
		if (this->windowState() == Qt::WindowFullScreen) {
			// Restore geometry
			this->setWindowState(Qt::WindowMaximized);
		} else {
			this->setWindowState(Qt::WindowFullScreen);
		}

	}

}

void TabbedGUIWindow::dragEnterEvent(QDragEnterEvent * event) {

	//-- Parent call
	QMainWindow::dragEnterEvent(event);

	cout << "Drag entering with action " << event->proposedAction() << endl;

	// Only support file drops (Copy URL)
	//----------------------
	const QMimeData * mimeData = event->mimeData();
	if (event->proposedAction() == Qt::CopyAction && mimeData->hasUrls()) {

		//-- Only support the first provided
		QList<QUrl> files = mimeData->urls();
		QUrl firstFile = files.first();

		if (firstFile.toLocalFile().endsWith(".pro.xml")) {
			event->acceptProposedAction();
		} else {
			event->dropAction();
		}

	}

}

void TabbedGUIWindow::dropEvent(QDropEvent * event) {

	//-- Parent
	QMainWindow::dropEvent(event);

	const QMimeData * mimeData = event->mimeData();
	if (event->proposedAction() == Qt::CopyAction && mimeData->hasUrls()) {

		//-- Only support the first provided
		QList<QUrl> files = mimeData->urls();
		QUrl firstFile = files.first();
		cout << "\tOpening from file: "
				<< (firstFile.toLocalFile().toStdString()) << endl;

		if (firstFile.toLocalFile().endsWith(".pro.xml")) {

			// Only open project
			//----------------------
			LoadFactory::getLoad_I().load(firstFile.toLocalFile().toStdString());

			//-- Record in Recent Files
			GuiSettings::addRecentFile(firstFile.toLocalFile());

			this->openProject();
		}

	}

}

/** \defgroup Slots */
/** @{ */

void TabbedGUIWindow::openProject() {

	// Close Project before opening the one loaded in core
	//------------------------
	this->closeProject();

	// Add a new Tab for each FSM
	//----------------------------------
	Project * project = Core::getInstance()->getProject();
	QList<Fsm*> fsmList = project->getFSMs();
	for (QList<Fsm*>::iterator it=fsmList.begin();it!=fsmList.end();it++) {

		//-- Get Fsm
		Fsm * fsm = *it;

		//-- Add to tabs
		dynamic_cast<FSMTabPane*>(this->centralWidget())->openFSM(fsm);

	}


	//-- Go to first FSM
	if (dynamic_cast<FSMTabPane*>(this->centralWidget())->count()>1) {
		dynamic_cast<FSMTabPane*>(this->centralWidget())->setCurrentIndex(1);
	}


}


void TabbedGUIWindow::closeProject() {

	// Close All Tabs excepted the Welcome one
	//----------------------------
	while (dynamic_cast<FSMTabPane*>(this->centralWidget())->count()>1) {
		dynamic_cast<FSMTabPane*>(this->centralWidget())->removeTab(1);
	}

}


/** @} */

