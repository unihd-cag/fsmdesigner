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
#include <gui/common/RoundedPanel.h>
#include <gui/common/propertywidget.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>
#include <gui/scene/selectionanimator.h>

#include <gui/output/outputwidget.h>

#include <gui/input/conditionwidget.h>

#include <gui/settings/GuiSettings.h>


//-- Gui: Items

#include "TabbedGUISceneView.h"
TabbedGUISceneView::TabbedGUISceneView(Scene* scene, QWidget* parent) :
	QGraphicsView(scene, parent) {

	//-- Prepare variables
	this->initVariables();
	this->setRelatedScene(scene);

	//-- Init Control Panel && Items Panel
	this->initControlPanel();
	this->initializeItemsPanel();
	this->initializeLayoutPanel();
	this->initializeInfoWidgets();
	this->setContentsMargins(10, 10, 10, 10);
	this->viewport()->setContentsMargins(0, 0, 0, 0);

	this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);


	//-- background
	// bgGrid = new QPixmap(":/icons/grid1.png",0,0);


	//-- Initialise Scene per default to see
	scene->initializeScene();

	//-- Update UI
	this->fit();



}

TabbedGUISceneView::~TabbedGUISceneView() {
	//delete this->scene();
}

void TabbedGUISceneView::initVariables() {

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



}

void TabbedGUISceneView::initControlPanel() {

	// Prepare control Panel
	//---------------------------

	//-- Create Panel Widget this->viewport()
	this->controlPanel = new QWidget();
//	this->controlPanel->setMinimumSize(200,400);
	//this->controlPanel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);

	//-- Create Scoll area
	this->controlScrollArea = new QScrollArea(this->viewport());
	this->controlScrollArea->setWidget(this->controlPanel);
	this->controlScrollArea->setWidgetResizable(true);
	this->controlScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->controlScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->controlScrollArea->move(10, 10);

	this->controlScrollArea->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);

	//-- Create VBox
	QVBoxLayout * vbox = new QVBoxLayout(this->controlPanel);


	// Add FSM Name as title
	//----------------------------

	//-- FSM Title
	QLabel * fsmNameLabel = new QLabel(QString()+"<h1>"+this->getRelatedScene()->getFsm()->getFsmName().c_str()+"</h1>");
	vbox->addWidget(fsmNameLabel);

	//-- Project Name
	QLabel * projectNameLabel = new QLabel(QString()+"<i><h4>"+this->getRelatedScene()->getFsm()->getProject()->getName()+"</h4></i>");
	QFont projectNameFont = projectNameLabel->font();
	projectNameFont.setItalic(true);
	vbox->addWidget(projectNameLabel);

	// Add Small Menu Panel
	//------------------------------

	//-- Label
	QLabel * manageLabel = new QLabel("<h3>Manage FSM</h3>");
	vbox->addWidget(manageLabel);

	//-- Panel
	QWidget * smallMenuWidget = new QWidget();
	smallMenuWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
	QGridLayout * smallMenuLayout = new QGridLayout();
	smallMenuLayout->setHorizontalSpacing(5);
	smallMenuWidget->setLayout(smallMenuLayout);
	vbox->addWidget(smallMenuWidget);

	//------ Add Save
	QPushButton * saveButton = new QPushButton(QIcon(QPixmap(
			":/icons/Save")), "");
	saveButton->setIconSize(QSize(24, 24));
	saveButton->setFlat(true);
	saveButton->setToolTip("Save FSM");
	smallMenuLayout->addWidget(saveButton, 0, 0);

	this->connect(saveButton, SIGNAL(clicked()), SLOT(saveProject()));

	//------ Add Save as
	QPushButton * saveAsButton = new QPushButton(QIcon(QPixmap(
				":/icons/SaveAs")), "");
	saveAsButton->setIconSize(QSize(24, 24));
	saveAsButton->setFlat(true);
	saveAsButton->setToolTip("Save FSM As...");
	smallMenuLayout->addWidget(saveAsButton, 0, 1);

	this->connect(saveAsButton, SIGNAL(clicked()), SLOT(saveProjectAs()));

	//------ Add Rename
	QPushButton * renameButton = new QPushButton(QIcon(QPixmap(
				":/icons/Rename")), "");
	renameButton->setIconSize(QSize(24, 24));
	renameButton->setFlat(true);
	renameButton->setToolTip("Rename FSM");
	smallMenuLayout->addWidget(renameButton, 0, 2);

	this->connect(renameButton, SIGNAL(clicked()), SLOT(renameFsm()));



	// Add Verification controls
	//----------------------------------

	//-- Label
	QLabel * verifyLabel = new QLabel("<h3>Verification</h3>");
	vbox->addWidget(verifyLabel);

	//-- Panel
	QWidget * verificationPanel = new QWidget();
	verificationPanel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
	QGridLayout * verificationPanelLayout = new QGridLayout();
	verificationPanel->setLayout(verificationPanelLayout);
	vbox->addWidget(verificationPanel);


	//------ Verify
	QPushButton * verifyButton = new QPushButton(QIcon(QPixmap(
					":/icons/verification.png")), "");
	verifyButton->setIconSize(QSize(24, 24));
	verifyButton->setFlat(true);
	verifyButton->setToolTip("Verify");
	verificationPanelLayout->addWidget(verifyButton, 0, 0, Qt::AlignLeft);

	this->connect(verifyButton, SIGNAL(clicked()),this->scene(), SLOT(verify()));

	//------ Reset
	QPushButton * verifyResetButton = new QPushButton(QIcon(QPixmap(
						":/icons/Clear-brush")), "");
	verifyResetButton->setIconSize(QSize(24, 24));
	verifyResetButton->setFlat(true);
	verifyResetButton->setToolTip("Reset Verification");
	verificationPanelLayout->addWidget(verifyResetButton, 0, 1, Qt::AlignLeft);

	dynamic_cast<Scene*>(this->scene())->getFSMVerificator()->connect(verifyResetButton, SIGNAL(clicked()),SLOT(reset()));


	// Add Verilog generation controls
	//--------------------------

	//-- Add Label
	QLabel * genVerilogLabel = new QLabel("<h3>Verilog Generation<h3>");
	genVerilogLabel->setTextFormat(Qt::RichText);
	vbox->addWidget(genVerilogLabel);

	//-- Add A Panel Widget
	QWidget * generationPanel = new QWidget();
	generationPanel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
	QGridLayout * generationPanelLayout = new QGridLayout();
	generationPanel->setLayout(generationPanelLayout);
	vbox->addWidget(generationPanel);

	//------ Add VPlan checkbox
	this->controlGenerateVPlan = new QCheckBox("Generate VPlan");
	this->controlGenerateVPlan->setToolTip(
			"Generate a Verification Plan file along with Verilog for this FSM");
	generationPanelLayout->addWidget(this->controlGenerateVPlan, 0, 0,Qt::AlignLeft);

	//------ Add NC SIm MMap box
	this->controlGenerateVerilogMMap = new QCheckBox("Generate MMap");
	this->controlGenerateVerilogMMap->setToolTip(
			"Generate an NCSim Map in a .f file to see State names in the SimVison Waveform");
	generationPanelLayout->addWidget(this->controlGenerateVerilogMMap, 0, 1,Qt::AlignLeft);


	//------ Add Std Forward
	this->controlGenerateForward = new QCheckBox("Forward State");
	this->controlGenerateForward->setToolTip(
			"An Extra output gives the current state output, avoiding to have to rebuild the bus yourself for usage");
	generationPanelLayout->addWidget(this->controlGenerateForward, 1, 0,2,1,Qt::AlignLeft);


	//------ Add Forward delayed state option
	this->controlGenerateForwardDelayed = new QCheckBox("Forward Delayed State");
	this->controlGenerateForwardDelayed->setToolTip(
			"An Extra output gives the state delayed for on clock cycle, to allow synchronising the datapath");
	generationPanelLayout->addWidget(this->controlGenerateForwardDelayed, 1, 1,Qt::AlignLeft);

	//------ Add Forward async state option
	this->controlGenerateForwardAsync = new QCheckBox("Forward Async State");
	this->controlGenerateForwardAsync->setToolTip(
			"An extra output gives the state asynchronously outputed from internal case, to allow synchronising the datapath");
	generationPanelLayout->addWidget(this->controlGenerateForwardAsync, 2, 1,Qt::AlignLeft);



	//------ Add reload button
	this->controlRegenerateVerilogButton = new QPushButton("Generate Verilog");
	this->controlRegenerateVerilogButton->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);
	generationPanelLayout->addWidget(this->controlRegenerateVerilogButton,3, 0,1,2,Qt::AlignLeft);

	this->connect(this->controlRegenerateVerilogButton, SIGNAL(clicked()),
			SLOT(generateVerilogReload()));

	// Add Properties Editor
	//--------------------------------
	this->controlPropertyWidget = new PropertyWidget(this->controlPanel);
	this->controlPropertyWidget->setRelatedScene(
			dynamic_cast<Scene*> (this->scene()));
	this->controlPropertyWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
	//this->controlPropertyWidget->connect(this->scene(),SIGNAL(selectionChanged()),SLOT(sceneSelectionChanged()));
	vbox->addWidget(this->controlPropertyWidget);

	// Add Output Editor
	//-----------------------
	OutputWidget * outputWidget = new OutputWidget();
	outputWidget->setRelatedScene(this->getRelatedScene());
	outputWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
	vbox->addWidget(outputWidget);

	// Add Input Editor
	//-----------------------
	ConditionWidget * conditionWidget = new ConditionWidget();
	conditionWidget->setRelatedScene(this->getRelatedScene());
	conditionWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
	vbox->addWidget(conditionWidget);

	// Add Undo View
	//---------------------
	this->controlUndoView = new QUndoView(
			dynamic_cast<Scene*> (this->scene())->getUndoStack());
	this->controlUndoView->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
	vbox->addWidget(this->controlUndoView);

	// Dependend Signals connections
	//------------------

	//-- Selection on scene has changed
	this->connect(this->scene(), SIGNAL(selectionChanged()),
			SLOT(sceneSelectionChanged()));

	// Update control Panel to finish
	//---------------------------------------
	this->updateControlPanel();
}

void TabbedGUISceneView::updateControlPanel() {

	Scene* scene = dynamic_cast<Scene*> (this->scene());

	// Disable if there is no FSM
	//------------------------------------
	//this->controlPanel->setDisabled(!Core::getInstance()->hasFsm());

	//-- Update Reload tooltip
	if (scene->getFsm() == NULL
			|| scene->getFsm()->getLastGeneratedVerilogFile().size() == 0) {
		this->controlRegenerateVerilogButton->setToolTip(
				"Reload the Verilog if already generated once");
	} else {
		stringstream str;
		str << "Reload the Verilog to: "
				<< scene->getFsm()->getLastGeneratedVerilogFile();
		this->controlRegenerateVerilogButton->setToolTip(str.str().c_str());
	}

}

void TabbedGUISceneView::initializeItemsPanel() {

	Scene * scene = this->getRelatedScene();

	//-- Rounded panel
	this->itemsPanel = new RoundedPanel(this->viewport());
	this->itemsPanel->move(10, 10);
	this->itemsPanel->setPalette(QPalette(Qt::yellow));
	this->itemsPanel->setAutoFillBackground(true);

	QVBoxLayout * itemsPanelLayout = new QVBoxLayout();
	this->itemsPanel->setLayout(itemsPanelLayout);

	//-- Add Title
	QLabel * title = new QLabel("<h2>Items</h2>");
	itemsPanelLayout->addWidget(title);

	//-- Add Button Icons for the Various Items
	//-------------------------------------------------


	//-- State
	QPushButton * stateButton = new QPushButton();
	QPixmap stateIcon(":/icons/state.png");
	stateButton->setIcon(QIcon(stateIcon));
	stateButton->setIconSize(stateIcon.size());
	stateButton->setToolTip("Add a state");
	itemsPanelLayout->addWidget(stateButton, 0, Qt::AlignRight);

	QObject::connect(stateButton, SIGNAL(clicked()), scene,
			SLOT(startPlaceState()));

	//-- Transition
	QPushButton * transitionButton = new QPushButton();
	QPixmap transIcon(":/icons/trans.png");
	transitionButton->setIcon(QIcon(transIcon));
	transitionButton->setIconSize(transIcon.size());
	transitionButton->setToolTip("Add A transition");
	itemsPanelLayout->addWidget(transitionButton, 0, Qt::AlignRight);

	QObject::connect(transitionButton, SIGNAL(clicked()), scene,
			SLOT(startPlaceTransition()));

	//-- Link To
	QPushButton * linkButton = new QPushButton();
	QPixmap linkIcon(":/icons/link.png");
	linkButton->setIcon(QIcon(linkIcon));
	linkButton->setIconSize(linkIcon.size());
	linkButton->setToolTip("Add a link to a state");
	itemsPanelLayout->addWidget(linkButton, 0, Qt::AlignRight);

	QObject::connect(linkButton, SIGNAL(clicked()), scene,
			SLOT(startPlaceLink()));

	//-- Join
	QPushButton * joinButton = new QPushButton();
	QPixmap joinIcon(":/icons/join.png");
	joinButton->setIcon(QIcon(joinIcon));
	joinButton->setIconSize(joinIcon.size());
	joinButton->setToolTip("Add a Join point to the FSM");
	itemsPanelLayout->addWidget(joinButton, 0, Qt::AlignRight);

	QObject::connect(joinButton, SIGNAL(clicked()), scene,
			SLOT(startPlaceJoin()));

}

/**
 * Init panel offering Layout Options
 */
void TabbedGUISceneView::initializeLayoutPanel() {

	Scene * scene = this->getRelatedScene();

	//-- Rounded panel
	this->layoutPanel = new RoundedPanel(this->viewport());
	this->layoutPanel->move(10, 10);
	this->layoutPanel->setPalette(QPalette(Qt::yellow));
	this->layoutPanel->setAutoFillBackground(true);

	QVBoxLayout * layoutPanelLayout = new QVBoxLayout();
	this->layoutPanel->setLayout(layoutPanelLayout);

	//-- Add Title
	QLabel * title = new QLabel("<h2>Layout</h2>");
	layoutPanelLayout->addWidget(title);

	// Add Layouting Buttons
	//------------------------------

	//-- Align vertical
	QPushButton * verticalAlignButton = new QPushButton("Align V.");
	verticalAlignButton->setToolTip(
			"Aligns on a same vertical axis all the selected states");
	layoutPanelLayout->addWidget(verticalAlignButton);

	QObject::connect(verticalAlignButton, SIGNAL(clicked()), scene,
			SLOT(alignSelectionVertical()));

	//-- Align Horizontal
	QPushButton * horizontalAlignButton = new QPushButton("Align H.");
	horizontalAlignButton->setToolTip(
			"Aligns on a same Horizontal axis all the selected states");
	layoutPanelLayout->addWidget(horizontalAlignButton);

	QObject::connect(horizontalAlignButton, SIGNAL(clicked()), scene,
			SLOT(alignSelectionHorizontal()));

}

void TabbedGUISceneView::initializeInfoWidgets() {

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

void TabbedGUISceneView::drawForeground(QPainter * painter, const QRectF & rect) {



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

void TabbedGUISceneView::mouseMoveEvent(QMouseEvent* e) {

	// Parent job
	QGraphicsView::mouseMoveEvent(e);

	// Cursor Display on place mode
	//-------------
	if (((Scene*) this->scene())->getPlaceMode() != FSMDesigner::CHOOSE) {

		// Depending on choose mode, use cursos to help on certain rules

	}

	// Hide Panels if mouse is pressed and we are over the left panels
	//---------------------------------------
	if ((e->buttons() & Qt::LeftButton) && this->controlPanel->rect().contains(e->pos(),true)) {

		if (this->controlPanel->isVisible())
			this->togglePanelsVisibility();

	} else if ((e->buttons() & Qt::LeftButton)) {

		//-- Redisplay
		if (!this->controlPanel->isVisible())
			this->togglePanelsVisibility();

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

void TabbedGUISceneView::mouseReleaseEvent(QMouseEvent* e) {

	//-- Parent job
	QGraphicsView::mouseReleaseEvent(e);

	//-- Deactivate areaSelection
	if (this->areaSelectionInitial != NULL) {
		delete this->areaSelectionInitial;
		this->scene()->update();
	}
	this->areaSelectionInitial = NULL;

	// If we release mouse in Control panel region, and it is not visible, reshow it
	//----------------
	if (this->controlPanel->rect().contains(e->pos(),true) && (!this->controlPanel->isVisible())) {
		this->togglePanelsVisibility();
	}


	//this->scene()->update();
}

void TabbedGUISceneView::mousePressEvent(QMouseEvent* e) {

	// Must call parent
	QGraphicsView::mousePressEvent(e);

	//make sure the view window gets the focus, so property changes etc are saved
	//setFocus(Qt::MouseFocusReason);


	if (e->button() == Qt::MidButton) {
		resetTransform();
		ensureVisible(QRectF(0, 0, 0, 0)); //Scrolls so that point (0,0) is visible
	}

	//-- With left button, check we are not in a special object add mode
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
void TabbedGUISceneView::wheelEvent(QWheelEvent *event) {



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

		//-- Reposition View UI
		this->replacePanels();

	}
}

void TabbedGUISceneView::sceneSelectionChanged() {

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


void TabbedGUISceneView::resizeEvent(QResizeEvent * event) {

	// Draw a blue background
	//----------
	//QLinearGradient gradient(this->visibleRegion().boundingRect().this->parentWidget()th(),this->visibleRegion().boundingRect().height(),0,0);
	//QLinearGradient gradient(this->scene()->itemsBoundingRect().this->parentWidget()th(),this->scene()->itemsBoundingRect().height(),0,0);
	/*QLinearGradient gradient(this->this->parentWidget()th(),this->height(),0,0);

	 gradient.setColorAt(0,Qt::lightGray);
	 gradient.setColorAt(1,Qt::white);
	 QBrush * bgb = new QBrush(gradient);

	 this->setBackgroundBrush(*bgb);*/



	QGraphicsView::resizeEvent(event);


	//-- Resize scroll panels

	//-- Replace Panels
	this->replacePanels();



}

void TabbedGUISceneView::showEvent ( QShowEvent * event ) {

	//qDebug() << "Show scene view";

	//-- Parent call
	QGraphicsView::showEvent(event);


	//-- Replace panels
	this->replacePanels();

	//qDebug() << "Done showEvent the scene view";

}

void TabbedGUISceneView::scrollContentsBy(int dx, int dy) {

	//-- Parent
	QGraphicsView::scrollContentsBy(dx, dy);

	//-- Replace Panels
	this->replacePanels();
}

void TabbedGUISceneView::replacePanels() {

	//-- Top left
	this->controlScrollArea->move(this->rect().topLeft());
	this->controlScrollArea->move(10,10);
	//this->controlScrollArea->setBaseSize(this->rect().width(),this->rect().height());
	//this->controlScrollArea->setMaximumSize(this->rect().width(),this->rect().height());
	//this->controlScrollArea->setFixedSize(400,this->rect().height()-20);
	this->controlScrollArea->setMaximumSize(400,this->rect().height()-20);
	this->controlScrollArea->setMinimumSize(0,this->rect().height()-20);
	//this->controlScrollArea->ensureWidgetVisible(this->controlPanel);

	//-- Top right
	this->itemsPanel->move(this->rect().topRight().x()
			- this->itemsPanel->width()-20, this->rect().topRight().y());

	//-- Under Items on the right
	this->layoutPanel->move(this->itemsPanel->x(),
			this->itemsPanel->rect().bottomLeft().y() + 10);


	// Info Widget
	//-------------------

	//-- Lock bottom right
	this->lockModeicon->move(this->rect().bottomRight().x()
			- this->lockModeicon->width(),
			this->rect().bottomRight().y() - this->lockModeicon->height());


}

void TabbedGUISceneView::hidePanels() {

	this->controlPanel->setVisible(false);
	this->itemsPanel->setVisible(false);
	this->layoutPanel->setVisible(false);
}

void TabbedGUISceneView::showPanels() {

	this->controlPanel->setVisible(true);
	this->itemsPanel->setVisible(true);
	this->layoutPanel->setVisible(true);

}

void TabbedGUISceneView::togglePanelsVisibility() {

	this->controlPanel->setVisible(!this->controlPanel->isVisible());
	this->itemsPanel->setVisible(!this->itemsPanel->isVisible());
	this->layoutPanel->setVisible(!this->layoutPanel->isVisible());

}

void TabbedGUISceneView::keyPressEvent(QKeyEvent* ke) {


	//-- Parent processing
	QGraphicsView::keyPressEvent(ke);


	//-- Pressing Ctrl Locks placement
	if (ke->key() ==  Qt::Key_Control && !ke->isAccepted()) {

		//-- Place mode
		this->getRelatedScene()->setPlaceLocked();

		//-- Show Lock
		this->lockModeicon->setVisible(true);
		this->replacePanels();
	}




}

void TabbedGUISceneView::keyReleaseEvent(QKeyEvent* ke) {

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
	// F10 = Hide All control Panels using opacity
	//---------------------------
	else if (ke->key() == Qt::Key_F9) {


		this->togglePanelsVisibility();

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
		this->replacePanels();

	}



}

void TabbedGUISceneView::fit() {

	//-- Get Items bouding rect
	QRectF itemsBoundingRect = this->scene()->itemsBoundingRect();
	QRectF fitRectScene = this->scene()->sceneRect();

	//-- Center on it
	if (itemsBoundingRect.isValid())
		this->fitInView(itemsBoundingRect, Qt::KeepAspectRatio);


	//-- Replace UI
	this->replacePanels();

}


void TabbedGUISceneView::placeSetUnlocked() {
	((Scene*) this->scene())->setPlaceUnlocked();
}

void TabbedGUISceneView::placeSetLocked() {
	((Scene*) this->scene())->setPlaceLocked();
}

void TabbedGUISceneView::placeSetMode(FSMDesigner::Item placeMode) {
	((Scene*) this->scene())->setPlaceMode(placeMode);
}


void TabbedGUISceneView::generateVerilog() {

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
			true, defaultName,location.exists()?location.absolutePath().toStdString().c_str():"");
	veriloggen->setForwardStateDelayed(this->controlGenerateForwardDelayed->isChecked());
	veriloggen->setForwardStateAsync(this->controlGenerateForwardAsync->isChecked());
	veriloggen->setForwardState(this->controlGenerateForward->isChecked());

	veriloggen->createVerilog();
	delete veriloggen;

}

void TabbedGUISceneView::generateVerilogReload() {

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
		veriloggen->setForwardStateDelayed(this->controlGenerateForwardDelayed->isChecked());
		veriloggen->setForwardStateAsync(this->controlGenerateForwardAsync->isChecked());
		veriloggen->setForwardState(this->controlGenerateForward->isChecked());
		veriloggen->updateVerilog();
		delete veriloggen;

		//-- Also Generate VPlan ?
		if (this->controlGenerateVPlan->isChecked()) {

			//-- Prepare a Generator
			//VerificationPlanGenerator vplanGenerator(fsm);

		}

	}

}

void TabbedGUISceneView::saveProjectAs() {

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

void TabbedGUISceneView::saveProject() {

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

void TabbedGUISceneView::renameFsm() {

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

