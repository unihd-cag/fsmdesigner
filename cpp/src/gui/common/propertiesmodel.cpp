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
//----------------------

//-- Std
#include <sstream>
#include <iostream>
using namespace std;

//-- Core
#include <core/FSMDesigner.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/State.h>

#include <core/Trans.h>
#include <core/Condition.h>
#include <core/State.h>
#include <core/Link.h>
#include <core/Hypertrans.h>
#include <core/Trackpoint.h>


//-- Gui
#include <gui/scene/Scene.h>
#include <gui/scene/SceneRelatedObject.h>


#include <gui/items/StateItem.h>
#include <gui/items/Transline.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/HyperTransition.h>

#include <gui/common/ItemsRelatedEditor.h>
#include <gui/common/propertywidget.h>
#include <gui/common/propertiesdelegate.h>

//-- Undo
#include <gui/actions/ChangeStateNameAction.h>
#include <gui/actions/ChangeStateColorAction.h>
#include <gui/actions/ChangeStateOutputAction.h>
#include <gui/actions/ChangeTransitionNameAction.h>
#include <gui/actions/ChangeTransitionStartAction.h>
#include <gui/actions/ChangeTransitionEndAction.h>
#include <gui/actions/ChangeConditionNameAction.h>
#include <gui/actions/ChangeConditionValueAction.h>

//-- Qt
#include <QtGui>

#include "propertiesmodel.h"
PropertiesModel::PropertiesModel(QObject* parent) :
	QAbstractItemModel(parent), ItemsRelatedEditor() {

}

PropertiesModel::~PropertiesModel() {

}

int PropertiesModel::columnCount(const QModelIndex& index) const {

	Q_UNUSED(index);

	return 2;
}

int PropertiesModel::rowCount(const QModelIndex& parent) const {

	if (parent != QModelIndex() || this->editingItem == NULL || relatedScene == NULL
			|| relatedScene->getFsm() == NULL) {
		return 0;
	}

	Fsm& fsm = *(relatedScene->getFsm());
	int rows;

	switch (editingItemType) {

	case FSMDesigner::NONE: {
		rows = 0;
	}

	break;
	case FSMDesigner::STATE: {
		rows = 5; //State
	}
	break;
	case FSMDesigner::TRANS: {
		rows = 5; //Transition Transline
		Trans* trans = (Trans*) ((Transline*)editingItem)->getModel();
		FOREACH_TRANSITION_CONDITIONS(trans)
			rows = rows + 2;
		END_FOREACH_TRANSITION_CONDITIONS
		break;
	}
	case FSMDesigner::LINKDEPARTURE:
		//rows = 3; //Link without color
		rows = 4; //Link with color
		break;
	case FSMDesigner::HYPERTRANS:
		rows = 4; //HyperTransition

		//-- Add 2 rows per hypertransition condition
		FOREACH_HYPERTRANSITION_CONDITIONS(static_cast<HyperTransition*>(editingItem)->getModel())
		    rows = rows + 2;
		END_FOREACH_HYPERTRANSITION_CONDITIONS

		break;
	default:
		rows = 0;
		break;
	}

	return rows;
}

QVariant PropertiesModel::data(const QModelIndex& index, int role) const {

	Fsm& ff = *(relatedScene->getFsm());
	string s;
	QString qs;
	int ia;
	int i;
	stringstream ss;


	// Return Data Depending on the Edited Item
	//--------------------------
	bool isEditRole = role== Qt::EditRole;
	bool isDisplayRole = role== Qt::DisplayRole;
	bool isCheckRole = role== Qt::CheckStateRole;
	bool isEditOrDisplayRole = isEditRole | isDisplayRole;
	bool isEditOrCheckRole = isEditRole | isCheckRole;
	bool isEditOrDisplayOrCheckRole = isEditRole | isDisplayRole | isCheckRole;
	QVariant result;

	// State
	//----------------
	if (editingItemType == FSMDesigner::STATE) {

		//-- Get Model
		State * state = static_cast<StateItem*>(editingItem)->getModel();

		//---- Name Columns
		//--------------------
		if (index.column()==0 && isEditOrDisplayRole) {

			switch (index.row()) {
			case PropertiesDelegate::ROW_STATE_TYPE:
				return QString("Item");
				break;
			case PropertiesDelegate::ROW_STATE_NAME:
				return QString("Name");
				break;
			case PropertiesDelegate::ROW_STATE_RESET:
				return QString("Resetstate");
				break;
			case PropertiesDelegate::ROW_STATE_OUTPUT:
				return QString("Output");
				break;
			case PropertiesDelegate::ROW_STATE_COLOR:
				return QString("Color");
				break;
			default:
				break;
			}

		} // EOF Info column

		//---- Data Columns
		//--------------------------
		else if (index.column()==1 &&  isEditOrDisplayOrCheckRole) {

			// (Switch on rows, and on role in every result)
			if (index.row() == PropertiesDelegate::ROW_STATE_TYPE &&  isEditOrDisplayRole) {

				return QString("State");

			} else if (index.row() == PropertiesDelegate::ROW_STATE_NAME && isEditOrDisplayRole ) {

				return QString::fromStdString(state->getName());

			} else if (index.row() == PropertiesDelegate::ROW_STATE_RESET && isEditOrCheckRole ) {

				//-- We have a check role
				if (isCheckRole)
					return state->isReset() ? Qt::Checked : Qt::Unchecked;
				//-- Otherwise return normal boolean
				else
					return state->isReset();
			}

			else if (index.row() == PropertiesDelegate::ROW_STATE_OUTPUT &&  isEditOrDisplayRole ) {

				return QString::fromStdString(state->getOutput());

			} else if (index.row() == PropertiesDelegate::ROW_STATE_COLOR &&  isEditOrDisplayRole ) {

				return QVariant(QColor::fromRgb(state->getColor()));
			}

		} // EOF Data columns

	} // EOF State

	// Transition
	//-------------------
	else if (editingItemType == FSMDesigner::TRANS) {

		//-- Get Model
		Trans * trans = static_cast<Transline*>(editingItem)->getModel();

		//---- Name Columns
		//--------------------
		if (index.column()==0 && isEditOrDisplayRole) {

			switch (index.row()) {
			case PropertiesDelegate::ROW_TRANS_TYPE:
				return QString("Item");
				break;
			case PropertiesDelegate::ROW_TRANS_NAME:
				return QString("Name");
				break;
			case PropertiesDelegate::ROW_TRANS_DEFAULT:
				return QString("Default");
				break;
			case PropertiesDelegate::ROW_TRANS_START:
				return QString("Start");
				break;
			case PropertiesDelegate::ROW_TRANS_END:
				return QString("End");
				break;

			default:
				if (index.row() < rowCount(QModelIndex())) {
					if ((index.row() % 2) == 0) {
						return QString("CValue");
					} else {
						return QString("CName");
					}
				}
				break;
			}

		}
		//---- Data Columns
		//--------------------------
		else if (index.column()==1) {

			// (Switch on rows, and on role in every result)
			if (index.row() == PropertiesDelegate::ROW_TRANS_TYPE && isEditOrDisplayRole) {

				return QString("Transition");

			} else if (index.row() == PropertiesDelegate::ROW_TRANS_NAME && isEditOrDisplayRole) {

				return QString::fromStdString(trans->getName());

			} else if (index.row() == PropertiesDelegate::ROW_TRANS_DEFAULT && isEditOrCheckRole) {

				//-- We have a check role
				if (isCheckRole)
					return trans->isDefault() ? Qt::Checked : Qt::Unchecked;
				//-- Otherwise return normal boolean
				else
					return trans->isDefault();

			} else if (index.row() == PropertiesDelegate::ROW_TRANS_START && isEditOrDisplayRole) {

				return QString::fromStdString(trans->getStartState()->getName());

			} else if (index.row() == PropertiesDelegate::ROW_TRANS_END && isEditOrDisplayRole) {

				return QString::fromStdString(trans->getEndState()->getName());

			} else if (index.row() > PropertiesDelegate::ROW_TRANS_END && isEditOrDisplayRole) {

				//-- Remaining are Conditions
			    int conditionRowsCount = trans->getConditions().size()*2;
			    int remainingRows = this->rowCount()-index.row();
			    int beginPosition = conditionRowsCount-remainingRows;
			    int conditionId = (beginPosition - beginPosition%2) / 2;

			    // Name
			    if ((remainingRows % 2) == 0) {

			        return QString::fromStdString(trans->getConditionByID(conditionId)->getName());
                }
			    // Value
			    else {
			        return QString::fromStdString(trans->getConditionByID(conditionId)->getInput());
                }


			}
		} // EOF Data columns

	} // EOF Transition
	else if (editingItemType == FSMDesigner::LINKDEPARTURE) {

	    //-- Get Model
        Trackpoint * trackpoint = static_cast<LinkDeparture*>(editingItem)->getModel();

        //---- Name Columns
        //--------------------
        if (index.column()==0 && isEditOrDisplayRole) {

            switch (index.row()) {
            case 0:
                return QString("Item");
                break;
            case 1:
                return QString("ID");
                break;
            case 2:
                return QString("Target");
                break;
            case 3:
                return QString("Color");
                break;
            default:
                break;
            }

        }
        //---- Data Columns
        //--------------------------
        else if (index.column()==1) {




        }


	}

	//-- Return result
	return result;





	switch (index.column()) {
	// FIRST COLUMN
	//-------------------------
	case 0: {

		//---- Output depending on Item Type
		//----------------------------------
		switch (role) {
		case Qt::EditRole:
		case Qt::DisplayRole:
			switch (editingItemType) {

			//-- Link
			case FSMDesigner::LINKDEPARTURE: {
				switch (index.row()) {
				case 0:
					return QString("Item");
					break;
				case 1:
					return QString("ID");
					break;
				case 2:
					return QString("Target");
					break;
				case 3:
					return QString("Color");
					break;
				default:
					break;
				}
			}
			//-- HyperTransition
			case FSMDesigner::HYPERTRANS: {

				switch (index.row()) {
				case 0:
					return QString("Item");
					break;
				case 1:
					return QString("ID");
					break;
				case 2:
					return QString("Target");
					break;
				case 3:
					return QString("Type");
					break;
				default:
					if (index.row() < rowCount(QModelIndex())) {
						if ((index.row() % 2) == 0) {
							return QString("CName");
						} else {
							return QString("CValue");
						}
					}
					break;
				}
		} // EOF HTRANS
		default:
			break;
		} // EOF Item type switch

		break;
	} // EOF Role switch

	break;
	} // EOF First column


	// Second Column
	//------------------------
	case 1: {

		//---- Output depending on item type
		//------------------------------------
		switch (role) {

		case Qt::EditRole:
		case Qt::DisplayRole:
			switch (editingItemType) {


			//-- Link
			case FSMDesigner::LINKDEPARTURE: {

				switch (index.row()) {
					case 0: {
						return QString("Link");
						break;
					}
					case 1: {
						i = 0;
						ss << i;
						s = ss.str();
						qs = QString::fromStdString(s);
						return qs;
						break;
					}
					case 2: {
						switch (role) {
							case Qt::EditRole:
								i = static_cast<LinkDeparture*>(editingItem)->getModel()->getTargetLink();
								return i;
								break;
							case Qt::DisplayRole:
								i = static_cast<LinkDeparture*>(editingItem)->getModel()->getTargetLink();
								qs = QString::fromStdString(relatedScene->getFsm()->getStatebyID(i)->getName());
								return qs;
								break;
						}
						break;
					}
					case 3: {
						i = (int) static_cast<LinkDeparture*>(editingItem)->getModel()->getColor();
						QColor c = QColor::fromRgb((unsigned int) i);
						QVariant v = c;
						return v;
						break;
					}
					default: {
						break;
					}
				}
			} // EOF Link

			//-- Hypertrans
			case FSMDesigner::HYPERTRANS: {

				switch (index.row()) {
					case 0: {
						return QString("HyperTransition");
						break;
					}
					case 1: {
						i = static_cast<HyperTransition*>(editingItem)->getModel()->getId();
						ss << i;
						s = ss.str();
						qs = QString::fromStdString(s);
						return qs;
						break;
					}
					case 2: {
						switch (role) {
							case Qt::EditRole:
								return static_cast<HyperTransition*>(editingItem)->getModel()->getTargetState()->getId();
								break;
							case Qt::DisplayRole:
								return QString::fromStdString(static_cast<HyperTransition*>(editingItem)->getModel()->getTargetState()->getName());
								break;
						}
					}
					case 3: {
						if (static_cast<HyperTransition*>(editingItem)->getModel()->getType()) {
							qs = "all_states";
						} else {
							qs = "rest_states";
						}
						return qs;
						break;
					}
					default: {
						if (index.row() < rowCount(QModelIndex())) {
						    // FIXME
//							if (relatedScene->getFsm()->getHypertransFirstCondition()) {
//								int c = index.row();
//								c = c - 5;
//								while (c > 0) {
//									relatedScene->getFsm()->getHypertransNextCondition();
//									c = c - 2;
//								}
//							}
//							if ((index.row() % 2) == 0) {
//								s = relatedScene->getFsm()->getHypertransConditionName();
//								qs = QString::fromStdString(s);
//								return qs;
//							} else {
//								s = relatedScene->getFsm()->getHypertransConditionValue();
//								qs = QString::fromStdString(s);
//								return qs;
//							}
						}
						break;
					}
				}
			} // EOF HTrans

		}// EOF Item type switch
		default:
			break;
		} // EOF Role switch

		break;

	}// EOF Second column

	default:
		break;

	}// EOF Column switch

	return QVariant();

}

QModelIndex PropertiesModel::index(int row, int column,
		const QModelIndex& parent) const {

	int id = row * 2 + column;

	if (!parent.isValid()) {
		return createIndex(row, column, id);
	}

	return createIndex(row, column, 100);
}

QModelIndex PropertiesModel::parent(const QModelIndex& index) const {

	if (!index.isValid() || index.internalId() < 100) {
		return QModelIndex();
	}
	return createIndex(index.row(), index.column(), 100);
}

bool PropertiesModel::setData(const QModelIndex& index, const QVariant& value,
		int role) {

	string s;
	int i;
	int level;
	int posi;
	int rowhelp;
	int rowcnt;

	// Only work on Column 1
	//-------------
	if (index.column()!=1)
		return false;

	//-- Result
	bool result = true;


	switch (editingItemType) {

	// State
	//-------------------------
	case FSMDesigner::STATE: {

		switch (index.row()) {

		//-- Name
		case 1: {


			//-- Update model
			this->getRelatedScene()->getUndoStack()->push(new ChangeStateNameAction(value.toString().trimmed(),static_cast<StateItem*>(editingItem)));

			break;
		}

		//-- Reset State
		case 2:{

			//-- Only update if it is set to true
			if (value.toBool() == true) {
			    relatedScene->getFsm()->setResetState(static_cast<StateItem*>(editingItem)->getModel()->getId());
				return true;
			}
			break;
		}

		//-- Output
		case 3: {
		    relatedScene->getUndoStack()->push(new ChangeStateOutputAction(value.toString().trimmed(),static_cast<StateItem*>(editingItem)));
			break;
		}

		//-- Color
		case 4: {
		    relatedScene->getUndoStack()->push(new ChangeStateColorAction(value.value<QColor> (),static_cast<StateItem*>(editingItem)));
			break;
		}

		//-- Nothing
		default:
			break;
		}
		break;

	} //-- EOF State

	// Transition
	//---------------------------
	case FSMDesigner::TRANS:
	{
		Trans * trans = static_cast<Transline*>(editingItem)->getModel();

		switch (index.row()) {
		case PropertiesDelegate::ROW_TRANS_TYPE:
			break;
		case PropertiesDelegate::ROW_TRANS_NAME: {


		    this->getRelatedScene()->getUndoStack()->push(new ChangeTransitionNameAction(value.toString().trimmed(),static_cast<Transline*>(editingItem)));

			break;
		}
		case PropertiesDelegate::ROW_TRANS_DEFAULT: {

		    //-- Only do is there is a change
		    if (trans->isDefault() != value.toBool()) {

		        //-- FIXME Use an action
		        trans->setDefault(value.toBool());
		    }


			break;
		}

		//-- Change transition start
		case PropertiesDelegate::ROW_TRANS_START: {

		    //-- Check there is really a change
		    State * targetState = relatedScene->getFsm()->getStatebyID(value.toInt());
		    if (trans->getStartState()!=targetState) {
		        ChangeTransitionStartAction * changeTransitionStart= new ChangeTransitionStartAction(relatedScene->getFsm()->getStatebyID(value.toInt()),static_cast<Transline*>(editingItem));
		        this->getRelatedScene()->getUndoStack()->push(changeTransitionStart);

		    }

		}
			break;
		case PropertiesDelegate::ROW_TRANS_END: {

		    //-- Check there is really a change
            State * targetState = relatedScene->getFsm()->getStatebyID(value.toInt());
            if (trans->getStartState()!=targetState) {
                ChangeTransitionEndAction * changeTransitionEnd= new ChangeTransitionEndAction(relatedScene->getFsm()->getStatebyID(value.toInt()),static_cast<Transline*>(editingItem));
                this->getRelatedScene()->getUndoStack()->push(changeTransitionEnd);
            }

		    break;
		}

		//-- Remaining are conditions
		default:

		    //-- Remaining are Conditions
            int conditionRowsCount = trans->getConditions().size()*2;
            int remainingRows = this->rowCount()-index.row();
            int beginPosition = conditionRowsCount-remainingRows;
            int conditionId = (beginPosition - beginPosition%2) / 2;

            //-- Name
            if ((remainingRows % 2) == 0) {

                ChangeConditionNameAction * changeName = new ChangeConditionNameAction(value.toString(),trans->getConditionByID(conditionId));
                getRelatedScene()->getUndoStack()->push(changeName);

                return true;

            }
            //-- Value
            else {

                ChangeConditionValueAction * changeValue = new ChangeConditionValueAction(value.toString(),trans->getConditionByID(conditionId));
                getRelatedScene()->getUndoStack()->push(changeValue);

                return true;
            }

            return false;

			break;
		}
	}

		break;
	case FSMDesigner::LINKDEPARTURE: // Link

		switch (index.row()) {
		case 2:


			s = value.toString().trimmed().toStdString();
			FOREACH_STATE(relatedScene->getFsm())
					if (strcmp(s.c_str(), state->getName().c_str()) == 0) {
					    static_cast<LinkDeparture*>(editingItem)->getModel()->setTargetLink(state->getId());
					}
			END_FOREACH_STATE

			break;

		case 3: {
			unsigned int color = value.value<QColor> ().rgb();
			static_cast<LinkDeparture*>(editingItem)->getModel()->setColor(color);
		}
			break;
		}
		break;
	case FSMDesigner::HYPERTRANS: // HyperTransition

		switch (index.row()) {
		case 0:
			break;
		case 1:
			break;
		case 2: // Target state

			s = value.toString().trimmed().toStdString();
			FOREACH_STATE(relatedScene->getFsm())
					if (strcmp(s.c_str(), state->getName().c_str()) == 0) {

					    static_cast<HyperTransition*>(editingItem)->getModel()->setTargetState(state);


					}
			END_FOREACH_STATE
			break;
		case 3:
			break;
		default: // Conditions
		    /// FIXME
			/*relatedScene->getFsm()->getHypertransbyID(((Hypertrans*) editingItem)->hid);
			if (index.row() < rowCount(QModelIndex())) {
				if (relatedScene->getFsm()->getHypertransFirstCondition()) {
					int c = index.row();
					c = c - 5;
					while (c > 0) {
						relatedScene->getFsm()->getHypertransNextCondition();
						c = c - 2;
					}
				}
				int cid = relatedScene->getFsm()->getHypertransConditionID();
				if ((index.row() % 2) == 0) {
					relatedScene->getFsm()->modifyHypertrans(((Hypertrans*) editingItem)->hid,
							cid, value.toString().trimmed().toStdString());
				} else {
					relatedScene->getFsm()->modifyHypertransConditionValue(
							((Hypertrans*) editingItem)->hid, cid,
							value.toString().trimmed().toStdString());
				}
			}*/

			break;
		}
		break;
	default:
		break;
	}

	//-- Return and emit signal


	return result;
}

Qt::ItemFlags PropertiesModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flag = QAbstractItemModel::flags(index);
	if (isEditable(index)) {
		flag |= Qt::ItemIsEditable;
	}
	return flag;
}

bool PropertiesModel::isEditable(const QModelIndex& index) const {
	if (editingItemType == FSMDesigner::LINKDEPARTURE || editingItemType
			== FSMDesigner::HYPERTRANS) { // Link + HyperTransition
		if (index.column() == 1 && index.row() == 1) {
			return false;
		}
	}
	if (editingItemType == FSMDesigner::HYPERTRANS && index.column() == 1
			&& index.row() == 3) { // HyperTransition
		return false;
	}
	return index.column() == 1 && !(index.row() == 0);
}

QModelIndex PropertiesModel::buddy(const QModelIndex& index) const {
	if (index.column() == 0) {
		return createIndex(index.row(), 1, index.internalPointer());
	}
	return index;
}

QVariant PropertiesModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation == Qt::Horizontal) {
		if (role != Qt::DisplayRole) {
			return QVariant();
		}
		switch (section) {
		case 0:
			return QString("Property");
		case 1:
			return QString("Value");
		default:
			return QString();
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

void PropertiesModel::setRelatedScene(Scene * scene) {

	//-- Set Scene
	SceneRelatedObject::setRelatedScene(scene);



}

void PropertiesModel::selectionUpdated() {

}
