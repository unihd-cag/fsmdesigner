/*
 * WelcomeTab.h
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

#ifndef WELCOMETAB_H_
#define WELCOMETAB_H_

// Includes
//-------------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
class Project;

class WelcomeTab : public QWidget {

	Q_OBJECT

protected:

	// The content panel
	QWidget * contentPanel;

	/// The Last path opened, to restore File dialog
	QString  openFileLastPath;

	/// The Project info name display label
	QLabel * projectInfoName;
	QLabel * projectInfoLocation;

	QLineEdit * projectCreateName;

public:
	WelcomeTab(QWidget * parent = NULL);
	virtual ~WelcomeTab();


protected:



	/// Initialize appareance and so on
	void initialize();

	QWidget * initializeContentPanel();

	QWidget * initializeSettingsPanel();


	/// Resize elements
	virtual void resizeEvent ( QResizeEvent * event );


public Q_SLOTS:

    /// Called when the model changed
    /// Implementation should Update UI
    virtual void modelChanged();


protected Q_SLOTS:



	/// Open A recent file from recent files list index
	void openRecentFile( QListWidgetItem * item);

	/**
	 * Open the given file, or ask for one
	 * @param path if not provided or "", ask for a file to open
	 */
	void openFile(QString path = "");


	/**
	 * Creates a new project by taking name from name text line editor
	 */
	void createProject();

	/**
	 * Create a new Project
	 * @param name
	 */
	void createProject(QString name);

	/**
	 * Rename the current Project
	 * If the name is not provided, asks for one
	 */
	void renameProject(QString newName = "");

signals:

    /**
     * Signals a request to close the current project
     */
	void projectClosed();

	/**
     * Signals opening of a file
     * @param path if not provided or "", ask for a file to open
     */
    void openedFile(QString path = "");

    /**
     * Signals the creation of a project
     * @param name
     */
    void createdProject(QString name);

};

#endif /* WELCOMETAB_H_ */
