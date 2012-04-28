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
#include <gui/verify/FSMVerificator.h>

//-- Generation
#include <genverilog/generationofverilog.h>

//-- Core
#include <core/FSMDesigner.h>

//-- Gui
#include <gui/items/FSMGraphicsItem.h>

#include <gui/common/RoundedPanel.h>
#include <gui/common/propertywidget.h>
#include <gui/common/FAction.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>
#include <gui/scene/selectionanimator.h>

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

	//-- Create Control Panel
	this->controlPanel = new QWidget(this);
	this->controlPanel->move(5,5);
	this->controlPanel->setLayout(new QVBoxLayout(this->controlPanel));

	//-- Create Toolbar
	//----------
	QToolBar * toolbar = new QToolBar();
	this->controlPanel->layout()->addWidget(toolbar);

	//---- Verification
	//------------------------

	// Verify
    FAction * verifyAction = new FAction(QIcon(QPixmap(":/icons/verification.png")));
    verifyAction->setWhatsThis("Quick Verify");
	toolbar->addAction(verifyAction);
	this->connect(verifyAction,SIGNAL(hovered(FAction*)),SLOT(toolbarActionHovered(FAction*)));
	this->connect(verifyAction,SIGNAL(hoveredLeave(FAction*)),SLOT(toolbarActionHoveredLeft(FAction*)));
	this->connect(verifyAction,SIGNAL(triggered()),this->getRelatedScene()->getFSMVerificator(),SLOT(verify()));

	// Verify Clear
	FAction * verifyClearAction = new FAction(QIcon(QPixmap(":/icons/Clear-brush")));
	verifyClearAction->setWhatsThis("Clear Verification Display");
    toolbar->addAction(verifyClearAction);
    this->connect(verifyClearAction,SIGNAL(hovered(FAction*)),SLOT(toolbarActionHovered(FAction*)));
    this->connect(verifyClearAction,SIGNAL(hoveredLeave(FAction*)),SLOT(toolbarActionHoveredLeft(FAction*)));
    this->connect(verifyClearAction,SIGNAL(triggered()),this->getRelatedScene()->getFSMVerificator(),SLOT(reset()));

	toolbar->addSeparator();

	//---- Edition
    //------------------------

	// Table
    FAction * tableAction = new FAction(QIcon(QPixmap(":/icons/TableEditor28")));
    tableAction->setWhatsThis("Table Editor");
    toolbar->addAction(tableAction);
    this->connect(tableAction,SIGNAL(hovered(FAction*)),SLOT(toolbarActionHovered(FAction*)));
    this->connect(tableAction,SIGNAL(hoveredLeave(FAction*)),SLOT(toolbarActionHoveredLeft(FAction*)));
    this->connect(tableAction,SIGNAL(triggered()),SLOT(showGlobalTable()));

    toolbar->addSeparator();

    //---- Generate
    //------------------------

    // Fast verilog
    FAction * reloadVerilogAction = new FAction(QIcon(QPixmap(":/icons/GenerateVerilog28")));
    reloadVerilogAction->setWhatsThis("Fast Regenerate Verilog");
    toolbar->addAction(reloadVerilogAction);
    this->connect(reloadVerilogAction,SIGNAL(hovered(FAction*)),SLOT(toolbarActionHovered(FAction*)));
    this->connect(reloadVerilogAction,SIGNAL(hoveredLeave(FAction*)),SLOT(toolbarActionHoveredLeft(FAction*)));
    this->connect(reloadVerilogAction,SIGNAL(triggered()),SLOT(generateVerilogReload()));

    toolbar->addSeparator();


	//-- Create Status Label
	//---------
	QLabel * label = new QLabel();
	label->setUpdatesEnabled(true);
	this->controlPanel->layout()->addWidget(label);

	//-- Initialise Scene per default to see
	//------------------
	scene->initializeScene();


	// Help Panel
	//-----------------------
	this->helpPanel = new QTextBrowser(this);
	this->helpPanel->setBaseSize(250,400);
	this->helpPanel->setText("Hello world <b>Hey</b>");
	this->helpPanel->setTextBackgroundColor(Qt::yellow);

	QPalette * p = new QPalette();
	p->setColor(QPalette::Background,QColor(Qt::yellow));
	this->helpPanel->setPalette(*p);
	this->helpPanel->setAutoFillBackground(true);
	this->helpPanel->setBackgroundRole(QPalette::Background);
	//this->helpPanel->palette().setColor(QPalette::Background,QColor(Qt::yellow));

	//---- Update Help on sceneSelection Change
	this->connect(this->scene(),SIGNAL(selectionChanged()),SLOT(changeHelp()));

	//-- Update UI
	//--------------------------
	this->fit();



}

FSMSceneView::~FSMSceneView() {
	//delete this->scene();
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
    QPixmap lockPixMap(":/icons/Lock-icon.png");

    this->lockModeicon = new QWidget(this->viewport());
    this->lockModeicon->setLayout(new QHBoxLayout());
    this->lockModeicon->setAutoFillBackground(true);
    this->lockModeicon->setPalette(QPalette(Qt::lightGray));
    this->lockModeicon->setVisible(false);

    QLabel * locklabel = new QLabel();
    locklabel->setPixmap(lockPixMap);
    this->lockModeicon->layout()->addWidget(locklabel);


}

void FSMSceneView::toolbarActionHovered(FAction * action) {

    //qDebug() << "Action: " << action->toolTip();
    //qDebug() << "Ctrl Panel children: " << this->controlPanel->children().size() << "//" << this->controlPanel->layout()->children().size();

    dynamic_cast<QLabel*>(this->controlPanel->children().at(2))->setText(action->whatsThis());
}

void FSMSceneView::toolbarActionHoveredLeft(FAction * action) {

    //qDebug() << "Action: " << action->toolTip();
    //qDebug() << "Ctrl Panel children: " << this->controlPanel->children().size() << "//" << this->controlPanel->layout()->children().size();

    dynamic_cast<QLabel*>(this->controlPanel->children().at(2))->setText("");
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

void FSMSceneView::changeHelp() {

    QString newHelpText;

    // Get Selected Item
    //  - Multiple Selection: Don't display anything
    //-----------------------
    QGraphicsItem * selectedItem = this->scene()->selectedItems().size()==1 ? this->scene()->selectedItems().first() : NULL;

    if (selectedItem!=NULL)
        switch (selectedItem->type()) {

            case FSMGraphicsItem<>::STATEITEM:

                newHelpText.append(":/help/StateItem");

            break;

            default:
            break;
        }


    //-- Set new Help using defined resource location
    if (newHelpText.size()>0) {

        QFile file(newHelpText);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        this->helpPanel->setText(in.readAll());
        this->helpPanel->setVisible(true);
        file.close();


    } else {
        this->helpPanel->setText("");
        this->helpPanel->setVisible(false);
    }


}


void FSMSceneView::drawForeground(QPainter * painter, const QRectF & rect) {



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
	//painter->save();
	painter->setOpacity(0.15);
	painter->setBrush(brush);
	painter->setPen(QPen(Qt::DashLine));
	painter->drawPath(path);
	//painter->restore();


	//-- Parent Job
	QGraphicsView::drawForeground (painter, rect ) ;

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

		// Convert position to scene coordinates
		QPointF initialOnScene = this->mapToScene(
				this->areaSelectionInitial->toPoint());
		QPointF currentOnScene = this->mapToScene(e->pos());

		// Painter path to selection
		QPainterPath* selectionPath = new QPainterPath(initialOnScene);
		selectionPath->addRoundRect(initialOnScene.x(), initialOnScene.y(),
				currentOnScene.x() - initialOnScene.x(), currentOnScene.y()
						- initialOnScene.y(), 10);
		this->scene()->setSelectionArea(*selectionPath);

		this->scene()->update();

	}
}

void FSMSceneView::mouseReleaseEvent(QMouseEvent* e) {

	//-- Parent job
	QGraphicsView::mouseReleaseEvent(e);

	//-- Deactivate areaSelection
	if (this->areaSelectionInitial != NULL) {
		delete this->areaSelectionInitial;
		this->scene()->update();
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

	//-- Selection list
	QList<QGraphicsItem*> selectedItems = this->scene()->selectedItems();

	//-- Do nothing if nothing selected
	if (selectedItems.size() == 0)
		return;

	//-- Get First
	QGraphicsItem * firstSelected = selectedItems.first();

	// Update the Properties Widget
	//-------------------------------
	/*if (firstSelected->type()==StateItem::Type) {
	 this->controlPropertyWidget->editState(dynamic_cast<StateItem*>(firstSelected));
	 } else if (firstSelected->type()==Transline::Type) {
	 this->controlPropertyWidget->editTransition(dynamic_cast<Transline*>(firstSelected));
	 }*/

}


void FSMSceneView::resizeEvent(QResizeEvent * event) {
    // Call Parent
	QGraphicsView::resizeEvent(event);



	//-- Replace help panel
	//---------------------
	this->helpPanel->move(event->size().width()-this->helpPanel->baseSize().width()-20,5);




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
	if (ke->key() ==  Qt::Key_Control && !ke->isAccepted()) {

		//-- Place mode
		this->getRelatedScene()->setPlaceLocked();

		//-- Show Lock
		this->lockModeicon->setVisible(true);
	}




}

void FSMSceneView::keyReleaseEvent(QKeyEvent* ke) {

	// Parent processing
	//---------------------------
	QGraphicsView::keyReleaseEvent(ke);

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


		/*if (this->window()->windowState() == Qt::WindowFullScreen) {
			// Restore geometry
			this->window()->setWindowState(Qt::WindowMaximized);
		} else {
			this->window()->setWindowState(Qt::WindowFullScreen);
		}*/

	}
	// Releasing CTRL unlocks placement
	//-----------
	else if (ke->key() ==  Qt::Key_Control) {

		//-- Place mode
		this->getRelatedScene()->setPlaceUnlocked();

		//-- Hide Lock
		this->lockModeicon->setVisible(false);

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
	((Scene*) this->scene())->setPlaceMode(placeMode);
}


void FSMSceneView::generateVerilog() {

	//-- Get FSM
	Fsm * f = dynamic_cast<Scene*> (this->scene())->getFsm();

	// Verify FSM
	//----------------
	Verify * verifier = new Verify(f,this);
	if (verifier->wanttoverify()) {
		// Do verify
		verifier->verifyFsm();
		verifier->info();

		// Check passed
		if (!verifier->getVerified())
			return;

	}

	// Do generate
	//------------------

	//-- Default path is where the FSM file is
	QDir location = Core::getInstance()->getProject()->getDirectory();

	//-- FileName offer
	QString defaultName = QString::fromStdString(f->getFsmName())+"_fsm.v";

	//-- Generate
	GenerationOfVerilog* veriloggen = new GenerationOfVerilog(f, this, true,
			true, location.exists()?defaultName:"",location.exists()?location.absolutePath().toStdString().c_str():"");
//	veriloggen->setForwardStateDelayed(this->controlGenerateForwardDelayed->isChecked());
//	veriloggen->setForwardStateAsync(this->controlGenerateForwardAsync->isChecked());
//	veriloggen->setForwardState(this->controlGenerateForward->isChecked());

	veriloggen->createVerilog();
	delete veriloggen;

}

void FSMSceneView::generateVerilogReload() {

	//-- Is there a previously saved path ?

	// Get FSM
	Fsm * fsm = dynamic_cast<Scene*> (this->scene())->getFsm();

	//-- If no, generate Normally
	if (fsm != NULL && fsm->getLastGeneratedVerilogFile().size() == 0) {
		this->generateVerilog();
	} else if (fsm != NULL) {

		//-- If yes, generate Directly
		//--------------------------------

		//-- Verify
		Verify * verifier = new Verify(fsm,this);

		// Do verify
		verifier->verifyFsm();
		verifier->info();

		// Check passed
		if (!verifier->getVerified())
			return;



		//-- Generate
		GenerationOfVerilog* veriloggen = new GenerationOfVerilog(fsm, this,
				true, true, "");
//		veriloggen->setForwardStateDelayed(this->controlGenerateForwardDelayed->isChecked());
//		veriloggen->setForwardStateAsync(this->controlGenerateForwardAsync->isChecked());
//		veriloggen->setForwardState(this->controlGenerateForward->isChecked());
		veriloggen->updateVerilog();
		delete veriloggen;

		//-- Also Generate VPlan ?
//		if (this->controlGenerateVPlan->isChecked()) {
//
//			//-- Prepare a Generator
//			//VerificationPlanGenerator vplanGenerator(fsm);
//
//		}

	}

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

			//-- Yeah do rename
			this->getRelatedScene()->getFsm()->setName(newName.toStdString());

			//-- FIXME Propagate to title
		}
	}

}

