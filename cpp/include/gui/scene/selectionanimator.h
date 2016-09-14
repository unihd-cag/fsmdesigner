/*
 * SelectionAnimator.h
 *
 *  Created on: May 10, 2010
 *      Author: rleys
 */

#ifndef SELECTIONANIMATOR_H_
#define SELECTIONANIMATOR_H_

// Includes
//----------------------

//-- Qt
#include <QtGui>
#include <QtCore>

/**
 * This class connects to the selection signals of a Graphics Scene and Triggers painting on selected items
 */
class SelectionAnimator : public QObject  {

	Q_OBJECT

private:


	/// The base Graphics Scene
	QGraphicsScene * scene;

    /// This timer applies a repetitive effect on the selected items
    QTimeLine * selectionEffectTimeline;

public:
	SelectionAnimator(QGraphicsScene * scene);
	virtual ~SelectionAnimator();




public slots:

	void selectionTimelineValue(qreal value);
	void sceneSelectionChanged();



};

#endif /* SELECTIONANIMATOR_H_ */
