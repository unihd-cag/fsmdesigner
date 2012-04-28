/*
 * SceneRelatedWidget.h
 *
 *  Created on: May 20, 2011
 *      Author: rleys
 */

#ifndef SCENERELATEDOBJECT_H_
#define SCENERELATEDOBJECT_H_


// Includes
//------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
class Scene;

/**
 * Class to be implemented by Widgets that are related to a scene, to help with some common variables
 */
class SceneRelatedObject {

protected:

	/// The scene to which the object is related
	Scene * relatedScene;

public:
	SceneRelatedObject(Scene * scene = NULL);
	virtual ~SceneRelatedObject();


	virtual void setRelatedScene(Scene * scene) ;

	Scene * getRelatedScene() {
		return this->relatedScene;
	}



	/// To be Linked to the related scene to update content
	//virtual void selectionUpdated();

};

#endif /* SCENERELATEDOBJECT_H_ */
