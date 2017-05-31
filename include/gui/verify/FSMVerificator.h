/*
 * FSMVerificator.h
 *
 *  Created on: Aug 22, 2011
 *      Author: rleys
 */

#ifndef FSMVERIFICATOR_H_
#define FSMVERIFICATOR_H_

// Includes
//-----------------


//--Std

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
class Scene;
class StateItem;


/**
 * This class analyses a Scene containing a FSM and tries to determine errors.
 * Error objects are then added to the concerned FSMGraphicsItem to allow nice display
 */
class FSMVerificator : public QObject,public SceneRelatedObject {

	Q_OBJECT


public:
	FSMVerificator(Scene *);
	virtual ~FSMVerificator();

public Q_SLOTS:

	/**
	 * Proceed to verification
	 */
	void verify();

	/**
	 * Reset Verification state
	 * -> Removes all Errors present in Items on the scene
	 */
	void reset();


	/**
	 * \defgroup Verifications
	 * \brief Different Verification rules
	 */
	/** @{ */

	/// Checks All state have a different output from one another
	void verifyStateUniqueOutput(QList<StateItem*> stateItems);


	/** @} */



};

#endif /* FSMVERIFICATOR_H_ */
