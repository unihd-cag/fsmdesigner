/*
 * TranslineText.h
 *
 *  Created on: Oct 21, 2010
 *      Author: rleys
 */

#ifndef TRANSLINETEXT_H_
#define TRANSLINETEXT_H_

// Includes
//--------------

//-- Std
#include <iostream>
using namespace std;

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
#include <core/State.h>
#include <core/Trans.h>
#include <core/Trackpoint.h>

//-- Gui
#include <gui/items/FSMGraphicsTextItem.h>
class Transline;

class TranslineText : public FSMGraphicsTextItem {

    protected:

        QString name;

        /// Store start time of editing to filter unselection events based on time
        QDateTime * editorStartTime;

        /// The Transition model, for model changing convienience
        Trans * transition;

    public:

    enum { Type = FSMGraphicsItem<>::TRANSLINETEXT };


    /**
     *
     * @param qs
     * @param transition
     */
    TranslineText(QString qs, Trans * transition);

    /**
     *
     */
    virtual ~TranslineText();

    /**
     * Returns the transition for which we are representing the name
     * @return
     */
    Trans * getTransition();

    /// overriden
	virtual QRectF boundingRect () ;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type;}


    bool recordPosition();

    /**
     * Updates text content on transition model change
     */
    void modelChanged();

protected:

	/**
	 * Overriden to detect selection changes and thus painting parameters
	 */
	virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

	/// See parent
	virtual bool recordText();


};

#endif /* TRANSLINETEXT_H_ */
