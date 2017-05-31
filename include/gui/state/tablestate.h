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

#ifndef TABLESTATE_H_
#define TABLESTATE_H_

// Includes
//-------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>

/**
 * Global State Table Widget
 */
class TableState: public QTreeView , public SceneRelatedObject {

    protected:

        /**
         * Called Anywhere in the table to add a state
         */
        QAction* addState;

        /**
         * Called when clicked on state line to delete the state
         */
        QAction* deleteState;

    public:

        /**
         * Create actions
         * @param w
         */
        TableState(QWidget* parent = NULL);

        virtual ~TableState();

        /**
         * Propagate scene to delegate and model
         * @param scene
         */
        void setRelatedScene(Scene * scene);

        /**
         * Right click menu to delete state
         * @param e
         */
        void contextMenuEvent(QContextMenuEvent* e);


};

#endif
