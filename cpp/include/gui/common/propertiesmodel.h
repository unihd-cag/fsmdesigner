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

#ifndef PROPERTIESMODEL_H_
#define PROPERTIESMODEL_H_

// Includes
//-------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/common/ItemsRelatedEditor.h>

class PropertiesModel: public QAbstractItemModel,
        public SceneRelatedObject,
        public ItemsRelatedEditor {

    Q_OBJECT

    public:

        PropertiesModel(QObject* parent = 0);
        virtual ~PropertiesModel();

        int columnCount(const QModelIndex& index) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const;
        QModelIndex index(int row, int column, const QModelIndex& parent =
                QModelIndex()) const;
        QModelIndex parent(const QModelIndex& index) const;
        bool setData(const QModelIndex& index, const QVariant& value, int role =
                Qt::EditRole);
        Qt::ItemFlags flags(const QModelIndex& index) const;
        bool isEditable(const QModelIndex& index) const;
        QModelIndex buddy(const QModelIndex& index) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role =
                Qt::DisplayRole) const;

    public:

        /// Reimplement from parent to propagate to model and delegate
        virtual void setRelatedScene(Scene * scene);

    public Q_SLOTS:

        /// To be Linked to the related scene to update content
        virtual void selectionUpdated();

};

#endif 
