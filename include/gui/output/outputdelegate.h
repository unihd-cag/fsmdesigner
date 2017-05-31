/***********************************************************************
 *                                                                      *
 * (C) 2009, Frank Lemke, Computer Architecture Group,                  *
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

#ifndef OUTPUTDELEGATE_H_
#define OUTPUTDELEGATE_H_

// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/common/ItemsRelatedEditor.h>
class Transline;
class State;

class OutputDelegate: public QStyledItemDelegate,
		public SceneRelatedObject,
		public ItemsRelatedEditor {

Q_OBJECT
	;
public:
	OutputDelegate(QObject* parent = 0);
	~OutputDelegate();

	QSize
			sizeHint(const QStyleOptionViewItem& option,
					const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model,
			const QModelIndex& index) const;

public Q_SLOTS:

	/// To be Linked to the related scene to update content
	virtual void selectionUpdated();

};

#endif
