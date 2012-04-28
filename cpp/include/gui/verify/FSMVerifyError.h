/*
 * FSMVerifyError.h
 *
 *  Created on: Aug 22, 2011
 *      Author: rleys
 */

#ifndef FSMVERIFYERROR_H_
#define FSMVERIFYERROR_H_

// Includes
//-----------------------

//-- Std

//-- Qt
#include <QtGui>
#include <QtCore>


/**
 * This class represents a verification error, like for example two states having the same output
 */
class FSMVerifyError {

public:
	typedef enum VerifyErrorType {

		STATE_UNIQUE_OUTPUT

	}VerifyErrorType;

protected:

	/// The type of the error
	VerifyErrorType type;

	/// List of the Items that are concerned
	QList<QGraphicsItem* > concernedItems;

	QString message;


public:
	FSMVerifyError(VerifyErrorType);
	virtual ~FSMVerifyError();

	/// Removes itself from all concerned items
	void remove();

	/**
	 * Add an pointer to an item concerned by this error.
	 * If pointer is already  contained, don't add
	 * @param item Pointer to the item to be added
	 * @return true if added, false if not (if already registered for example)
	 */
	bool addConcernedItem(QGraphicsItem * item);

	QList<QGraphicsItem*> getConcernedItems() {
		return this->concernedItems;
	}

	/// Returns the type of the error (i.e Error code)
	VerifyErrorType getType() {
		return this->type;
	}

	void setMessage(QString message = "") {
		this->message = message;
	}

	QString& getMessage() {
		return this->message;
	}


};

#endif /* FSMVERIFYERROR_H_ */
