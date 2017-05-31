/*
 * SGC.cpp
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

#include "SGC.h"

SGC * SGC::singleton = NULL;
QMutex SGC::singletonMutex;


SGC::SGC() {
	// TODO Auto-generated constructor stub

}

SGC::~SGC() {
	// TODO Auto-generated destructor stub
}



void SGC::activate() {
	this->secureAreaMutex.lock();
}


void SGC::collect()  {


	//-- Delete everything in list
	while (!this->pendingDeletes.isEmpty()) {

		//-- FIXME Check address is not already in the deleted map

		//-- Delete
		QGraphicsItem* pDel = this->pendingDeletes.takeFirst();
		delete pDel;

		//-- FIXME Keep a trace in a deleted map

	}

	//-- Unlock at the end
	this->secureAreaMutex.unlock();
}


void SGC::requestDelete(QGraphicsItem * pDel) {

	//-- Is in secure mode?
	if (!this->secureAreaMutex.tryLock()) {

		//-- Pending
		this->pendingDeletes.append(pDel);

	} else {

		//-- Not secured
		delete pDel;
	}



}
