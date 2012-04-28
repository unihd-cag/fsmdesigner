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

// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/FSMDesigner.h>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/common/ItemsRelatedEditor.h>

#include "outputdelegate.h"

OutputDelegate::OutputDelegate(QObject* parent) :
	QStyledItemDelegate(parent), ItemsRelatedEditor() {
}

OutputDelegate::~OutputDelegate() {

}

QSize OutputDelegate::sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	return QStyledItemDelegate::sizeHint(option, index);
}

void OutputDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {

	if (editingItemType == FSMDesigner::STATE) {

		QStyledItemDelegate::setEditorData(editor, index);
	} else { // all other properties
		QStyledItemDelegate::setEditorData(editor, index);
	}
}

QWidget* OutputDelegate::createEditor(QWidget* parent,
		const QStyleOptionViewItem& option, const QModelIndex& index) const {

		return QStyledItemDelegate::createEditor(parent, option, index);

}

void OutputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const {

	if (editingItemType == FSMDesigner::STATE) {
		QStyledItemDelegate::setModelData(editor, model, index);
	} else {
		QStyledItemDelegate::setModelData(editor, model, index);
	}
}

void OutputDelegate::selectionUpdated() {

}
