/*
 * SceneRelatedWidget.cpp
 *
 *  Created on: May 20, 2011
 *      Author: rleys
 */

// Includes
//------------

//-- Qt
#include <QtGui>

//-- Gui
#include <gui/scene/Scene.h>

#include "SceneRelatedObject.h"


SceneRelatedObject::SceneRelatedObject(Scene * scene) {
	this->relatedScene = scene;

}

SceneRelatedObject::~SceneRelatedObject() {
	// TODO Auto-generated destructor stub
}

void SceneRelatedObject::setRelatedScene(Scene * scene) {

//	//-- Disconnection
//	if (this->getRelatedScene() != NULL) {
//		QObject::disconnect(this->getRelatedScene(),SIGNAL(selectionChanged()), (QObject*)this,
//				SLOT(selectionUpdated()));
//	}

	//-- Set Scene
	this->relatedScene = scene;

//	if (this->getRelatedScene() != NULL) {
//		//-- Connection
//		QObject::connect(this->getRelatedScene(), SIGNAL(selectionChanged()),(QObject*)this,
//				SLOT(selectionUpdated()));
//	}

}

