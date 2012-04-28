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
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
#include <core/Core.h>
#include <core/State.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include <gui/items/StateItem.h>

#include <gui/output/outputwidget.h>



#include "outputmodel.h"

OutputModel::OutputModel(QObject* parent) :
	QAbstractItemModel(parent), ItemsRelatedEditor() {

}

OutputModel::~OutputModel() {

}

int OutputModel::columnCount(const QModelIndex& index) const {

	Q_UNUSED(index);

	return 2;
}

int OutputModel::rowCount(const QModelIndex& parent) const {

	if (parent != QModelIndex() || relatedScene == NULL
			|| relatedScene->getFsm() == NULL) {
		return 0;
	}

	Fsm& fsm = *(relatedScene->getFsm());
	return fsm.getNumberOfOutputs(); //State


}

QVariant OutputModel::data(const QModelIndex& index, int role) const {


	string s;
	QString qs;
	int ia;
	int i;
	stringstream ss;
	int ifsmid;
	int nOutputs;
	nOutputs = relatedScene->getFsm()->getNumberOfOutputs();

	//-- Analyse Role
	bool isEditRole = role== Qt::EditRole;
	bool isDisplayRole = role== Qt::DisplayRole;
	bool isEditOrDisplayRole = isEditRole | isDisplayRole;

	if (editingItemType == FSMDesigner::STATE) {
		ifsmid = relatedScene->getFsm()->getFsmID();
		nOutputs = relatedScene->getFsm()->getNumberOfOutputs();

		// FIRST COLUMN (Output name)
		if ((index.column() == 0) && isEditOrDisplayRole) {
			if ((index.row() < nOutputs)) {
				return QString::fromStdString(relatedScene->getFsm()->getOutputName(index.row()));
			} else {
				return QString("");
			}
		}
		// SECOND COLUMN (Value for state)
		if ((index.column() == 1) && isEditOrDisplayRole) {

			if (index.row() < nOutputs) {
				return QString(QChar(static_cast<StateItem*>(editingItem)->getModel()->getOutputAt(index.row())));
			}

			return QString("");
		}

	} else {

		nOutputs = relatedScene->getFsm()->getNumberOfOutputs();
		// FIRST COLUMN
		if ((index.column() == 0) && ((role == Qt::EditRole) || (role
				== Qt::DisplayRole))) {
			if ((index.row() < nOutputs)) {
				return QString::fromStdString(relatedScene->getFsm()->getOutputName(index.row()));
			}
		}

	}

	return QVariant();

}

QModelIndex OutputModel::index(int row, int column, const QModelIndex& parent) const {

	int id = row * 2 + column;

	if (!parent.isValid()) {
		return createIndex(row, column, id);
	}

	return createIndex(row, column, 100);
}

QModelIndex OutputModel::parent(const QModelIndex& index) const {

	if (!index.isValid() || index.internalId() < 1000) {
		return QModelIndex();
	}
	return createIndex(index.row(), index.column(), 1000);
}

bool OutputModel::setData(const QModelIndex& index, const QVariant& value,
		int role) {


		char cv;
		char cs[255];


		// Change output value of state
		if ((index.column() == 1) && (editingItemType == FSMDesigner::STATE)) {
			QVariant vv = value;

			int val = vv.toInt();
			if ((val == 0) || (val == 1)) {
				sprintf(cs, "%d", val);
				cv = cs[0];
				static_cast<StateItem*>(editingItem)->getModel()->setOutputAt(index.row(),cv);
			}

		}

		return true;


}

Qt::ItemFlags OutputModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flag = QAbstractItemModel::flags(index);
	if (isEditable(index)) {
		flag |= Qt::ItemIsEditable;
	}
	return flag;
}

bool OutputModel::isEditable(const QModelIndex& index) const {

	if (index.row() == -1) {
		return false;
	} else {
		return true;
	}
}

QModelIndex OutputModel::buddy(const QModelIndex& index) const {
	if (index.column() == 0) {
		return createIndex(index.row(), 1, index.internalPointer());
	}
	return index;
}

QVariant OutputModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation == Qt::Horizontal) {
		if (role != Qt::DisplayRole) {
			return QVariant();
		}
		switch (section) {
		case 0:
			return QString("Outputs");
		case 1:
			return QString("Output Bits");
		default:
			return QString();
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

void OutputModel::rowsAdded(int first, int last) {
	beginInsertRows(QModelIndex(), first, last);
	endInsertRows();
	layoutChanged();
}

void OutputModel::rowsChanged() {
	layoutChanged();
}
