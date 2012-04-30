/**
 * MergedMainWindow.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 *
 *
 *
 */

// Includes
//------------------------------
//-- Std
#include <cstdio>
#include <iostream>

//-- Qt
#include <QtGui>
#include <QtCore>
#include <QtSvg>

//-- Core
#include <core/Core.h>
#include <core/Utils.h>
#include <core/LoadFactory.h>
#include <core/SaveFactory.h>
#include <core/Project.h>
#include <core/Fsm.h>

//-- Settings
#include <gui/settings/GuiSettings.h>

//-- Gui
#include <gui/tabbedGUI/FSMTabPane.h>
#include <gui/tabbedGUI/WelcomeTab.h>
#include <gui/verify/verifycontrolpanel.h>

#include <gui/table/table.h>

#include <gui/scene/Scene.h>

//-- Ui
#include <ui_MergedMainWindow.h>

#include "MergedMainWindow.h"

MergedMainWindow::MergedMainWindow() :
        Ui_MergedMainWindow() {

    // Setup UI
    //----------------
    this->setupUi(this);

    // Window Global parameters
    //-----------------------
    this->setWindowTitle(
            QString("FSM Designer ") + FSMDesigner_VERSION_MAJOR_STR
                    + "."+FSMDesigner_VERSION_MINOR_STR);

    //-- Logo
    //Q_INIT_RESOURCE(icons);
    //QPixmap logo(":/icons/FSMDesignerlogosmall.png");
    //this->setWindowIcon(QIcon(logo));

    // Restore Geometry
    //---------------------
    QVariant savedGeometry = GuiSettings::value("MergedMainWindow.geometry");
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

    // Add Welcome Tab as default Tab
    WelcomeTab * welcomeTab = new WelcomeTab(tabPane);
    tabPane->addTab(welcomeTab, "Welcome");

    // Update depending on UI settings
    //-------------------------
    this->connect(GuiSettings::getAsQObject(), SIGNAL(settingsChanged()),
            SLOT(settingsChanged()));
    this->settingsChanged();

    // Init
    //--------------------------

    //---- Recent Files
    //--------------------------
    QStringList recentFiles = GuiSettings::getRecentFiles();
    FOREACH_STRING_LIST(QStringList::iterator , recentFiles, QString, recentFile)

        QAction * action = this->menu_Recent_Files->addAction(recentFile);
        action->setData(QVariant(recentFile));

    END_FOREACH
    this->menu_Recent_Files->connect(this->menu_Recent_Files,
            SIGNAL(triggered(QAction*)), this, SLOT(openFile(QAction*)));

    // Also connect to welcomeTab
    this->connect(welcomeTab, SIGNAL(openedFile(QString)), this,
            SLOT(openFile(QString)));

    //---- Connect Components
    //------------------------------------

    //---- Properties Widget Scene reference gets updated on tab activation
    this->propertywidget->connect(this->centralWidget(),
            SIGNAL(sceneSelectionChanged(Scene*)),
            SLOT(setRelatedScene(Scene*)));

    //---- Input / Output Widget Scene reference gets updated on tab activation
    this->inputwidget->connect(this->centralWidget(),
            SIGNAL(sceneSelectionChanged(Scene*)),
            SLOT(setRelatedScene(Scene*)));
    this->outputwidget->connect(this->centralWidget(),
            SIGNAL(sceneSelectionChanged(Scene*)),
            SLOT(setRelatedScene(Scene*)));

    //---- Connect Menu
    //------------------------------

    //-- Open / New
    this->connect(this->action_Open, SIGNAL(triggered()), SLOT(openFile()));
    this->connect(this->action_New, SIGNAL(triggered()), SLOT(newProject()));

    //-- Save
    this->connect(this->action_Save, SIGNAL(triggered()), this,
            SLOT(saveProject()));
    this->connect(this->action_SaveAs, SIGNAL(triggered()), this,
            SLOT(saveProjectAs()));

    //-- Export/Print
    this->connect(this->action_Print,SIGNAL(triggered()),this,SLOT(print()));

    //-- Project
    this->connect(welcomeTab, SIGNAL(projectClosed()), this,
            SLOT(closeProject()));
    this->connect(welcomeTab, SIGNAL(createdProject(QString)), this,
            SLOT(createProject(QString)));

    //---- Editing Actions
    tabPane->connect(this->action_State, SIGNAL(triggered()),
            SLOT(placeSetState()));
    tabPane->connect(this->action_Transition, SIGNAL(triggered()),
            SLOT(placeSetTransition()));
    tabPane->connect(this->action_Hypertrans, SIGNAL(triggered()),
                SLOT(placeSetHyperTransition()));
    tabPane->connect(this->action_Link, SIGNAL(triggered()),
            SLOT(placeSetLink()));
    tabPane->connect(this->action_Join, SIGNAL(triggered()),
            SLOT(placeSetJoin()));

    //---- Undo / Redo
    tabPane->connect(this->action_Redo, SIGNAL(triggered()), SLOT(redo()));
    tabPane->connect(this->action_Undo, SIGNAL(triggered()), SLOT(undo()));

    //---- Normal Generate Verilog
    tabPane->connect(this->action_GenVerilog, SIGNAL(triggered()),
            SLOT(generateVerilog()));

    //---- Table
    this->connect(this->actionGlobal, SIGNAL(triggered()),
            SLOT(showGlobalTable()));

}

MergedMainWindow::~MergedMainWindow() {

}

void MergedMainWindow::resizeEvent(QResizeEvent * event) {

    //-- Parent handling
    QMainWindow::resizeEvent(event);

    //-- Save Geometry state
    GuiSettings::setValue("MergedMainWindow.geometry",
            QVariant(this->saveGeometry()));

}

void MergedMainWindow::keyReleaseEvent(QKeyEvent * event) {

    //-- !! Parent call
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

void MergedMainWindow::dropEvent(QDropEvent * event) {

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
            LoadFactory::getLoad_I().load(
                    firstFile.toLocalFile().toStdString());

            //-- Record in Recent Files
            GuiSettings::addRecentFile(firstFile.toLocalFile());

            this->openProject();
        }

    }

}

void MergedMainWindow::dragEnterEvent(QDragEnterEvent * event) {

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

void MergedMainWindow::newProject() {

    // Close Project before opening the one loaded in core
    //------------------------
    this->closeProject();

    // Create A new Project with one FSM
    //-----------------
    Core::getInstance()->getProject()->addFSM();

    this->openProject();

}

void MergedMainWindow::openProject() {

    // Add a new Tab for each FSM
    //----------------------------------
    Project * project = Core::getInstance()->getProject();
    QList<Fsm*> fsmList = project->getFSMs();
    for (QList<Fsm*>::iterator it = fsmList.begin(); it != fsmList.end();
            it++) {

        //-- Get Fsm
        Fsm * fsm = *it;

        //-- Add to tabs
        dynamic_cast<FSMTabPane*>(this->centralWidget())->openFSM(fsm);

    }

    //-- Go to first FSM
    if (dynamic_cast<FSMTabPane*>(this->centralWidget())->count() > 1) {
        dynamic_cast<FSMTabPane*>(this->centralWidget())->setCurrentIndex(1);
    }

    //-- Signal Welcome tab to update UI
    dynamic_cast<WelcomeTab*>(dynamic_cast<FSMTabPane*>(this->centralWidget())->widget(
            0))->modelChanged();

}

void MergedMainWindow::closeProject() {

    // Close All Tabs excepted the Welcome one
    //----------------------------
    while (dynamic_cast<FSMTabPane*>(this->centralWidget())->count() > 1) {
        dynamic_cast<FSMTabPane*>(this->centralWidget())->removeTab(1);
    }

    // Close Project in the Core
    //----------------------------------
    Core::getInstance()->reset();

//    while (dynamic_cast<FSMTabPane*>(this->centralWidget())->count()>0) {
//        dynamic_cast<FSMTabPane*>(this->centralWidget())->removeTab(1);
//    }

}

void MergedMainWindow::settingsChanged() {

}

void MergedMainWindow::openRecentFile(QListWidgetItem * item) {

    //-- Open
    this->openFile(item->text());

}

void MergedMainWindow::openFile(QAction * sourceAction) {

//    cout << "Opening RecentFile: " << sourceAction->data().toString().toStdString() << endl;
    this->openFile(sourceAction->data().toString());
}

void MergedMainWindow::openFile(QString path) {

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

        //-- Close Project
        this->closeProject();

        //-- Load Project
        LoadFactory::getLoad_I().load(file.toStdString());

        //-- Open Project
        this->openProject();

        //-- Record in Recent Files
        GuiSettings::addRecentFile(file);

    }
    // Open a single FSM
    //----------------------
    else if (file.endsWith(".xml")) {

        //-- Import to Project
        QMessageBox::information(
                this,
                "FSM Import",
                "You have opened a single FSM XML file. The FSM will be imported in the currently opened project");

        //-- Close Project
        this->closeProject();

        //-- Load File
        LoadFactory::getLoad_I().load(file.toStdString());

        //-- Open Project
        this->openProject();

        //-- Record in Recent Files
        GuiSettings::addRecentFile(file);

    }

    //-- Update UI
    //this->modelChanged();

}

void MergedMainWindow::saveProjectAs() {

    // Ask Where to save
    //----------------------
    QString file = QFileDialog::getSaveFileName(this,
            "Choose a filename to save",
            Core::getInstance()->getProject()->getDirectory().absolutePath(),
            "FSMDesigner Project files (*.pro.xml)");
    if (file.length() > 0) {

        //-- Ensure we have .pro.xml at the end
        if (!file.endsWith(".pro.xml"))
            file += ".pro.xml";

        //-- Set File to Project
        Core::getInstance()->getProject()->setFileInfo(QFileInfo(file));

        //-- Save
        QApplication::setOverrideCursor(Qt::WaitCursor);

        Save_I& s = SaveFactory::getSave_I();
        s.saveProject(
                Core::getInstance()->getProject()->getFileInfo().absoluteFilePath().toStdString());

        //-- Record in Recent Files
        GuiSettings::addRecentFile(file);

        QApplication::restoreOverrideCursor();
    }
}

void MergedMainWindow::saveProject() {

    // Check There is something to save
    //---------------------------------------

    //-- If Project as a Save Location, save back, if not save as
    if (!Core::getInstance()->getProject()->isFileInfo()) {
        this->saveProjectAs();
    } else {

        //-- Save
        QApplication::setOverrideCursor(Qt::WaitCursor);
        Save_I& s = SaveFactory::getSave_I();
        s.saveProject(
                Core::getInstance()->getProject()->getFileInfo().absoluteFilePath().toStdString());
        QApplication::restoreOverrideCursor();
    }

}

void MergedMainWindow::createProject() {

    //-- Get Name from Line edit
    //QString name = this->projectCreateName->text();

    //-- Call real create
    // this->createProject(name);

}

void MergedMainWindow::createProject(QString name) {

    //-- Create new project
    this->newProject();

    //-- Set name of new project
    Core::getInstance()->getProject()->setName(name);

}

void MergedMainWindow::renameProject(QString newName) {

    //-- Ask for name if not provided
    if (newName == "") {

        newName = QInputDialog::getText(this, "Rename Project",
                "New Project Name:", QLineEdit::Normal,
                Core::getInstance()->getProject()->getName());

        //-- Name can't be zerolength
        if (newName == "") {
            QMessageBox::critical(
                    this,
                    "Rename Project",
                    "Name provided to rename the project can't be an empty string");
            return;
        }

    }

    //-- Rename
    Core::getInstance()->getProject()->setName(newName);

    //-- Update UI
    //this->modelChanged();

}

void MergedMainWindow::showGlobalTable() {

    // Show Only If there is a selected Scene
    //--------------------
    Scene * selectedScene =
            dynamic_cast<FSMTabPane *>(this->centralWidget())->getSelectedScene();
    if (selectedScene == NULL)
        return;

    // Create Dialog
    Table * table = new Table(selectedScene, this);

    // Show
    table->show();
}

void MergedMainWindow::print() {

    // Show Only If there is a selected Scene
    //--------------------
    Scene * selectedScene =
            dynamic_cast<FSMTabPane *>(this->centralWidget())->getSelectedScene();
    if (selectedScene == NULL)
        return;

    // Space taken by the FSM on the scene
    QGraphicsView * view = selectedScene->views().first();
    QRectF brect = selectedScene->itemsBoundingRect();

    // Show Print Preview dialog
    //---------------------
    QPrintPreviewDialog previewDialog;

    //-- Connect
    this->connect(&previewDialog,SIGNAL(paintRequested(QPrinter*)),SLOT(print(QPrinter*)));
    previewDialog.exec();

    //if (previewDialog.exec() == QDialog::Accepted)
    //    return;

    //-- Disconnect
    previewDialog.disconnect(this,SLOT(print(QPrinter)));

/*
    // Open Print Dialog
    //------------------
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print FSM"));
    if (dialog->exec() != QDialog::Accepted)
     return;

    // Print if Accepted
    QPainter painter(&printer);
    selectedScene->render(&painter,QRectF(),brect);*/



}

void MergedMainWindow::print(QPrinter * printer) {

    // Print only If there is a selected Scene
    //--------------------
    Scene * selectedScene =
            dynamic_cast<FSMTabPane *>(this->centralWidget())->getSelectedScene();
    if (selectedScene == NULL)
        return;

    // Space taken by the FSM on the scene
    QRectF brect = selectedScene->itemsBoundingRect();

    // Do rendering
    QPainter painter(printer);
    selectedScene->render(&painter,QRectF(),brect);



}

void MergedMainWindow::exportAsSVG() {

    // Show Only If there is a selected Scene
    //--------------------
    Scene * selectedScene =
            dynamic_cast<FSMTabPane *>(this->centralWidget())->getSelectedScene();
    if (selectedScene == NULL)
        return;

    QRectF brect = selectedScene->itemsBoundingRect();

    QSvgGenerator generator;
    generator.setFileName("test.svg");
    generator.setSize(QSize(1024, 768));
    generator.setViewBox(QRect(0, 0, 1024, 768));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt."));

}

