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

//-- Std
#include <sstream>
#include <cstdio>

using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Trans.h>
#include <core/Project.h>
#include <core/State.h>

//-- Gui
#include <gui/items/Transline.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

//-- Actions
#include <gui/actions/ChangeConditionNameAction.h>
#include <gui/actions/ChangeConditionValueAction.h>
#include <gui/actions/ChangeTransitionNameAction.h>


#include "tabletransmodel.h"

TableTransModel::TableTransModel(Scene * scene,QObject* parent) :
        QStandardItemModel(parent) , SceneRelatedObject(scene) {

    // Define  Columns
    //---------------------------
    this->setHorizontalHeaderItem(0,new  QStandardItem("Start"));
    this->setHorizontalHeaderItem(1,new  QStandardItem("Transition"));
    this->setHorizontalHeaderItem(2,new  QStandardItem("Condition Name"));
    this->setHorizontalHeaderItem(3,new  QStandardItem("Value"));
    this->setHorizontalHeaderItem(4,new  QStandardItem("End"));

    // Watch over changes
    this->connect(this,SIGNAL(itemChanged(QStandardItem*)),SLOT(changed(QStandardItem*)));
}

TableTransModel::~TableTransModel() {


}

void TableTransModel::setRelatedScene(Scene * scene) {
    SceneRelatedObject::setRelatedScene(scene);

    if (scene==NULL)
        return;


    // Update Content
    //---------------
    QStandardItem * root = this->invisibleRootItem();
    FOREACH_TRANSITIONS(scene->getFsm())

        // Add Row with transition name
        //--------------
        QStandardItem *item = new QStandardItem(QString::fromStdString(transition->getName()));

        // Add Conditions
        //-------------
        int cCount = 0;
        FOREACH_TRANSITION_CONDITIONS(transition)

            // Create Columns for Transition
            QList<QStandardItem *> cols;

            //-- Start State
            cols+=new QStandardItem(QString::fromStdString(transition->getStartState()->getName()));
            cols.last()->setData(transition->getStartState()->getId(),Qt::EditRole);
            cols.last()->setData(QString::fromStdString(transition->getStartState()->getName()),Qt::DisplayRole);
            cols.last()->setEditable(false);

            //-- Transition Name
            cols+=new QStandardItem(QString::fromStdString(transition->getName()));
            //cols.last()->setData(QVariant(transition),Qt::EditRole);

            //-- Condition Name
            cols+=new QStandardItem(QString::fromStdString(condition->getName()));
            cols.last()->setData(transition->getId(),Qt::UserRole);
            cols.last()->setData(cCount,Qt::UserRole+1);

            //-- Condition Input
            cols+=new QStandardItem(QString::fromStdString(condition->getInput()).replace('-','x'));
            cols.last()->setData(transition->getId(),Qt::UserRole);
            cols.last()->setData(cCount,Qt::UserRole+1);

            //-- End State
            cols+=new QStandardItem(QString::fromStdString(transition->getEndState()->getName()));
            cols.last()->setData(transition->getEndState()->getId(),Qt::EditRole);
            cols.last()->setData(QString::fromStdString(transition->getEndState()->getName()),Qt::DisplayRole);
            cols.last()->setEditable(false);

            root->appendRow(cols);


            cCount++;
        END_FOREACH_TRANSITION_CONDITIONS


    END_FOREACH_TRANSITIONS
}


void TableTransModel::changed(QStandardItem* item) {



    // If UserRole+1 is valid, it is the index of the condition
    //--------------------------------
    if (item->data(Qt::UserRole+1).isValid()) {

        // UserRole = Condition id
        // UserRole+1 = transition id
        Trans * transition = getRelatedScene()->getFsm()->getTransbyID(item->data(Qt::UserRole).toUInt());
        Condition * condition = transition->getConditionByID(item->data(Qt::UserRole+1).toUInt());

        // Condition Name
        //----------------
        if (item->column()==2) {

            qDebug() << "Changing Condition to: " << item->data(Qt::EditRole) << " and " << item->data(Qt::EditRole+1);

            ChangeConditionNameAction * changeName = new ChangeConditionNameAction(item->data(Qt::EditRole).toString(),condition);
            getRelatedScene()->getUndoStack()->push(changeName);
        }
        // Condition Input Condition
        //-----------------
        else if (item->column()==3) {


            ChangeConditionValueAction * changeValue = new ChangeConditionValueAction(item->data(Qt::EditRole).toString(),condition);
            getRelatedScene()->getUndoStack()->push(changeValue);

        }

    }
    // Otherwise changing a transition
    //-------------------
    else {

        //-- Find Transline
        Trans * transition = getRelatedScene()->getFsm()->getTransbyID(item->data(Qt::UserRole).toUInt());
        Transline * transline = getRelatedScene()->findTransline(transition).first();

        // Transition Name
        //----------------
        if (item->column()==1) {

            ChangeTransitionNameAction * changeName = new ChangeTransitionNameAction(item->data(Qt::EditRole).toString(),transline);
            getRelatedScene()->getUndoStack()->push(changeName);
        }

    }


}





