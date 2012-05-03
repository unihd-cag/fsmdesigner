/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

// Includes
//--------------

//-- QT
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
class Scene;
class StateItem;
class Transline;

class PropertyWidget: public QTreeView, public SceneRelatedObject {

Q_OBJECT

public:
	QAction* addCondiToTrans;
	QAction* deleteCondiOfTrans;


	PropertyWidget(QWidget* w);

	void contextMenuEvent(QContextMenuEvent* e);


private:

	void createActions();

public Q_SLOTS:

    /// Reimplement from parent to propagate to model and delegate
    virtual void setRelatedScene(Scene * scene);

	/// To be Linked to the related scene to update content
	virtual void selectionUpdated();

	/** \defgroup Properties Actions to be executed on model */
	/**@{ */

	void addConditionToTransitionBase();
	void deleteConditionFromTransitionBase();


	/** @} */



};

#endif
