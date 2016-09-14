/*
 * FSMGraphicsItem.h
 *
 *  Created on: Sep 24, 2010
 *      Author: rleys
 */

#ifndef FSMGRAPHICSITEM_H_
#define FSMGRAPHICSITEM_H_



// Includes
//-----------------

//-- Std
#include <map>
using namespace std;

//-- Core
#include <core/FSMDesigner.h>

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Gui Forwards
class StateItem;
class Transline;
class TranslineText;
class TrackpointItem;
class JoinItem;
class LinkDeparture;
class LinkArrival;
class HyperTransition;

//-- Verification
#include <gui/verify/FSMVerifyError.h>

/**
 * Common Class for FSM Graphics Items
 */
template<class T=QObject>  class FSMGraphicsItem {


protected:

	/// The model as template
	T * model;

	/**
	 * All the Verification Errors currently concerning this Item
	 */
	QMap<FSMVerifyError::VerifyErrorType,FSMVerifyError*> verificationErrors;


public:

	/// base constant for User Type identification : 65537
	enum {
		FSMGraphicsItemType = QGraphicsItem::UserType + 1
	};

	/// All Types enums
	typedef enum {

		STATEITEM = FSMGraphicsItemType,
		TRANSLINE,
		TRANSLINETEXT,
		HYPERTRANS,
		TRACKPOINT,
		LINKDEPARTURE,
		LINKARRIVAL,
		LEVELTRANSFER,
		JOINITEM,
		HIERARCHYEXIT


	} FSMGraphicsItemTypes ;

	typedef enum {

		TOP,TOPRIGHT,RIGHT,BOTTOMRIGHT,BOTTOM,BOTTOMLEFT,LEFT,TOPLEFT

	} ApproachDirection;

	/// ZValues to avoid collisions


	FSMGraphicsItem() {

	}

	virtual ~FSMGraphicsItem(){

	}

	/// Validate new position in FSM core
	/// \return true if a position was recorded, false if not
	virtual bool recordPosition(){
	    return false;
	}

	/**
	 * Remove the element from FSM
	 * @return true if deleted, false if not
	 */
	virtual QList<QUndoCommand*> remove(QUndoCommand * parentComand = NULL) {
		return QList<QUndoCommand*>();
	}

	/// Getter for internal model
	T * getModel() {
		return this->model;
	}

	/// Setter for internal model
	void setModel(T * newModel) {
		void * pastModel = this->model;
		this->model = newModel;
		//if (this->model != pastModel)
			this->modelChanged();
	}


	/// Called when the model changed (thus from setModel if pointers are different)
	virtual void modelChanged() {

	}


	/**
	 * Method for common behaviour of all FSM items, like SUPPR key produces a delete event
	 * @param event
	 */
	virtual void keyRelease ( QKeyEvent * event ) {

		if (event->key()==Qt::Key_Delete)
			this->remove();
	}



	/**
	 * \defgroup Verification
	 * \brief Utility methods vor verification errors
	 *
	 */
	/** @{ */


	/**
	 * Add an Error occured during Verification
	 * Internally mapped, so adding twice the same error doesn't have any effect
	 * @warning Doesn't check there isn't already an error of this type. Pointer will be replaced in map
	 * @param error
	 */
	void addVerificationError(FSMVerifyError * error) {
		this->verificationErrors[error->getType()] = error;
		this->verificationErrorAdded(error);
	}


	/**
	 * Removes the provided error from map.
	 * It only Nullifies the index.
	 * @param error
	 */
	void removeVerificationError(FSMVerifyError * error) {

		if (this->verificationErrors.contains(error->getType())) {
			this->verificationErrors.remove(error->getType());
			this->verificationErrorRemoved(error);
		}
	}

	/**
	 * Returns an Error matching the provided type
	 * @param type
	 * @return The pointer to error, or NULL if none
	 */
	FSMVerifyError * getVerificationError(FSMVerifyError::VerifyErrorType type) {
		return this->verificationErrors[type];
	}

	/**
	 * Event Method called when a Verification Error has been added to this item
	 * @param error The added error
	 */
	virtual void verificationErrorAdded(FSMVerifyError * error) {

	}

	/**
	 * Event Method called when a Verification Error has been added to this item
	 * @param error The removed error - If null, means All errors have beem removed
	 */
	virtual void verificationErrorRemoved(FSMVerifyError * error = NULL) {

	}

	/**
	 * Removes all the Verification Errors from this object,
	 * and deletes them.
	 * Destructor in FSMVerifyError propagates to other objects
	 * Only clears the map
	 *
	 *
	 */
	void verificationReset() {

		//-- Save values
		QList<FSMVerifyError*> errors = this->verificationErrors.values();



		//-- Delete Errors
		for (QList<FSMVerifyError*>::iterator it = errors.begin(); it!=errors.end();it++) {
			(*it)->remove();
			//delete (*it);
		}

		//-- Clear map
		this->verificationErrors.clear();

	}

	/** @} */


	/**\defgroup Utility methods to know about the various available types */
	/** @{ */

	static FSMGraphicsItem * toFSMItem(QGraphicsItem * item) {
		return dynamic_cast<FSMGraphicsItem*>(item);
	}

	static bool isStateItem(QGraphicsItem * item) {
			return item!=NULL && (item->type()==FSMGraphicsItem<>::STATEITEM);
	}

	bool isStateItem() {
		return this->isStateItem(dynamic_cast<QGraphicsItem*>(this));
	}

	static StateItem * toStateItem(QGraphicsItem * item) {
		return dynamic_cast<StateItem*>(item);
	}

	StateItem * toStateItem() {
			return this->toStateItem(dynamic_cast<QGraphicsItem*>(this));
	}

	static bool isTrackPoint(QGraphicsItem * item) {
		return item!=NULL && (item->type()==FSMGraphicsItem<>::TRACKPOINT);
	}

	bool isTrackPoint() {
		return this->isTrackPoint(dynamic_cast<QGraphicsItem*>(this));
	}

	static TrackpointItem * toTrackPoint(QGraphicsItem * item) {
		return dynamic_cast<TrackpointItem*>(item);
	}

	TrackpointItem * toTrackPoint() {
		return this->toTrackPoint(dynamic_cast<QGraphicsItem*>(this));
	}

	static bool isHyperTransition(QGraphicsItem * item) {
        return item!=NULL && (item->type()==FSMGraphicsItem<>::HYPERTRANS);
    }

    bool isHyperTransition() {
        return this->isHyperTransition(dynamic_cast<QGraphicsItem*>(this));
    }

    static HyperTransition * toHyperTransition(QGraphicsItem * item) {
        return dynamic_cast<HyperTransition*>(item);
    }

    HyperTransition * toHyperTransition() {
        return this->toHyperTransition(dynamic_cast<QGraphicsItem*>(this));
    }

	static bool isTransline(QGraphicsItem * item) {
		return item!=NULL && (item->type()==FSMGraphicsItem<>::TRANSLINE);
	}

	bool isTransline() {
		return this->isTransline(dynamic_cast<QGraphicsItem*>(this));
	}

	static Transline * toTransline(QGraphicsItem * item) {
		return dynamic_cast<Transline*>(item);
	}

	Transline * toTransline() {
		return this->toTransline(dynamic_cast<QGraphicsItem*>(this));
	}

	static bool isTranslineText(QGraphicsItem * item) {
        return item!=NULL && (item->type()==FSMGraphicsItem<>::TRANSLINETEXT);
    }

    bool isTranslineText() {
        return this->isTranslineText(dynamic_cast<QGraphicsItem*>(this));
    }

    static TranslineText * toTranslineText(QGraphicsItem * item) {
        return dynamic_cast<TranslineText*>(item);
    }

    TranslineText * toTranslineText() {
        return this->toTranslineText(dynamic_cast<QGraphicsItem*>(this));
    }

	static bool isJoinItem(QGraphicsItem * item) {
		return item!=NULL && (item->type()==FSMGraphicsItem<>::JOINITEM);
	}

	static JoinItem * toJoinItem(QGraphicsItem * item) {
		return dynamic_cast<JoinItem*>(item);
	}


	static bool isLinkDeparture(QGraphicsItem * item) {
		return item!=NULL && (item->type()==FSMGraphicsItem<>::LINKDEPARTURE);
	}

	static LinkDeparture * toLinkDeparture(QGraphicsItem * item) {
		return dynamic_cast<LinkDeparture*>(item);
	}



	/**@}*/





};

#endif /* FSMGRAPHICSITEM_H_ */
