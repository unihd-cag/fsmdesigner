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

#define DEBUGINPUTMODEL

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
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/Trans.h>
#include <core/Hypertrans.h>
#include <core/State.h>

//-- Gui
#include <gui/common/ItemsRelatedEditor.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include <gui/input/inputwidget.h>

#include <gui/items/Transline.h>
#include <gui/items/HyperTransition.h>


#include "inputmodel.h"
InputModel::InputModel(QObject* parent) :
	QAbstractItemModel(parent), ItemsRelatedEditor() {

	//-- Init
	this->conditionID = -1;

}

InputModel::~InputModel() {

}

int InputModel::columnCount(const QModelIndex& index) const {

	Q_UNUSED(index);

	return 2;
}

int InputModel::rowCount(const QModelIndex& parent) const {

	if (parent != QModelIndex() || relatedScene == NULL
			|| relatedScene->getFsm() == NULL) {
		return 0;
	}

	return relatedScene->getFsm()->getNumberOfInputs(); //State

}

QVariant InputModel::data(const QModelIndex& index, int role) const {

	string s;
	QString qs;
	int ia;
	int i;
	string cnames;
	QString cnameqs;
	int cnamerow = 0;
	QString qsind;
	stringstream ss;
	int ifsmid;
	int nInputs;
	nInputs = relatedScene->getFsm()->getNumberOfInputs();

	// First Column -> Names ( Always displayed)
    //-------------------
    if ((index.column() == 0) && ((role == Qt::EditRole) || (role
                    == Qt::DisplayRole))) {

          return QString::fromStdString(relatedScene->getFsm()->getInputName(index.row()));

    }

	// Do nothing if index is invalid or no condition has been selected
	if (!index.isValid() || this->conditionID==-1)
	    return QVariant();


	// Second Column -> Data (only if a condition is selected)
	//--------------------
	if ((index.column() == 1) && ((role == Qt::EditRole) || (role
            == Qt::DisplayRole))) {

	    //----- Transition
        //-------------------------
	    if (editingItemType == FSMDesigner::TRANS) {

	        Trans * trans = (Trans*)static_cast<Transline*>(editingItem)->getModel();
            Condition * condition = trans->getConditionByID(this->conditionID);
            return QString(QChar(condition->getInputBit(index.row())));


	    }
        //----- Hyper Transition
        //-------------------------
	    else if (editingItemType == FSMDesigner::HYPERTRANS) {

	        Hypertrans * htrans = static_cast<HyperTransition*>(editingItem)->getModel();
	        Condition * condition = htrans->getConditionByID(this->conditionID);
            return QString(QChar(condition->getInputBit(index.row())));

	    }

	}

	return QVariant();

}

QModelIndex InputModel::index(int row, int column, const QModelIndex& parent) const {

	int id = row * 2 + column;

	if (!parent.isValid()) {
		return createIndex(row, column, id);
	}

	return createIndex(row, column, 100);
}

QModelIndex InputModel::parent(const QModelIndex& index) const {

	if (!index.isValid() || index.internalId() < 1000) {
		return QModelIndex();
	}
	return createIndex(index.row(), index.column(), 1000);
}


bool InputModel::setData(const QModelIndex& index, const QVariant& value,
		int role) {

		char cv;
		char cs[255];

		// Second Column of Transition
		if ((index.column() == 1) && this->conditionID>=0 && (editingItemType == FSMDesigner::TRANS)) {

			QVariant vv = value;

			QString qs2 = vv.toString();
			QByteArray ba = qs2.toLatin1();
			const char *c_str2 = ba.data();
			char cv2 = c_str2[0];

			if ((cv2 == '0') || (cv2 == '1') || (cv2 == '-') || (cv2 == 'x')
					|| (cv2 == 'X')) {
			    static_cast<Transline*>(editingItem)->getModel()->getConditionByID(this->conditionID)->changeInputAt(index.row(), cv2);
			} else {
				cv2 = '-';
				static_cast<Transline*>(editingItem)->getModel()->getConditionByID(this->conditionID)->changeInputAt(index.row(), cv2);
			}

		}

		return true;

}

Qt::ItemFlags InputModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flag = QAbstractItemModel::flags(index);
	if (isEditable(index)) {
		flag |= Qt::ItemIsEditable;
	}
	return flag;
}

bool InputModel::isEditable(const QModelIndex& index) const {

	if (index.row() == -1) {
		return false;
	} else {
		return true;
	}
}

QModelIndex InputModel::buddy(const QModelIndex& index) const {
	if (index.column() == 0) {
		return createIndex(index.row(), 1, index.internalPointer());
	}
	return index;
}

QVariant InputModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation == Qt::Horizontal) {
		if (role != Qt::DisplayRole) {
			return QVariant();
		}
		switch (section) {
		case 0:
			return QString("Inputs");
		case 1:
			return QString("Condition Bits");
		default:
			return QString();
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

void InputModel::rowsAdded(int first, int last) {

	//dynamic_cast<MainguiWindow*>(w)->outputeditorwidget->model()->
	beginInsertRows(QModelIndex(), first, last);
	endInsertRows();
	layoutChanged();
}

void InputModel::rowsChanged() {
	layoutChanged();
}

// FIXME
void InputModel::comboBoxAddItem(QString text) {
	//dynamic_cast<MainguiWindow*> (wid)->cNameComboBox->addItem(text);
}

void InputModel::setConditionID(int id) {
	this->conditionID = id;
}

