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

/**
 * Base class for an Action related to a FSM
 */
class FSMAction : public QUndoCommand, public SceneRelatedObject {

    protected:

    public:

        /**
         * Default constructor
         * Must always refer to a scene
         */
        FSMAction(Scene * scene,QUndoCommand * parentCommand = NULL);

};

#endif /* FSMUNDOCOMMAND_H_ */
