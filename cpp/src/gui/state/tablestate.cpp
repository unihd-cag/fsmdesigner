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

// Includes
//----------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

//-- Gui
#include <gui/state/tablestatemodel.h>
#include <gui/state/tablestatedelegate.h>

#include "tablestate.h"

TableState::TableState(QWidget* parent) :
        QTreeView(parent) {

    // Init Table
    //-------------------------
    TableStateModel * statemodel = new TableStateModel(this->getRelatedScene(),this);
    TableStateDelegate * statedelegate = new TableStateDelegate(this->getRelatedScene(),this);
    this->setModel(statemodel);
    this->setItemDelegate(statedelegate);
    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);
    this->sortByColumn(2, Qt::AscendingOrder);



    // Create actions
    //------------------------

    //-- Add state
    this->addState = new QAction(this);
    this->addState->setObjectName(QString::fromUtf8("addstate"));
    this->addState->setEnabled(true);
    this->addState->setText(
            QApplication::translate("TableState", "Add State", 0,
                    QApplication::UnicodeUTF8));

    //-- Delete state
    this->deleteState = new QAction(this);
    this->deleteState->setObjectName(QString::fromUtf8("deletestate"));
    this->deleteState->setEnabled(true);
    this->deleteState->setText(
            QApplication::translate("TableState", "Delete State", 0,
                    QApplication::UnicodeUTF8));
}

TableState::~TableState() {

}


void TableState::setRelatedScene(Scene * scene) {

    // parent
    SceneRelatedObject::setRelatedScene(scene);

    // Delegate
    dynamic_cast<TableStateDelegate*>(this->itemDelegate())->setRelatedScene(scene);

    // Model
    dynamic_cast<TableStateModel*>(this->model())->setRelatedScene(scene);
}

void TableState::contextMenuEvent(QContextMenuEvent* e) {

//    // Create Context Menu
//    //------------
//    QMenu contextMenu(this);

//    // Add Add State
//    //------------------
//    contextMenu.addAction(addState);

//    // Add Delete state if we are in the rows
//    //----------------
//    if (currentIndex().isValid()) {
//        contextMenu.addAction(deleteState);
//    }

//    // Show
//    //-----------
//    contextMenu.exec(e->globalPos());
}

