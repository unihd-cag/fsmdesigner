/*
 * FSMDesigner.h
 *
 * This fil mainly contains enums or so
 *
 *  Created on: May 20, 2011
 *      Author: rleys
 */

#ifndef FSMDESIGNER_H_
#define FSMDESIGNER_H_

namespace FSMDesigner {

// Enums and so on
//-----------------------

//-- Elements
typedef enum {
	NONE,
	CHOOSE,
	STATE,
	TRANS,
	HYPERTRANS,
	LINKDEPARTURE,
	LINKARRIVAL,
	JOIN,
	REMOVE,
	HIERARCHY,
	TRACKPOINT
} Item;


typedef enum {
	LOCKED,
	UNLOCKED
} PlaceLockMode;



}
#endif /* FSMDESIGNER_H_ */
