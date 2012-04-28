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

#ifndef TABLETRANS_H_
#define TABLETRANS_H_

// Includes
//-------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>

class TableTrans: public QTreeView, SceneRelatedObject {

    protected:
        QAction* addCondition;
        QAction* deleteCondition;
        QAction* addTrans;
        QAction* deleteTrans;

    public:
        TableTrans(QWidget* parent);
        virtual ~TableTrans();


        /**
         * Show right click menu for actions
         * @param e
         */
        void contextMenuEvent(QContextMenuEvent* e);

        /**
         * Propagate to Model and Delegate
         * @param scene
         */
        void setRelatedScene(Scene * scene);


};

#endif
