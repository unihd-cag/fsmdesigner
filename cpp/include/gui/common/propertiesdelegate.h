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

#ifndef PROPERTIESDELEGATE_H_
#define PROPERTIESDELEGATE_H_

// Includes
//------------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/common/ItemsRelatedEditor.h>
#include <gui/common/filterkeywords.h>

class PropertiesDelegate : public QStyledItemDelegate, public SceneRelatedObject,public ItemsRelatedEditor {

  Q_OBJECT;

    public:

        enum ROW_STATE {
            ROW_STATE_TYPE,
            ROW_STATE_NAME,
            ROW_STATE_RESET,
            ROW_STATE_OUTPUT,
            ROW_STATE_COLOR
        };

        enum ROW_TRANS {
            ROW_TRANS_TYPE,
            ROW_TRANS_NAME,
            ROW_TRANS_DEFAULT,
            ROW_TRANS_START,
            ROW_TRANS_END
        };

        enum ROW_HTRANS {
            ROW_HTRANS_TYPE,
            ROW_HTRANS_NAME,
            ROW_HTRANS_END
        };

        enum ROW_LINK {
            ROW_LINK_TYPE,
            ROW_LINK_NAME,
            ROW_LINK_TARGET,
            ROW_LINK_RESET,
            ROW_LINK_COLOR
        };




        PropertiesDelegate(QObject* parent = 0);
        ~PropertiesDelegate();


        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
        void setEditorData(QWidget* editor,const QModelIndex& index) const;
        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;


    public:

        /// Reimplement from parent to propagate to model and delegate
        virtual void setRelatedScene(Scene * scene);

    public Q_SLOTS:

        /// To be Linked to the related scene to update content
        virtual void selectionUpdated();

    protected:
        void setEdit() {colorEdited=true;}
        bool colorEdited;

};


#endif
