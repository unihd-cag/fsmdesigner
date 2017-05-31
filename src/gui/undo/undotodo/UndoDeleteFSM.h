/*
 * UndoDeleteFSM.h
 *
 *  Created on: Jul 29, 2011
 *      Author: rleys
 */

#ifndef UNDODELETEFSM_H_
#define UNDODELETEFSM_H_

// Includes
//-------------

//-- Core
class Project;
class Fsm;

//-- Qt
#include <QtGui>
#include <QtCore>


class UndoDeleteFSM : public QUndoCommand {

protected:

	Project * project; ///< Project to which the FSM belongs
	Fsm 	* fsmToDelete; ///< FSM to be deleted


public:
	UndoDeleteFSM(Project * project,Fsm * fsmToDelete,QUndoCommand * parent = NULL );
	virtual ~UndoDeleteFSM();


	virtual int		id();
	virtual bool	mergeWith(const QUndoCommand * command);
	virtual void	redo();
	virtual void	undo();

};

#endif /* UNDODELETEFSM_H_ */
