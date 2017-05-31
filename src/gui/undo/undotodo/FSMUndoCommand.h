/*
 * FSMUndoCommand.h
 *
 *  Created on: Mar 17, 2011
 *      Author: rleys
 */
#ifndef FSMUNDOCOMMAND_H_
#define FSMUNDOCOMMAND_H_

// Includes
//-------------

//-- Core
class Scene;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>


template <class G,class M>
class FSMUndoCommand : public QUndoCommand , public SceneRelatedObject {


protected:

	/// The GUi item serving as source for deletion
	G * deletedGuiItem;

	/// The model that has been/will be deleted
	M * deletedModel;

public:

	FSMUndoCommand() {

		// Init pointers
		this->deletedGuiItem = deletedGuiItem;
		this->deletedModel = deletedModel;
	}

	FSMUndoCommand(G *deletedGuiItem,M*deletedModel, QUndoCommand * parent = NULL ) :
		QUndoCommand(parent), SceneRelatedObject() {

		// Init pointers
		this->deletedGuiItem = deletedGuiItem;
		this->deletedModel = deletedModel;

		// Get scene
		if (this->deletedGuiItem!=NULL) {
			this->setRelatedScene(dynamic_cast<Scene*>(dynamic_cast<QGraphicsItem*>(this->deletedGuiItem)->scene()));
		}
	}

};

#endif /* FSMUNDOCOMMAND_H_ */
