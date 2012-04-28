/*
 * FSMUndoCommand.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: rleys
 */

// Includes
//-------------
//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/Scene.h>
#include <gui/scene/SceneRelatedObject.h>

#include "FSMAction.h"

FSMAction::FSMAction(Scene * scene,QUndoCommand * parentCommand) : SceneRelatedObject(scene) , QUndoCommand(parentCommand) {

}

