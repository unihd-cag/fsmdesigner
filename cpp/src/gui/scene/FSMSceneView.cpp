/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you this->scene() redistribute it and/or modify *
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
//-------------

//-- Std
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
using namespace std;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
#include <core/Fsm.h>
#include <core/Core.h>
#include <core/Project.h>
#include <core/SaveFactory.h>
#include <core/Save_I.h>
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Link.h>
#include <core/User.h>

//-- Verify
#include <verification/verify.h>
#include <verification/Verificator.h>
#include <verification/VerificationListener.h>
#include <verification/StateOutputsRule.h>
#include <verification/StateTransitions.h>
#include <verification/OverlappingTransitionsRule.h>

#include <genverilog/VerilogGeneratorChecks.h>


#include <gui/verify/TableVerificationListener.h>
#include <gui/verify/FSMVerificator.h>

//-- Generation
#include <generate/GeneratorFactory.h>

//-- Core
#include <core/FSMDesigner.h>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>

#include <gui/common/RoundedPanel.h>
#include <gui/common/propertywidget.h>
#include <gui/common/FAction.h>
#include <gui/common/StatusToolBar.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>
#include <gui/scene/selectionanimator.h>

#include <gui/tabbedGUI/FSMTabPane.h>

#include <gui/output/outputwidget.h>
#include <gui/input/conditionwidget.h>
#include <gui/settings/GuiSettings.h>
#include <gui/table/table.h>


//-- Gui: Items

#include "FSMSceneView.h"
FSMSceneView::FSMSceneView(Scene* scene, QWidget* parent) :
	QGraphicsView(scene, parent) {

	//-- Prepare variables
	this->initVariables();
	this->setRelatedScene(scene);

	//-- Init Control Panel && Items Panel
	//----------------------
	this->setContentsMargins(10, 10, 10, 10);
	this->viewport()->setContentsMargins(0, 0, 0, 0);
	this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

	//-- Create Control ToolBar
	this->controlToolBar = new StatusToolBar(this);
	this->controlToolBar->move(5,5);


	//---- Help and so
	//-----------------------
	FAction * renameAction = this->controlToolBar->addAction(QIcon(QPixmap(":/icons/Rename")),"Rename FSM");
    this->connect(renameAction, SIGNAL(triggered()), SLOT(renameFsm()));

    this->controlToolBar->getToolBar()->addSeparator();

	//---- Verification
	//------------------------

	// Verify
    QToolButton * verifyButton = this->controlToolBar->addActionButton(QIcon(QPixmap(":/icons/verification.png")),"Quick FSM check");
	this->connect(verifyButton->defaultAction(),SIGNAL(triggered()),this->getRelatedScene(),SLOT(verify()));

	// Verify in details
	QAction * verifyDetailsAction = new QAction("Check FSM...",verifyButton);
	verifyButton->addAction(verifyDetailsAction);
    this->connect(verifyDetailsAction,SIGNAL(triggered()),SLOT(verifyTable()));

	// Verify Clear
	FAction * verifyClearAction = this->controlToolBar->addAction(QIcon(QPixmap(":/icons/Clear-brush")),"Clear check results");
    this->connect(verifyClearAction,SIGNAL(triggered()),this->getRelatedScene()->getFSMVerificator(),SLOT(reset()));

	this->controlToolBar->getToolBar()->addSeparator();

	//---- Edition
    //------------------------

	// Table
	FAction * tableAction = this->controlToolBar->addAction(QIcon(QPixmap(":/icons/TableEditor28")),"Table Editor");
    this->connect(tableAction,SIGNAL(triggered()),SLOT(showGlobalTable()));

    this->controlToolBar->getToolBar()->addSeparator();

    //---- Generate
    //------------------------

    // Fast verilog
    QToolButton * reloadVerilogButton = this->controlToolBar->addActionButton(QIcon(QPixmap(":/icons/GenerateVerilog28")),"(Re)Generate Verilog");
    this->connect(reloadVerilogButton->actions().first(),SIGNAL(triggered()),SLOT(generateVerilogReload()));

    // Normal Generate
    QAction * reloadVerilogAction = new QAction("Generate to...",reloadVerilogButton);
    reloadVerilogButton->addAction(reloadVerilogAction);
    this->connect(reloadVerilogAction,SIGNAL(triggered()),SLOT(generateVerilog()));



    this->controlToolBar->getToolBar()->addSeparator();

	//-- Initialise Scene per default to see
	//------------------
	scene->initializeScene();


	// Help Panel
	//-----------------------
	this->helpPanel = new QTextBrowser(this);
	//this->helpPanel->setBaseSize(250,600);
	//this->helpPanel->setFixedSize(250,600);
	this->helpPanel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	this->helpPanel->setText("Hello world <b>Hey</b>");
	this->helpPanel->setTextBackgroundColor(Qt::yellow);


	//---- Update Help on sceneSelection Change
	this->connect(this->scene(),SIGNAL(selectionChanged()),SLOT(sceneSelectionChanged()));

	// Update UI when settings get changed
	//----------------------
	this->connect(GuiSettings::getAsQObject(), SIGNAL(settingsChanged()),
	            SLOT(settingsChanged()));
    this->settingsChanged();

	//-- Update UI
	//--------------------------
	this->fit();




}

FSMSceneView::~FSMSceneView() {
	//delete this->scene();
}


void FSMSceneView::settingsChanged() {

    // Update enabled state
    this->helpPanel->setEnabled(GuiSettings::value("uni.hd.ziti.fsmdesigner.ui.general.help",true).toBool());
    this->changeHelp();

}


void FSMSceneView::initVariables() {

	// Instanciate the SelectionAnimator
	//---------------
	this->selectionAnimator = new SelectionAnimator(this->scene());

	// Area selection
	this->areaSelectionInitial = NULL;

	// Selection area
	//-----------------------
	this->selectionAreaPen = NULL;

	// Update strategy
	//-----------
	//this->setCacheMode(QGraphicsView::CacheBackground);
	//this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	//this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate); // Repaints the whole graphic when something is changed or scrolled
	this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); // Repaints the whole graphic when something is changed or scrolled
	this->setRenderHint(QPainter::Antialiasing,true);


	// Lock Widget
    //-------------------
    QPixmap lockPixMap(":/icons/Lock64");

    this->lockIcon = new QLabel(this);
    this->lockIcon->setPixmap(lockPixMap);
    this->lockIcon->setVisible(false);

    this->lockCursor = new QCursor(lockPixMap);



}


void FSMSceneView::showGlobalTable() {

    // Show Only If there is a selected Scene
    //--------------------
    Scene * selectedScene = this->getRelatedScene();
    if (selectedScene==NULL)
        return;


    // Create Dialog
    Table * table = new Table(selectedScene,this);

    // Show
    table->show();
}

void FSMSceneView::toggleHelp() {

    // Toggle Help
    this->helpPanel->setEnabled(!this->helpPanel->isEnabled());
    this->changeHelp();

}

void FSMSceneView::changeHelp() {

    QList<QString> newHelpTexts;


    // If not enabled -> Don't update/display
    //--------------

    // Place Mode Help
    //------------------------
    if (this->helpPanel->isEnabled() && this->getRelatedScene()->getPlaceMode()!=FSMDesigner::CHOOSE) {

        // Add Global Place help
        newHelpTexts.append(":/help/PlaceMode");

        // Add Specific Helps
        if (this->getRelatedScene()->getPlaceMode()==FSMDesigner::STATE) {
            newHelpTexts.append(":/help/PlaceMode_State");
        } else if (this->getRelatedScene()->getPlaceMode()==FSMDesigner::JOIN) {
            newHelpTexts.append(":/help/PlaceMode_Join");
        }

    }
    // Selected Item Help
    //  - Multiple Selection: Don't display anything
    //-----------------------
    else if (this->helpPanel->isEnabled()) {
        QGraphicsItem * selectedItem = this->scene()->selectedItems().size()==1 ? this->scene()->selectedItems().first() : NULL;

        if (selectedItem!=NULL)
            switch (selectedItem->type()) {

                case FSMGraphicsItem<>::STATEITEM:
                    newHelpTexts.append(":/help/StateItem");
                break;

                default:
                break;
            }
    }

    //-- Set new Help using defined resource locations
    //--------------------
    if (newHelpTexts.size()>0) {

        // Merge all provided resource locations into the resulting string
        QString resultHelpText = "";
        for (QList<QString>::iterator it = newHelpTexts.begin() ; it != newHelpTexts.end(); it++ ) {
            QFile file(*it);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);
            resultHelpText.append(in.readAll());
            file.close();
        }

        // Set on panel
        this->helpPanel->setText(resultHelpText);
        this->helpPanel->setVisible(true);


    } else {
        this->helpPanel->setText("");
        this->helpPanel->setVisible(false);
    }

    // Reset Size
    //--------------------
    this->helpPanel->setBaseSize(250,this->height()*80/100);
    this->helpPanel->setFixedSize(250,this->height()*80/100);
    this->helpPanel->setMaximumSize(250,this->height()*80/100);


}


void FSMSceneView::drawForeground(QPainter * painter, const QRectF & rect) {

    //-- Parent Job
    QGraphicsView::drawForeground (painter, rect ) ;

	if (this->areaSelectionInitial == NULL)
			return;

	//-- Prepare Pen
	if (this->selectionAreaPen == NULL) {
		QBrush brush(Qt::Dense1Pattern);
		brush.setColor(Qt::gray);
		this->selectionAreaPen = new QPen(brush,Qt::DashLine);
	}


	// Paint selection area if necessary
	QPainterPath path = this->scene()->selectionArea();
	//path = this->mapFromScene(path);
	QPolygonF polygon = path.toFillPolygon();

	//this->update(path);

	// Do paint
	QBrush brush(Qt::Dense1Pattern);
	brush.setColor(Qt::gray);
	painter->save();
	painter->setOpacity(0.15);
	painter->setBrush(brush);
	painter->setPen(QPen(Qt::DashLine));
	painter->drawPath(path);
	painter->restore();




}

void FSMSceneView::mouseMoveEvent(QMouseEvent* e) {

	// Parent job
	QGraphicsView::mouseMoveEvent(e);

	// Cursor Display on place mode
	//-------------
	if (((Scene*) this->scene())->getPlaceMode() != FSMDesigner::CHOOSE) {

		// Depending on choose mode, use cursos to help on certain rules
	    return;
	}

	// Update Selection area
	//----------------
	if (this->areaSelectionInitial != NULL) {

	    // hold previous area
	    QPainterPath oldArea = this->scene()->selectionArea();

		// Convert position to scene coordinates
		QPointF initialOnScene = this->mapToScene(
				this->areaSelectionInitial->toPoint());
		QPointF currentOnScene = this->mapToScene(e->pos());

		// Painter path to selection
		QPainterPath selectionPath = QPainterPath(initialOnScene);
		selectionPath.addRoundRect(initialOnScene.x(), initialOnScene.y(),
				currentOnScene.x() - initialOnScene.x(), currentOnScene.y()
						- initialOnScene.y(), 10);
		this->scene()->setSelectionArea(selectionPath);

		// Update on biggest area between new and old one
		//----------------
		if (selectionPath.boundingRect().contains(oldArea.boundingRect()))
		    this->scene()->update(selectionPath.boundingRect());
		else
		    this->scene()->update(oldArea.boundingRect());
	}
}

void FSMSceneView::mouseReleaseEvent(QMouseEvent* e) {

	//-- Parent job
	QGraphicsView::mouseReleaseEvent(e);

	//-- Deactivate areaSelection
	if (this->areaSelectionInitial != NULL) {
	    this->scene()->update(this->scene()->selectionArea().boundingRect());
		delete this->areaSelectionInitial;
	}
	this->areaSelectionInitial = NULL;

}

void FSMSceneView::mousePressEvent(QMouseEvent* e) {

	// Must call parent
    //-------------------------
    QGraphicsView::mousePressEvent(e);


    // If Placing something -> don't do anything
    if (((Scene*) this->scene())->getPlaceMode() != FSMDesigner::CHOOSE) {
        return;
    }


    // MIddle Button -> fit outszoomed
    //--------------------
	if (e->button() == Qt::MidButton) {
		resetTransform();
		ensureVisible(QRectF(0, 0, 0, 0)); //Scrolls so that point (0,0) is visible
	}

	//-- With left button, check we are not in a special object add mode
	//--------------------
	if (e->button() == Qt::LeftButton) {

		// Init area selection if nothing else selected
		if (this->scene()->selectedItems().size() == 0) {
			this->areaSelectionInitial = new QPointF(e->posF());
			this->scene()->setSelectionArea(QPainterPath(
					*this->areaSelectionInitial));
		}
	}
}

//Zoom in and out with the  mouse Wheel
void FSMSceneView::wheelEvent(QWheelEvent *event) {



	if (event->modifiers() == Qt::CTRL) {

		//-- Center on the position

		//this->centerOn(scenePos);

		//-- Calculate the scaling factor depending on wheeled degrees
		double step = 30;

		double numDegrees = std::abs(event->delta()) / 8.0;
		double numSteps = numDegrees / step;
		double factor = std::pow(1.125, numSteps);

		// Scale
		// To scale backward, use 1/factor, not -factor
		factor = 1.08;
		factor = event->delta() < 0 ? 1 / factor : factor;

		//-- Get the new scenePos
		QPointF scenePos = this->mapToScene(event->pos());

		//-- Scale and center or center and scale
		if (factor<1) {
			//this->centerOn(scenePos);
			this->scale(factor, factor);
		} else {
			this->scale(factor, factor);
			//this->centerOn(scenePos);
		}


	} else {

		//-- Default behaviour is to scroll
		QGraphicsView::wheelEvent(event);


	}
}

void FSMSceneView::sceneSelectionChanged() {

    // Update Help
    //--------------------------
    this->changeHelp();




}


void FSMSceneView::resizeEvent(QResizeEvent * event) {
    // Call Parent
	QGraphicsView::resizeEvent(event);



	//-- Replace help panel
	//---------------------
	this->helpPanel->move(event->size().width()-(this->helpPanel->baseSize().width()+20),5);
	this->changeHelp();

	//-- Replace Lock Icon
	//----------
	this->lockIcon->move(10,event->size().height()-(this->lockIcon->size().height()+35));



}

void FSMSceneView::showEvent ( QShowEvent * event ) {

	//qDebug() << "Show scene view";

	//-- Parent call
	QGraphicsView::showEvent(event);


	//qDebug() << "Done showEvent the scene view";

}

void FSMSceneView::scrollContentsBy(int dx, int dy) {

	//-- Parent
	QGraphicsView::scrollContentsBy(dx, dy);

}



void FSMSceneView::keyPressEvent(QKeyEvent* ke) {


	//-- Parent processing
	QGraphicsView::keyPressEvent(ke);


	//-- Pressing Ctrl Locks placement
	if (ke->key() ==  Qt::Key_Control) {
	    this->lockIcon->setVisible(true);
	}
	if (ke->key() ==  Qt::Key_Control && this->getRelatedScene()->getPlaceMode()!=FSMDesigner::CHOOSE) {

		//-- Place mode
		this->getRelatedScene()->setPlaceLocked();

		//-- Show Lock
//		this->setCursor(QCursor((*lockCursor)));
		//this->lockIcon->setVisible(true);
	}




}

void FSMSceneView::keyReleaseEvent(QKeyEvent* ke) {

	// Parent processing
	//---------------------------
	QGraphicsView::keyReleaseEvent(ke);

	//-- Restore help
	this->changeHelp();

	//-- Filter out accepted events
	if (ke->isAccepted())
		return;

	// f : Fits
	//---------------
	if (ke->key() == Qt::Key_F) {

		this->fit();

	}
	// F11 = Toggle Maximised
	//----------------------------
	else if (ke->key() == Qt::Key_F11) {


		if (this->window()->windowState() == Qt::WindowFullScreen) {
			// Restore geometry
			this->window()->setWindowState(Qt::WindowMaximized);
		} else {
			this->window()->setWindowState(Qt::WindowFullScreen);
		}

	}
	// Releasing CTRL unlocks placement
	//-----------
	else if (ke->key() ==  Qt::Key_Control) {

		//-- Place mode
		this->getRelatedScene()->setPlaceUnlocked();

		//-- Hide Lock
		this->lockIcon->setVisible(false);

	}



}

void FSMSceneView::fit() {

	//-- Get Items bouding rect
	QRectF itemsBoundingRect = this->scene()->itemsBoundingRect();
	QRectF fitRectScene = this->scene()->sceneRect();

	//-- Center on it
	if (itemsBoundingRect.isValid())
		this->fitInView(itemsBoundingRect, Qt::KeepAspectRatio);


}


void FSMSceneView::placeSetUnlocked() {
	((Scene*) this->scene())->setPlaceUnlocked();
}

void FSMSceneView::placeSetLocked() {
	((Scene*) this->scene())->setPlaceLocked();
}

void FSMSceneView::placeSetMode(FSMDesigner::Item placeMode) {

    // Setting Place mode
    //---------------
	((Scene*) this->scene())->setPlaceMode(placeMode);

	// Update Help Panel
	//----------------------
	this->changeHelp();
}


void FSMSceneView::generateVerilog() {

	//-- Get FSM
	Fsm * fsm = dynamic_cast<Scene*> (this->scene())->getFsm();

	// Verify FSM
	//----------------
	Verify * verifier = new Verify(fsm,this);
	if (verifier->wanttoverify()) {
		// Do verify
		verifier->verifyFsm();
		verifier->info();

		// Check passed
		if (!verifier->getVerified())
			return;

	}


	//-- Default path is where the FSM file is
    QDir location = Core::getInstance()->getProject()->getDirectory();

    //-- FileName offer
    QString defaultName = QString::fromStdString(fsm->getFsmName())+"_fsm.v";

    //-- Save path
    QString path = QFileDialog::getSaveFileName(this,"Choose a Verilog File to save to",location.path(),"Verilog Files (*.v)");
    if (path.length()==0)
        return;
    fsm->setParameter("ui.generator.verilog.lastPath",path.toStdString());

	// Now that we have a last generated path -> call on reload
	//---------------
    this->generateVerilogReload();

}

void FSMSceneView::generateVerilogReload() {

	//-- Is there a previously saved path ?

	// Get FSM
	Fsm * fsm = dynamic_cast<Scene*> (this->scene())->getFsm();



    // Verify FSM
    //----------------
    Verify * verifier = new Verify(fsm,this);
    verifier->verifyFsm();

    // Check passed
    if (!verifier->getVerified()) {
        verifier->info();
        return;
    }


    //-- If there is no previous recorded path -> ask for one
    QString path = QString::fromStdString(fsm->getParameter("ui.generator.verilog.lastPath",""));
    if (path.size()==0) {

        //-- Default path is where the FSM file is
        QDir location = Core::getInstance()->getProject()->getDirectory();

        //-- FileName offer
        QString defaultName = QString::fromStdString(fsm->getFsmName())+"_fsm.v";

        //-- Save path
        path = QFileDialog::getSaveFileName(this,"Choose a Verilog File to save to",location.path(),"Verilog Files (*.v)");
        if (path.length()==0)
            return;
        fsm->setParameter("ui.generator.verilog.lastPath",path.toStdString());

    }

    // Generate
    //--------------------

    //---- Verilog
    //----------------------
    Generator * generator = GeneratorFactory::getInstance()->newGenerator("Verilog");
    if (generator==NULL) {
        QMessageBox warnBox(QMessageBox::Warning,"Cannot find generator","There are no Generator registered under the 'Verilog' name. No Verilog can generated",QMessageBox::NoButton,this);
        warnBox.exec();
        return;
    }

    //-- Open File
    QFile verilogFile(path);
    if (!verilogFile.open(QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {

        QMessageBox warnBox(QMessageBox::Warning,"Cannot open File","The provided file to generate verilog to cannot be opened for writing",QMessageBox::NoButton,this);
        warnBox.exec();
        return;

    }

    //-- Generate
    QDataStream outputStream(&verilogFile);
    generator->generate(fsm,&outputStream);

    //-- Close
    delete generator;
    verilogFile.close();

    //---- Simvision mmap
    //---------------------
    generator = GeneratorFactory::getInstance()->newGenerator("Simvision_Mmap");
    if (generator==NULL) {
        QMessageBox warnBox(QMessageBox::Warning,"Cannot find generator","There are no Generator registered under the 'Simvision_Mmap' name. No Simvision Mmap can generated",QMessageBox::NoButton,this);
        warnBox.exec();
        return;
    }

    //-- Open File
    QFile simvisionFile(path.replace(".v",".svcf"));
    if (!simvisionFile.open(QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {

        QMessageBox warnBox(QMessageBox::Warning,"Cannot open File","The provided file to generate Simvision Mmap to cannot be opened for writing",QMessageBox::NoButton,this);
        warnBox.exec();
        return;

    }

    //-- Generate
    QDataStream simvisionOutputStream(&simvisionFile);
    generator->generate(fsm,&simvisionOutputStream);

    //-- Close
    delete generator;
    simvisionFile.close();

}

void FSMSceneView::saveProjectAs() {

	// Ask Where to save
	//----------------------
	QString file = QFileDialog::getSaveFileName(this,
			"Choose a filename to save",
			Core::getInstance()->getProject()->getDirectory().absolutePath(),
			"FSMDesigner Project files (*.pro.xml)");
	if (file.length() > 0) {

		//-- Ensure we have .pro.xml at the end
		if (!file.endsWith(".pro.xml"))
			file+=".pro.xml";

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

void FSMSceneView::saveProject() {

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

void FSMSceneView::renameFsm() {

	//-- Open an Input Dialog
	bool accepted = false;
	QString newName = 	QInputDialog::getText(
			this,
			"Rename FSM",
			"New name:",
			QLineEdit::Normal,
			QString::fromStdString(this->getRelatedScene()->getFsm()->getFsmName()),
			&accepted );

	//-- Accepted -> rename and try to propagate change
	if (accepted) {

		//-- Text can't be zero length
		if (newName.length()==0) {

			QMessageBox::critical(this, "Rename FSM", "New name MUST NOT be an empty string");

		} else {

			//-- Do rename
			this->getRelatedScene()->getFsm()->setName(newName.toStdString());

			//-- FIXME Propagate to title
			dynamic_cast<FSMTabPane*>(this->parentWidget()->parentWidget())->setTabText(
			        dynamic_cast<FSMTabPane*>(this->parentWidget()->parentWidget())->indexOf(this),
			        newName);

		}
	}

}

void FSMSceneView::verifyTable() {

    // Reset
    //--------------
    if (this->getRelatedScene()->getFSMVerificator()!=NULL) {
        this->getRelatedScene()->getFSMVerificator()->reset();
    }

    // Create Table verify listener
    //---------------
    QDialog dialog(this);
    dialog.setBaseSize(this->width(),600);
    dialog.setFixedSize(this->width(),600);
    dialog.setLayout(new QVBoxLayout());

    //-- Add Table to dialog
    TableVerificationListener listener;
    listener.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dialog.layout()->addWidget(&listener);

    //-- Add Ok button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    dialog.layout()->addWidget(&buttonBox);
    dialog.connect(&buttonBox,SIGNAL(accepted()),&dialog,SLOT(close()));

    // Verify
    //--------------
    Verificator * verificator = new Verificator();
    verificator->addRule(new StateOutputsRule());
    verificator->addRule(new StateTransitions());
    verificator->addRule(new OverlappingTransitionsRule());
    verificator->addRule(new VerilogGeneratorChecks());
    verificator->verify(this->getRelatedScene()->getFsm(),&listener);

    delete verificator;

    // Show Results
    //-----------------
    dialog.exec();


}
