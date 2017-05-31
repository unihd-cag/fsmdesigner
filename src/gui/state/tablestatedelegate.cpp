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
//--------------------
//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/common/ColorButton.h>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include "tablestatedelegate.h"

TableStateDelegate::TableStateDelegate(Scene * scene,QObject* parent) :
    QStyledItemDelegate(parent) , SceneRelatedObject(scene) {

}

TableStateDelegate::~TableStateDelegate() {

}

void TableStateDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {

    // Color for Color
    //--------------------
    if (index.column() == 2) {

        QColor color = index.data().value<QColor>();
        QSize size = this->sizeHint(option,index);

        //-- Paint and keep a border to cell
        painter->save();
        painter->translate(option.rect.x(),option.rect.y());
        //painter->fillRect(option.rect,0,size.width(),size.height(),color);
        //painter->scale(0.8,0.8);
        painter->fillRect(0+2,0+2,option.rect.width()-2,option.rect.height()-2,color);

        painter->restore();
        //QStyledItemDelegate::paint(painter,option,index);
    }
    // Standard Otherwise
    //-----------------------
    else {

        QStyledItemDelegate::paint(painter,option,index);

    }

}

QWidget* TableStateDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem& option, const QModelIndex& index) const {

    // Color Button for Color
    //--------------------
    if (index.column() == 2) {

        return new QColorDialog(index.data().value<QColor>());
    }
    // StandardEditor otherwise
    //--------------
    else {
        return QStyledItemDelegate::createEditor(parent, option, index);
   }


}


void TableStateDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const {

    // Color
    //-------------------
    if (index.column() == 2) {
        static_cast<QColorDialog*>(editor)->setCurrentColor(index.data().value<QColor>());
    }
    // Standard otherwise
    //-----------------------------
    else {

        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TableStateDelegate::setModelData(QWidget* editor,
        QAbstractItemModel* model, const QModelIndex& index) const {

    // Color
    //---------------------
    if (index.column() == 2) {
        const_cast<QAbstractItemModel*>(index.model())->setData(index,
                static_cast<QColorDialog*>(editor)->currentColor(), Qt::EditRole);
    }
    // Reset Selection ?
    //----------------------
    else if (index.column() == 3) {
        QComboBox *box = static_cast<QComboBox*>(editor);

        //-- False
        if (box->currentIndex() == 0)
            const_cast<QAbstractItemModel*>(index.model())->setData(index,
                    false, Qt::EditRole);
        //-- True
        else
            const_cast<QAbstractItemModel*>(index.model())->setData(index, true,
                    Qt::EditRole);
    }
    // Standard String otherwise
    //---------------------------------
    else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
