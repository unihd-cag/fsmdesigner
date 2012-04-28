/*
 * DOMSave.h
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */

#ifndef QXMLSAVE_H_
#define QXMLSAVE_H_

// Includes
//------------

//-- Std
#include <fstream>
#include <iostream>
using namespace std;

//-- Core
#include <core/Save_I.h>
class Fsm;

class QXMLSave : public Save_I {
public:
	QXMLSave();
	virtual ~QXMLSave();


	virtual void createDTD(string dirname);
	virtual void createProjectDTD(string dirname);
	virtual void save(string filename, string fsmname);
	virtual void saveProject(string filename);

	/**
	 * Write an FSM out to an output stream
	 * @param fsm
	 * @param output
	 */
	void writeFSM(Fsm * fsm,ofstream& output);

};

#endif /* DOMSAVE_H_ */
