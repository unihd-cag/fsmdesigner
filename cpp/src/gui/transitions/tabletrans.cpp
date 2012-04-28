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
#include <gui/transitions/tabletransmodel.h>
#include <gui/transitions/tabletransdelegate.h>

#include "tabletrans.h"

TableTrans::TableTrans(QWidget* parent) :
        QTreeView(parent) {

    // Init Table
    //----------------------
    TableTransModel * transmodel = new TableTransModel(this->getRelatedScene(),this);
    TableTransDelegate * transdelegate = new TableTransDelegate(this->getRelatedScene(),this);
    this->setModel(transmodel);
    this->setItemDelegate(transdelegate);
    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);

    // Create Actions
    //---------------------
    addTrans = new QAction(this);
    addTrans->setObjectName(QString::fromUtf8("addtrans"));
    addTrans->setEnabled(true);
    addTrans->setText(
            QApplication::translate("TableTrans", "Add Transition", 0,
                    QApplication::UnicodeUTF8));
    deleteTrans = new QAction(this);
    deleteTrans->setObjectName(QString::fromUtf8("deletetrans"));
    deleteTrans->setEnabled(true);
    deleteTrans->setText(
            QApplication::translate("TableTrans", "Delete Transition", 0,
                    QApplication::UnicodeUTF8));
    addCondition = new QAction(this);
    addCondition->setObjectName(QString::fromUtf8("addcondition"));
    addCondition->setEnabled(true);
    addCondition->setText(
            QApplication::translate("TableTrans", "Add Condition", 0,
                    QApplication::UnicodeUTF8));
    deleteCondition = new QAction(this);
    deleteCondition->setObjectName(QString::fromUtf8("deleteCondition"));
    deleteCondition->setEnabled(true);
    deleteCondition->setText(
            QApplication::translate("TableTrans", "Delete Condition", 0,
                    QApplication::UnicodeUTF8));
}

TableTrans::~TableTrans() {

}

void TableTrans::setRelatedScene(Scene * scene) {

    // Parent
    SceneRelatedObject::setRelatedScene(scene);

    // Delegate
    dynamic_cast<TableTransDelegate*>(this->itemDelegate())->setRelatedScene(scene);

    // Model
    dynamic_cast<TableTransModel*>(this->model())->setRelatedScene(scene);
}

void TableTrans::contextMenuEvent(QContextMenuEvent* e) {

    /*
    Core& core = CoreFactory::getCore();
    Fsm& f = *(core.getProject()->getFSMs().first());
    QMenu contextMenu(this);
    contextMenu.addAction(addTrans);
    if (currentIndex().parent() == QModelIndex()) {
        int row = 0;
        if (f.getFirstTrans()) {
            do {
                if (row == currentIndex().row()) {
                    contextMenu.addAction(deleteTrans);
                    contextMenu.addAction(addCondition);
                }
                row++;
            } while (f.getNextTrans());
        }
    } else {
        if (currentIndex().isValid()) {
            contextMenu.addAction(deleteCondition);
        }
    }
    contextMenu.exec(e->globalPos());*/
}

