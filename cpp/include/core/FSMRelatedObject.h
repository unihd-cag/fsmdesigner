/*
 * FSMRelatedObject.h
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

#ifndef FSMRELATEDOBJECT_H_
#define FSMRELATEDOBJECT_H_

// Includes
//---------------------

//-- Core
class Fsm;

class FSMRelatedObject {

protected:

    /// The scene to which the object is related
    Fsm * relatedFsm;


public:
	FSMRelatedObject(Fsm * fsm = NULL);
	virtual ~FSMRelatedObject();

	virtual void setRelatedFsm(Fsm * scene) ;

	Fsm * getRelatedFsm() {
		return this->relatedFsm;
	}



};

#endif /* FSMRELATEDOBJECT_H_ */
