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
//----------------------

//-- Std
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/FSMDesigner.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Gui
#include <gui/common/ItemsRelatedEditor.h>

#include <gui/scene/SceneRelatedObject.h>

#include <gui/input/inputwidget.h>

#include "inputdelegate.h"

InputDelegate::InputDelegate(QObject* parent) :
	QStyledItemDelegate(parent), ItemsRelatedEditor() {

}

InputDelegate::~InputDelegate() {

}

QSize InputDelegate::sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	return QStyledItemDelegate::sizeHint(option, index);
}

void InputDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {

	if (editingItemType == FSMDesigner::TRANS) {
		QStyledItemDelegate::setEditorData(editor, index);
	} else {
		QStyledItemDelegate::setEditorData(editor, index);
	}
}

QWidget* InputDelegate::createEditor(QWidget* parent,
		const QStyleOptionViewItem& option, const QModelIndex& index) const {
	if (editingItemType == FSMDesigner::TRANS) {
		return QStyledItemDelegate::createEditor(parent, option, index);
	} else {
		return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

void InputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const {

	if (editingItemType == FSMDesigner::TRANS) {
		QStyledItemDelegate::setModelData(editor, model, index);
	} else {
		QStyledItemDelegate::setModelData(editor, model, index);
	}
}

