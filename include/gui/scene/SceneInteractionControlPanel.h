/*
 * SceneInteractionControlPanel.h
 *
 *  Created on: Sep 28, 2010
 *      Author: rleys
 */

#ifndef SCENEINTERACTIONCONTROLPANEL_H_
#define SCENEINTERACTIONCONTROLPANEL_H_

// Includes
//-------------------

//-- Qt
#include <QtGui>
#include <QtCore>


/// A Panel to show Extra display enhancement functions
class SceneInteractionControlPanel : public QGraphicsWidget {

	Q_OBJECT

public:
	SceneInteractionControlPanel();
	virtual ~SceneInteractionControlPanel();

protected:

	/// Return Layout as a Grid Layout
	QGraphicsGridLayout * getLayout() {
		return dynamic_cast<QGraphicsGridLayout*>(this->layout());
	}
};

#endif /* SCENEINTERACTIONCONTROLPANEL_H_ */
