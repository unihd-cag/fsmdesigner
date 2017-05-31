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
//----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Core
#include <core/Fsm.h>
#include <core/State.h>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

//-- Gui
#include <gui/state/StatesComboBox.h>

#include "tabletransdelegate.h"

TableTransDelegate::TableTransDelegate(Scene * scene, QObject* parent) : QStyledItemDelegate(parent), SceneRelatedObject(scene) {

}

TableTransDelegate::~TableTransDelegate() {

}

void TableTransDelegate::paint(QPainter* painter,
        const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(painter, option, index);
}

QSize TableTransDelegate::sizeHint(const QStyleOptionViewItem& option,
        const QModelIndex& index) const {
    return QStyledItemDelegate::sizeHint(option, index);
}

void TableTransDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const {

    QStyledItemDelegate::setEditorData(editor, index);

}

QWidget* TableTransDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem& option, const QModelIndex& index) const {


    // C0 & C4 : States
    //--------------------------
    if (index.column() == 0 || index.column() == 4) {

        // Create a list of states name
        //-------------------
        StatesComboBox* cb = new StatesComboBox(relatedScene->getFsm(),false,relatedScene->getFsm()->getStatebyID(index.data(Qt::EditRole).toInt()),parent);


        return cb;
    }
    // Otherwise use normal Delegate
    //-----------------------------
    else {

        return QStyledItemDelegate::createEditor(parent,option,index);

    }


}

void TableTransDelegate::setModelData(QWidget* editor,
        QAbstractItemModel* model, const QModelIndex& index) const {

    // C0 & C4 : States
    //--------------------------
    if (index.column() == 0 || index.column() == 4) {


        // Get Editor
        //-----------------
        StatesComboBox * comboBox = static_cast<StatesComboBox*>(editor);
        model->setData(index,comboBox->getSelectedState()->getId(),Qt::EditRole);

    }
    // Normal
    else {

        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
