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
#include <core/Trans.h>
#include <core/Condition.h>
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
    TableTransModel * transmodel = new TableTransModel(this->getRelatedScene(),
            this);
    TableTransDelegate * transdelegate = new TableTransDelegate(
            this->getRelatedScene(), this);
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
    dynamic_cast<TableTransDelegate*>(this->itemDelegate())->setRelatedScene(
            scene);

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

void TableTrans::enteredRule(VerificatorRule * rule) {

    // Clean all coloring
    //----------------------------

    for (int row = 0; row < this->model()->rowCount(); row++) {

        //-- Get Row data
        QMap<int, QVariant> rowDatas = this->model()->itemData(
                this->model()->index(row, 2));

        //-- Remove background color
        rowDatas.remove(Qt::BackgroundColorRole);

        //-- Reset
        this->model()->setItemData(this->model()->index(row, 2), rowDatas);
    }

}

void TableTrans::ruleSuccessful(VerificatorRule * rule) {

}

void TableTrans::ruleFailed(VerificatorRule * rule, QList<RuleError*>& errors) {

    // Go through all errors
    //----------------------
    for (QList<RuleError*>::iterator eit = errors.begin(); eit != errors.end();
            eit++) {

        // Condition
        list<pair<FSMDesigner::Item, void *> > objects =
                (*eit)->getConcernedObjects();

        Condition * erroredSourceCondition = (Condition*) (objects.front()).second;
        Condition * erroredComparedCondition = (Condition*) (objects.back()).second;

        // Go through all lines to find out if the error is targetting this line
        //------------
        for (int row = 0; row < this->model()->rowCount(); row++) {

            //-- Get Row data
            QMap<int, QVariant> rowDatas = this->model()->itemData(
                    this->model()->index(row, 2));

            //-- Row Transition ID is at column 2 Qt::UserRole in QVariant map
            Trans * currentTrans =
                    this->getRelatedScene()->getFsm()->getTransbyID(
                            rowDatas[Qt::UserRole].toUInt());

            //-- Row condition ID is at column 2, Qt::UserRole+1 in QVariant map
            Condition * currentCondition = currentTrans->getConditionByID(
                    rowDatas[Qt::UserRole + 1].toUInt());

            // Check this current Condition is concerned by the error
            //------------------------
            if (currentCondition == erroredSourceCondition || currentCondition == erroredComparedCondition ) {

                //-- Paint the row red
                //this->itemDelegate(this->model()->index(row,2))->setUserData(Qt::BackgroundColorRole,QBrush(Qt::red));
                //this->model()->itemData(this->model()->index)
                qDebug() << "An Error is matching at line: " << row;
                rowDatas[Qt::BackgroundColorRole] = QBrush(Qt::red);
                this->model()->setItemData(this->model()->index(row, 2),
                        rowDatas);



            }

        }

    }

}

