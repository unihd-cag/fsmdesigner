/*
 * WelcomeTab.cpp
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

// Includes
//-------------------------

//-- Std
#include <iostream>
using namespace std;

//-- QT
#include <QtGui>

//-- Gui
#include <gui/common/RoundedPanel.h>

#include <gui/settings/GuiSettings.h>
#include <gui/settings/SettingsWidget.h>

//-- Core
#include <core/LoadFactory.h>
#include <core/Core.h>
#include <core/Project.h>

#include "WelcomeTab.h"
WelcomeTab::WelcomeTab(QWidget * parent) :
	QWidget(parent) {

	//-- Initialize
	this->initialize();

}

WelcomeTab::~WelcomeTab() {
	delete contentPanel;
}


void WelcomeTab::initialize() {

	// Occupy all the space
	//-----------------------------
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//-- Background
	this->setPalette(QPalette(Qt::white));
	this->setAutoFillBackground(true);

	// Create a rounded Panel
	//-----------------------------
	this->initializeContentPanel();

	// Parameters
	//------------------
	this->openFileLastPath = "";

	//-- Update UI
	this->modelChanged();

}

QWidget * WelcomeTab::initializeContentPanel() {

	// Create
	//---------------
	this->contentPanel = new RoundedPanel(this);
	this->contentPanel->setPalette(QPalette(QColor("#22BCFF")));
	this->contentPanel->setAutoFillBackground(true);
	//this->contentPanel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	QHBoxLayout * hColumns = new QHBoxLayout();
	hColumns->setMargin(10);
	this->contentPanel->setLayout(hColumns);

	// Two Columns
	// Open/Create | Settings
	//-----------------------------


	//-- Left side
	//-----------------
	QWidget * openCreateWidget = new QWidget();
	openCreateWidget->setPalette(QPalette(QColor("#5FCAF9")));
	openCreateWidget->setAutoFillBackground(true);
	openCreateWidget->setLayout(new QVBoxLayout());
	hColumns->addWidget(openCreateWidget, 1);
	//dynamic_cast<QVBoxLayout*>(openCreateWidget->layout())->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);

	//---- Project Info
	//-----------------------------
	Project * currentProject = Core::getInstance()->getProject();

	//-- Title
	QLabel * projectInfoLabel = new QLabel("<h1>Current Project</h1>");
	openCreateWidget->layout()->addWidget(projectInfoLabel);

	//-- Widget
	QWidget * projectInfoWidget = new QWidget();
	QGridLayout * projectInfoLayout = new QGridLayout();
	projectInfoLayout->setHorizontalSpacing(5);
	projectInfoWidget->setLayout(projectInfoLayout);
	openCreateWidget->layout()->addWidget(projectInfoWidget);

	//-- Name
	projectInfoLayout->addWidget(new QLabel("<h3>Name:</h3>"),			0,0);
	this->projectInfoName = new QLabel(currentProject->getName());
	projectInfoLayout->addWidget(this->projectInfoName,	0,1);

	// Rename project
	QPushButton * projectInfoRenameButton = new QPushButton(QIcon(QPixmap(
					":/icons/Rename")), "");
	projectInfoRenameButton->setIconSize(QSize(24, 24));
	projectInfoRenameButton->setFlat(true);
	projectInfoRenameButton->setToolTip("Rename FSM");
	projectInfoLayout->addWidget(projectInfoRenameButton, 				0,2);

	this->connect(projectInfoRenameButton, SIGNAL(clicked()), SLOT(renameProject()));

	//-- File location
	projectInfoLayout->addWidget(new QLabel("<h3>Location:</h3>"),			1,0);
	this->projectInfoLocation = new QLabel();
	projectInfoLayout->addWidget(this->projectInfoLocation,	1,1);


	//---- Create a project Widget
	//-------------------------------

	// Title
	QLabel *createLabel = new QLabel("<h1>Create a project...</h1>");
	openCreateWidget->layout()->addWidget(createLabel);

	// Form
	QWidget * createWidget = new QWidget();
	openCreateWidget->layout()->addWidget(createWidget);
	QFormLayout * createWidgetLayout = new QFormLayout();
	createWidget->setLayout(createWidgetLayout);

	// Label
	QLabel *createNameLabel = new QLabel("Project Name:");

	// text field
	this->projectCreateName = new QLineEdit();
	projectCreateName->setBaseSize(200, 40);

	// Create Button
	QPushButton * createButton = new QPushButton(QIcon(QPixmap(
							":/icons/ArrowRightGreenCircled")), "");
	createButton->setIconSize(QSize(24, 24));
	createButton->setFlat(true);
	createButton->setToolTip("Create Project");

	this->connect(createButton, SIGNAL(clicked()), SLOT(createProject()));

	// Text+Button
	QHBoxLayout * createNameAndButtonLayout = new QHBoxLayout();
	createNameAndButtonLayout->addWidget(projectCreateName);
	createNameAndButtonLayout->addWidget(createButton);


	// Add Label and Text+Button
	createWidgetLayout->addRow(createNameLabel, createNameAndButtonLayout);


	//---- Files opening
	//------------------------------

	// Add Open title
	QLabel * openLabel = new QLabel("<h1>...or Open...</h1>");
	dynamic_cast<QVBoxLayout*> (openCreateWidget->layout())->addWidget(
			openLabel);

	// Create grid Widget
	QGridLayout * openWidgetLayout = new QGridLayout();
	dynamic_cast<QVBoxLayout*> (openCreateWidget->layout())->addLayout(
			openWidgetLayout);

	//---- Open
	QLabel * openFileLabel = new QLabel("...a new file");
	openWidgetLayout->addWidget(openFileLabel, 0, 0);
	openWidgetLayout->setColumnStretch(0, 1);

	QPushButton * openFileButton = new QPushButton(QIcon(QPixmap(
			":/icons/Folder-icon.png")), "");
	openFileButton->setIconSize(QPixmap(":/icons/Folder-icon.png").size());
	openFileButton->setFlat(true);
	openWidgetLayout->addWidget(openFileButton, 1, 0);

	this->connect(openFileButton, SIGNAL(clicked()), SLOT(openFile()));

	//---- Recent files
	QLabel * openRecentFileLabel = new QLabel("...a recent file");
	openWidgetLayout->addWidget(openRecentFileLabel, 0, 1);
	openWidgetLayout->setColumnStretch(1, 1);

	// Fill view with model, and connect open signal
	QListWidget * recentFilesView = new QListWidget();
	recentFilesView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	recentFilesView->setAlternatingRowColors(true);
	recentFilesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	recentFilesView->setTextElideMode(Qt::ElideLeft);
	recentFilesView->addItems(GuiSettings::getRecentFiles());

	// Set tooltip to have a view on complete path
	for (int i=0;i<recentFilesView->count();i++) {
		recentFilesView->item(i)->setToolTip(recentFilesView->item(i)->text());
	}


	openWidgetLayout->addWidget(recentFilesView, 1, 1);
	this->connect(recentFilesView, SIGNAL(itemActivated( QListWidgetItem *)),
			SLOT(openRecentFile( QListWidgetItem *)));

	// End of layout
	dynamic_cast<QVBoxLayout*> (openCreateWidget->layout())->addStretch();

	//-- Right side
	//-----------------
	QWidget * settingsWidget = new QWidget();
	settingsWidget->setPalette(QPalette(QColor("#5FCAF9")));
	settingsWidget->setAutoFillBackground(true);
	settingsWidget->setLayout(new QVBoxLayout());
	hColumns->addWidget(settingsWidget, 1);

	//-- Add Title
	QLabel * settingsLabel = new QLabel("<h1>Tune settings...</h1>");
	settingsWidget->layout()->addWidget(settingsLabel);

	//-- Add Panel
	settingsWidget->layout()->addWidget(this->initializeSettingsPanel());

	// End of layout
	dynamic_cast<QVBoxLayout*> (settingsWidget->layout())->addStretch();

	//-- End horizontal layout
	//hColumns->addStretch();

	return this->contentPanel;
}

QWidget * WelcomeTab::initializeSettingsPanel() {

	QWidget * settingsWidget = new QWidget();
	QVBoxLayout * settingsLayout = new QVBoxLayout();
	settingsWidget->setLayout(settingsLayout);

	// Tab Locations
	//-------------------

	QLabel * tabTitle = new QLabel("<h3>Tabs location</h3>");
	settingsLayout->addWidget(tabTitle);

	//-- Panel
	QWidget * tabPanel = new QWidget();
	settingsLayout->addWidget(tabPanel);
	QGridLayout * tabPanelLayout = new QGridLayout();
	tabPanel->setLayout(tabPanelLayout);

	//-- The four buttons
	int size=24;
	QPixmap arrowIcon(":/icons/Actions-arrow-left-icon.png");

	QPushButton * tabTop = new QPushButton(arrowIcon.transformed(QTransform().rotate(90)),"");
	tabTop->setFixedSize(QSize(size,size));
	GuiSettings::getAsQObject()->connect(tabTop,SIGNAL(clicked()),SLOT(setTabTop()));
	tabPanelLayout->addWidget(tabTop,0,1);

	QPushButton * tabRight = new QPushButton(arrowIcon.transformed(QTransform().rotate(180)),"");
	tabRight->setFixedSize(QSize(size,size));
	GuiSettings::getAsQObject()->connect(tabRight,SIGNAL(clicked()),SLOT(setTabRight()));
	tabPanelLayout->addWidget(tabRight,1,2);

	QPushButton * tabBottom = new QPushButton(arrowIcon.transformed(QTransform().rotate(-90)),"");
	tabBottom->setFixedSize(QSize(size,size));
	GuiSettings::getAsQObject()->connect(tabBottom,SIGNAL(clicked()),SLOT(setTabBottom()));
	tabPanelLayout->addWidget(tabBottom,2,1);

	QPushButton * tabLeft = new QPushButton(arrowIcon,"");
	tabLeft->setFixedSize(QSize(size,size));
	GuiSettings::getAsQObject()->connect(tabLeft,SIGNAL(clicked()),SLOT(setTabLeft()));
	tabPanelLayout->addWidget(tabLeft,1,0);

	// Automatic Creation
	//--------------------------
	SettingsWidget * autoSettingsWidget = new SettingsWidget(":/settings/ui",settingsWidget);
	settingsLayout->addWidget(autoSettingsWidget);

	return settingsWidget;
}

void WelcomeTab::resizeEvent(QResizeEvent * event) {

	//cout << "Resize" << endl;

	QWidget::resizeEvent(event);

	//-- Content margins by 30 %
	qreal widthMargin = this->width() * 20 / 100;
	qreal heightMargin = this->height() * 20 / 100;

	//-- Replace content panel in the middle
	this->contentPanel->layout()->sizeHint();
	this->contentPanel->resize(this->width() - widthMargin,
			this->contentPanel->layout()->sizeHint().height());
	this->contentPanel->move(widthMargin / 2, heightMargin / 2);

}

/**
 * \defgroup Slots
 * @{
 */

void WelcomeTab::openRecentFile(QListWidgetItem * item) {

	//-- Open
	this->openFile(item->text());

}

void WelcomeTab::openFile(QString path) {

	QString file = path;

	//-- Ask Project to open if path is not provided
	if (file == "") {

		// First ask the project to open
		//------------
		// (Add a .pro.xml file filter)
		file = QFileDialog::getOpenFileName(this, "Choose a filename to load",
				this->openFileLastPath, "FSMDesigner Project/FSM file (*.xml)");

	}

	// Open Project File
	//--------------------
	if (file.endsWith(".pro.xml")) {


	    //-- Signal
	    this->openedFile(file);


	}
	// Open a single FSM
	//----------------------
	else if (file.endsWith(".xml")) {

	    //-- Signal
        this->openedFile(file);

	}


}

void WelcomeTab::createProject() {

	//-- Get Name from Line edit
	QString name = this->projectCreateName->text();

	//-- Call real create
	this->createProject(name);

}




void WelcomeTab::createProject(QString name) {

	//-- Name can't be zerolength
	if (name=="") {
		QMessageBox::critical(this,"Create Project","Name provided to create project can't be an empty string");
		return;
	}

	//-- Trigger signal // Global GUi should create the project
    this->createdProject(name);


	//-- Update UI
	this->modelChanged();

}



void WelcomeTab::renameProject(QString newName ) {

	//-- Ask for name if not provided
	if (newName=="") {

		newName = QInputDialog::getText(
				this,
				"Rename Project",
				"New Project Name:",
				QLineEdit::Normal,
				Core::getInstance()->getProject()->getName());

		//-- Name can't be zerolength
		if (newName=="") {
			QMessageBox::critical(this,"Rename Project","Name provided to rename the project can't be an empty string");
			return;
		}

	}

	//-- Rename
	Core::getInstance()->getProject()->setName(newName);

	//-- Update UI
	this->modelChanged();


}

void  WelcomeTab::modelChanged() {

	//-- Get Project
	Project * currentProject = Core::getInstance()->getProject();


	//-- Update Name label
	this->projectInfoName->setText(currentProject->getName());

	//-- Update location
	if (currentProject->getFileInfo().exists())
		this->projectInfoLocation->setText(currentProject->getFileInfo().absoluteFilePath());
	else
		this->projectInfoLocation->setText("Not Saved");
}

/** @}*/



