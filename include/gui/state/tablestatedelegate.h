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

#ifndef TABLESTATEDELEGATE_H_
#define TABLESTATEDELEGATE_H_

// Includes
//-------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>

/**
 *
 */
class TableStateDelegate: public QStyledItemDelegate, public SceneRelatedObject {

    public:
        TableStateDelegate(Scene * scene, QObject* parent = 0);
        virtual ~TableStateDelegate();

        /**
         * Display color as a color field, rest as normal
         * @param painter
         * @param option
         * @param index
         */
        virtual void    paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        /**
         * Create an editor for a specific column/row
         * @param parent
         * @param option
         * @param index
         * @return
         */
        QWidget* createEditor(QWidget* parent,
                        const QStyleOptionViewItem& option,
                        const QModelIndex& index) const;

        /**
         * Fill the editor with appropriate data
         * @param editor
         * @param index
         */
        void setEditorData(QWidget* editor, const QModelIndex& index) const;

        /**
         * Set model data from editor
         * @param editor
         * @param model
         * @param index
         */
        void setModelData(QWidget* editor, QAbstractItemModel* model,
                const QModelIndex& index) const;



};

#endif
