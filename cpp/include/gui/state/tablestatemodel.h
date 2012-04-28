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

#ifndef TABLESTATEMODEL_H_
#define TABLESTATEMODEL_H_

// Includes
//----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Scene
#include <gui/scene/SceneRelatedObject.h>

//-- Gui
#include <gui/common/filterkeywords.h>

/**
 * Table displaying States
 */
class TableStateModel: public QAbstractItemModel, public SceneRelatedObject {

    public:
        TableStateModel(Scene * scene,QObject* parent = 0);
        virtual ~TableStateModel() ;

        /**
         * Returns 4 Columns:
         *
         *  - Name
         *  - State Output
         *  - Color
         *  - Reset ?
         *
         * @param index
         * @return
         */
        int columnCount(const QModelIndex& index) const;

        /**
         * Row count = number of states
         * @param parent
         * @return
         */
        int rowCount(const QModelIndex& parent = QModelIndex()) const;

        /**
         * Display Columns Header
         * @param section
         * @param orientation
         * @param role
         * @return
         */
        QVariant headerData(int section, Qt::Orientation orientation, int role =
               Qt::DisplayRole) const;

        /**
         * Return data
         * @param index
         * @param role
         * @return
         */
        QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const;

        /**
         * Set data
         * @param index
         * @param value
         * @param role
         * @return
         */
        bool setData(const QModelIndex& index, const QVariant& value, int role =
                Qt::EditRole);

        /**
         * Return if the column is editable
         * @param index
         * @return
         */
        bool isEditable(const QModelIndex& index) const;

        virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

        virtual  QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;

        virtual QModelIndex parent ( const QModelIndex & index ) const;


};

#endif 
