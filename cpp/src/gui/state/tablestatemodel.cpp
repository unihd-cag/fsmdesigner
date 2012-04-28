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
#include <iostream>

using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/State.h>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

//-- Gui
#include <gui/state/tablestate.h>
#include <gui/actions/ChangeStateNameAction.h>
#include <gui/actions/ChangeStateOutputAction.h>
#include <gui/actions/ChangeStateColorAction.h>

#include "tablestatemodel.h"

TableStateModel::TableStateModel(Scene * scene, QObject* parent) :
        QAbstractItemModel(parent), SceneRelatedObject(scene) {
}

TableStateModel::~TableStateModel() {

}

int TableStateModel::columnCount(const QModelIndex& index) const {

    Q_UNUSED(index);

    return 4;
}

int TableStateModel::rowCount(const QModelIndex& parent) const {

    if (parent != QModelIndex()) {
        return 0;
    }

    return relatedScene->getFsm()->getStateCount();

}


QVariant TableStateModel::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation == Qt::Horizontal) {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (section) {
            case 0:
                return QString("Name");
            case 1:
                return QString("Output");
            case 2:
                return QString("Color");
            case 3:
                return QString("Resetstate");
            default:
                return QString();
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}


QVariant TableStateModel::data(const QModelIndex& index, int role) const {

    Fsm& f = *(relatedScene->getFsm());

    // Only Support EditRole and DisplayRole
    //------------------
    if (role!=Qt::EditRole && role != Qt::DisplayRole) {
        return QVariant();
    }

    // Switch on column
    //---------------------------
    switch (index.column()) {

        // Name
        //------------------
        case 0: {

            return QString::fromStdString(relatedScene->getFsm()->getStatesArray()[index.row()]->getName());

            break;
        }
        // State Output
        //------------------
        case 1: {

            return QString::fromStdString(relatedScene->getFsm()->getStatesArray()[index.row()]->getOutput());

            break;
        }
        // State Color
        //------------------
        case 2:{

            QColor col = QColor::fromRgb(relatedScene->getFsm()->getStatesArray()[index.row()]->getColor());
            //qDebug() << "Got Color for display : " << col.rgb();

            return col;

            break;
        }
        // Reset ?
        //------------------
        case 3:{

            return relatedScene->getFsm()->getStatesArray()[index.row()]->isReset();

            break;
        }
    }

    return QVariant();

}


bool TableStateModel::setData(const QModelIndex& index, const QVariant& value,
        int role) {



    switch (index.column()) {

       // Change Name
       //-------------------
       case 0: {

           // New Name
           //--------------
           QString newName = value.toString().trimmed();

           // Do change
           //---------------
           StateItem * item = relatedScene->findStateItem(relatedScene->getFsm()->getStatesArray()[index.row()]);
           relatedScene->getUndoStack()->push(new ChangeStateNameAction(newName,item));

           return true;
           break;
       }
       // State Output
       //------------------
       case 1: {

           // New Output
           //--------------
           QString newOutput = value.toString().trimmed();

           // Do change
           //---------------
           StateItem * item = relatedScene->findStateItem(relatedScene->getFsm()->getStatesArray()[index.row()]);
           relatedScene->getUndoStack()->push(new ChangeStateOutputAction(newOutput,item));

           return true;
           break;
       }
       // State Color
       //------------------
       case 2:{

           // New Color
           //--------------
           QColor newColor =    value.value<QColor>();


           qDebug() << "Changing color to : " << newColor.rgb();

           // Do change
           //---------------
           StateItem * item = relatedScene->findStateItem(relatedScene->getFsm()->getStatesArray()[index.row()]);
           relatedScene->getUndoStack()->push(new ChangeStateColorAction(newColor,item));

           return true;
           break;
       }
       // Reset ?
       //------------------
       case 3:{

           //return relatedScene->getFsm()->getStatesArray()[index.row()]->isReset();

           break;
       }
    }



    return false;
/*
    switch (index.column()) {

        // Name
        //-----------
        case 0:
            ir = index.row();
            i = 0;
            id = -1;
            if (f.getFirstState()) {
                do {
                    if (ir == i) {
                        id = f.getStateID();
                    }
                    i++;
                } while (f.getNextState());
            }
            if (id > -1) {
                QString qs = value.toString().trimmed();
                FilterKeywords* fk;
                fk = new FilterKeywords();
                qs = fk->RemoveBlanks(qs);
                qs = fk->FilterSpecialChars(qs);
                qs = fk->FilterHDLKeywords(qs);
                qs = fk->FilterBusChars(qs);
                //f.modifyState(id, value.toString().trimmed().toStdString());
                f.modifyState(id, qs.toStdString());
            }
            break;

        // Output
        //------------------
        case 1:
            ir = index.row();
            i = 0;
            id = -1;
            if (f.getFirstState()) {
                do {
                    if (ir == i) {
                        id = f.getStateID();
                    }
                    i++;
                } while (f.getNextState());
            }
            if (id > -1) {
                f.stateOutput(id, value.toString().trimmed().toStdString());
            }
            break;
        // Color
        //----------------
        case 2:
            ir = index.row();
            i = 0;
            id = -1;
            if (f.getFirstState()) {
                do {
                    if (ir == i) {
                        id = f.getStateID();
                    }
                    i++;
                } while (f.getNextState());
            }
            if (id > -1) {
                unsigned int color = value.value<QColor>().rgb(); //value.toString().trimmed().toInt();
                f.modifyState(id, color);
            }
            break;

        // Reset ?
        //---------------
        case 3:
            ir = index.row();
            i = 0;
            id = -1;
            if (f.getFirstState()) {
                do {
                    if (ir == i) {
                        id = f.getStateID();
                    }
                    i++;
                } while (f.getNextState());
            }
            if (id > -1) {
                bool b = value.toBool();
                if (b == true) {
                    f.resetState(id);
                    dynamic_cast<TableState*>(wid)->doItemsLayout();
                }
            }
            break;
        default:
            break;
            return true;
    }
    return false;*/
}


bool TableStateModel::isEditable(const QModelIndex& index) const {

    if (index.isValid())
        return true;
    return false;
    //return false;
}

Qt::ItemFlags TableStateModel::flags ( const QModelIndex & index ) const {

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

}

QModelIndex TableStateModel::index ( int row, int column, const QModelIndex & parent) const {


    return createIndex(row, column, 0);

}

QModelIndex TableStateModel::parent ( const QModelIndex & index ) const {


    return QModelIndex();


}

