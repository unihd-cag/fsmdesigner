/*
 * TabbedGUIWindow.h
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

#ifndef TabbedGUIWindow_H_
#define TabbedGUIWindow_H_

// Includes
//-------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
class Project;

class TabbedGUIWindow: public QMainWindow {

	Q_OBJECT

public:
	TabbedGUIWindow();
	virtual ~TabbedGUIWindow();


protected:

	/// Initialize Window appareance and so on
	void initialize();

	/// Save Geometry state
	virtual void resizeEvent ( QResizeEvent * event );

	/**
	 * F11: Toggle fullscreen
	 * @param event
	 */
	virtual void keyReleaseEvent ( QKeyEvent * event );

	/**
	 * Reimplements Drop event to fast open Project files
	 * @param event
	 */
	virtual void 	dropEvent(QDropEvent * event);

	virtual void 	dragEnterEvent(QDragEnterEvent * event) ;


public slots:

	/// Opens the provided project through Core interface
	void openProject();

	/// Closes the Project
	void closeProject();

	/// If the UI settings have changed
	void settingsChanged();

};

#endif /* TabbedGUIWindow_H_ */
