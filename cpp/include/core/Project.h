/*
 * Project.h
 *
 *  Created on: May 20, 2011
 *      Author: rleys
 */

#ifndef PROJECT_H_
#define PROJECT_H_

// Includes
//---------------

//-- Std
#include <string>
using namespace std;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
class Fsm;

/**
 * Represents an FSM designer project
 * @warning Managed by Core singleton
 */
class Project {

public:
	Project();
	virtual ~Project();

protected:

	/// The project Name
	QString name;

	/// The project file
	QFileInfo fileInfo;

	/// All the FSMs contained in the project
	QList<Fsm*> fsmList;

	/// The selected FSM to ensure compatibility with old UI
	Fsm * selectedFSM;

public:

	/**
	 * Add a new FSM to the project
	 * @return The newly created FSM
	 */
	Fsm * addFSM();

	/**
	 * Add a new FSM to the project, and changes its ID first
	 * @param The provided FSM
	 * @return The provided FSM, with changed ID
	 */
	Fsm * addFSM(Fsm *);

	/**
	 * Deletes an FSM
	 * @param id - The id of the fsm to delete
	 * @throw - An exception if the FSM does not exist
	 */
	void deleteFSM(int id);

	/**
	 * Deletes am FSM
	 * @param  - The direct pointer of the FSM to delete
	 * @throw - An exception if the FSM does not exist
	 */
	void deleteFSM(Fsm *);

	/**
	 * Get All FSMs from Project
	 *
	 * @return A Copy of the local list, to avoid user self add/remove FSMs
	 */
	QList<Fsm*> getFSMs();

	/**
	 * Finds the fsm with provided name
	 * @param name
	 * @return A pointer to found FSm, or NULL if not found
	 */
	Fsm * findFSM(string name);

	/**
	 * Finds the fsm with provided id
	 * @param id
	 * @return A pointer to found FSm, or NULL if not found
	 */
	Fsm * findFSM(int);


	QDir getDirectory() {
		if (!isFileInfo())
			return QDir();
		return this->fileInfo.dir();
	}

	void setFileInfo(QFileInfo fileInfo) {
		this->fileInfo = fileInfo;
	}

	/**
	 * Returns the file location of the project
	 */
	QFileInfo getFileInfo() {
		return this->fileInfo;
	}

	/**
	 *
	 * @return true if we have a file, false if not
	 */
	bool isFileInfo() {
		return this->fileInfo.isFile();
	}

	QString getName() {
		return this->name;
	}

	void setName(QString name) {
		this->name = name;
	}

	void setName(string name) {
		this->name = QString(name.c_str());
	}

	void setSelectedFSM(Fsm* fsm) {
			this->selectedFSM = fsm;
	}

	Fsm* getSelectedFSM() {
		return this->selectedFSM;
	}


};

#endif /* PROJECT_H_ */
