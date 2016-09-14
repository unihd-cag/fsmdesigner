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

#ifndef TabbedGUISceneView_H_
#define TabbedGUISceneView_H_

// Includes
//------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>

//-- GUI
class SelectionAnimator;
class Scene;
class Transline;
class PropertyWidget;

//-- Gui Common
#include <gui/scene/SceneRelatedObject.h>
class RoundedPanel;

class TabbedGUISceneView: public QGraphicsView, public SceneRelatedObject {

Q_OBJECT

public:
	TabbedGUISceneView(Scene* scene, QWidget* parent = NULL);
	~TabbedGUISceneView();

private:

	///\defgroup Selection area variables
	/// @{

	QPen * selectionAreaPen;

	/// @}

	///\defgroup Control Panel Widgets
	/// @{

	QScrollArea *		controlScrollArea;
	QWidget * 			controlPanel;


	QPushButton * 		controlRegenerateVerilogButton;
	QCheckBox * 		controlGenerateVPlan;
	QCheckBox * 		controlGenerateVerilogMMap;
	QCheckBox * 		controlGenerateForwardAsync;
	QCheckBox * 		controlGenerateForwardDelayed;
	QCheckBox * 		controlGenerateForward;

	PropertyWidget * 	controlPropertyWidget;

	QUndoView * 		controlUndoView;

	///@}

	///\defgroup Item Panel Widgets
	/// @{

	RoundedPanel * 		itemsPanel;

	///@}

	///\defgroup Layout Panel Widgets
	/// @{

	RoundedPanel * 		layoutPanel;

	///@}

	///\defgroup Info Widgets
	/// @{

	QWidget * 			lockModeicon;

	///@}



	Qt::ItemSelectionMode mode;

	QAction* Straight;
	QAction* setDefault;
	QAction* change_Start;
	QAction* change_Target;

	QAction* insertTrackpointbefore;
	QAction* insertTrackpointafter;
	QAction* connecttoLink;
	QAction* changetoHierarchy;
	QAction* createJoin;
	QAction* insertTrackpoint;
	QAction* connecttoHierarchy;
	QAction* actionresetposition;

	QAction* transformToTrackpoint;

	// Animator on selection
	SelectionAnimator * selectionAnimator;

	// Selection area control
	QPointF * areaSelectionInitial;

protected:
	virtual void wheelEvent(QWheelEvent *event); //Zoom in and out with the  mouse Wheel
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void keyPressEvent(QKeyEvent* ke);
	virtual void keyReleaseEvent(QKeyEvent* ke);
	virtual void drawForeground(QPainter * painter, const QRectF & rect);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void showEvent ( QShowEvent * event );

	/// Scroll Event to update control panel and other overlayed panels
	virtual void scrollContentsBy(int dx, int dy);

private:

	void initVariables();

	/**
	 * Init Control panel with extra control buttons
	 */
	void initControlPanel();

	/**
	 * Update the state of control Panel elements
	 */
	void updateControlPanel();

	/**
	 * Init panel offering FSM Items
	 */
	void initializeItemsPanel();

	/**
	 * Init panel offering Layout Options
	 */
	void initializeLayoutPanel();

	/**
	 * Init info widgets
	 */
	void initializeInfoWidgets();

	/**
	 * Replaces all the control/items/... panels corretly
	 */
	void replacePanels();

	/**
	 * Hide all control/items/... panels
	 */
	void hidePanels();

	/**
	 * Show all control/items/... panels
	 */
	void showPanels();

	/**
	 * Show if hidden, and viceversa
	 */
	void togglePanelsVisibility();

public slots:

	/// Fit the scene
	void fit();


	/// Generate verilog for the currently displayed FSM
	void generateVerilog();

	/// Reload/Generate verilog for the currently displayed FSM
	void generateVerilogReload();

	/** \defgroup Place mode methods */
	/**@{*/

	void placeSetUnlocked();
	void placeSetLocked();

	void placeSetMode(FSMDesigner::Item w);
	/**@}*/

	/// Slot for when the selection on the scene has changed
	void sceneSelectionChanged();

	/**
	 * Perform a save
	 * This saves the project because single FSM save is not possible
	 */
	void saveProject();

	/**
	 * Performs a save to a specified location
	 * This saves the project because single FSM save is not possible
	 */
	void saveProjectAs();

	/**
	 * Opens A control dialog to rename the FSM
	 */
	void renameFsm();


};

#endif
