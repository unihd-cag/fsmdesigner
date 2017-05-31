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
//--------------

//-- Std
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Trans.h>

//-- Gui
#include <gui/common/filterkeywords.h>
#include <gui/common/ItemsRelatedEditor.h>

#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/Transline.h>
#include <gui/items/StateItem.h>
#include <gui/items/LinkDeparture.h>

#include <gui/common/propertiesmodel.h>
#include <gui/common/propertywidget.h>

#include <gui/common/ColorButton.h>
#include <gui/state/StatesComboBox.h>


#include "propertiesdelegate.h"

PropertiesDelegate::PropertiesDelegate(QObject* parent) : QStyledItemDelegate(parent), colorEdited(false),ItemsRelatedEditor()  {

}

PropertiesDelegate::~PropertiesDelegate() {

}



void PropertiesDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {

    //  Color Row and column for State and Link
    //--------------------
    if ( (this->editingItemType ==FSMDesigner::STATE ||this->editingItemType ==FSMDesigner::LINKDEPARTURE) && index.column() == 1 && (index.row()==ROW_STATE_COLOR || index.row() == ROW_LINK_COLOR)) {

        QColor color = index.data().value<QColor>();
        QSize size = this->sizeHint(option,index);

        //-- Paint and keep a border to cell
        painter->save();
        painter->translate(option.rect.x(),option.rect.y());
        //painter->fillRect(option.rect,0,size.width(),size.height(),color);
        //painter->scale(0.8,0.8);
        painter->fillRect(0+2,0+2,option.rect.width()-2,option.rect.height()-2,color);

        painter->restore();
        //QStyledItemDelegate::paint(painter,option,index);
    }
    // Standard Otherwise
    //-----------------------
    else {

        QStyledItemDelegate::paint(painter,option,index);

    }

}


QSize PropertiesDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {  
    return QStyledItemDelegate::sizeHint(option, index);
}





QWidget* PropertiesDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)  const {

	// State
	//-----------------------
	if (editingItemType==FSMDesigner::STATE) {

		//-- Reset
		if (index.row() == ROW_STATE_RESET) {
			return QStyledItemDelegate::createEditor(parent, option, index);
		}
		//-- Color
		else if (index.row()==ROW_STATE_COLOR) {

		    return new QColorDialog(index.data().value<QColor>());
		}

	}

	// Transition
	//---------------------------
	else if (editingItemType==FSMDesigner::TRANS) {

		//-- Start or END
		if (index.row() == ROW_TRANS_START || index.row() == ROW_TRANS_END) {
			StatesComboBox* w = new StatesComboBox(relatedScene->getFsm(),true,NULL,parent);
			return w;
		}
		//-- Default
		else if (index.row() == ROW_TRANS_DEFAULT) {
			return QStyledItemDelegate::createEditor(parent, option, index);
		}

	}


	// Link
	//--------------
	else if (editingItemType==FSMDesigner::LINKDEPARTURE) {

		//-- Link
		if (index.row()==ROW_LINK_COLOR) {
			ColorButton *b=new ColorButton(QColor(),parent);
			return b;
		}
		//-- Target
		else if (index.row()==ROW_LINK_TARGET) {
			StatesComboBox* w=new StatesComboBox(relatedScene->getFsm(),true,NULL,parent);
			w->installEventFilter(const_cast<PropertiesDelegate*>(this));
			return w;
		}

	}

	// HTRANS
	//------------------
	else if (editingItemType==FSMDesigner::HYPERTRANS) {

		//-- Target
		if (index.row()==ROW_HTRANS_END) {
			StatesComboBox* w=new StatesComboBox(relatedScene->getFsm(),true,NULL,parent);
			return w;
		}
	}

	// Other Rows, color alternate and use a normal default editor
	//-----------------------------------
	QWidget* w = QStyledItemDelegate::createEditor(parent, option, index);
	return w;

}


void PropertiesDelegate::setEditorData(QWidget* editor,const QModelIndex& index) const {

	// State
	//-----------------------
	if (editingItemType==FSMDesigner::STATE) {

		//-- Reset
		if (index.row() == ROW_STATE_RESET) {
			QStyledItemDelegate::setEditorData(editor, index);
		}
		//-- Color
		else if (index.row()==ROW_STATE_COLOR) {

		    static_cast<QColorDialog*>(editor)->setCurrentColor(index.data().value<QColor>());

		} else {
			QStyledItemDelegate::setEditorData(editor, index);
		}

	}

	// Transition
	//---------------------------
	else if (editingItemType==FSMDesigner::TRANS) {

		//-- Start or END
		if (index.row() == ROW_TRANS_START || index.row() == ROW_TRANS_END) {
			StatesComboBox *box=static_cast<StatesComboBox*>(editor);
			int id = index.model()->data(index,Qt::EditRole).toInt();
			int idx=box->findData ( id);
			box->setCurrentIndex(idx);
		}
		//-- Default
		else if (index.row() == ROW_TRANS_DEFAULT) {
			QStyledItemDelegate::setEditorData(editor, index);
		} else {
			QStyledItemDelegate::setEditorData(editor, index);
		 }

	}


	// Link
	//--------------
	else if (editingItemType==FSMDesigner::LINKDEPARTURE) {

		//-- Link
		if (index.row()==ROW_LINK_COLOR) {
			unsigned int rgb = (index.model()->data(index,Qt::EditRole)).value<QColor>().rgb();
			ColorButton* b=static_cast<ColorButton*>(editor);
			b->setColor(rgb);
		}
		//-- Target
		else if (index.row()==ROW_LINK_TARGET) {
			StatesComboBox *box=static_cast<StatesComboBox*>(editor);
			int id = index.model()->data(index,Qt::EditRole).toInt();
			int idx=box->findData ( id);
			box->setCurrentIndex(idx);
		} else {
			QStyledItemDelegate::setEditorData(editor, index);
		}

	}

	// HTRANS
	//------------------
	else if (editingItemType==FSMDesigner::HYPERTRANS) {

		//-- Target
		if (index.row()==ROW_HTRANS_END) {
			StatesComboBox *box=static_cast<StatesComboBox*>(editor);
			int id = index.model()->data(index,Qt::EditRole).toInt();
			int idx=box->findData ( id);
			box->setCurrentIndex(idx);
		} else {
			QStyledItemDelegate::setEditorData(editor, index);
		}
	}

	// Other Rows, color alternate and use a normal default editor
	//-----------------------------------
	else {
		QStyledItemDelegate::setEditorData(editor, index);
	}

}


void PropertiesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {


	// State
	//-----------------------
	if (editingItemType==FSMDesigner::STATE) {

		//-- Reset
		if (index.row() == ROW_STATE_RESET) {

			//-- Get result
			bool reset = static_cast<QComboBox*> (editor)->currentIndex() == 0 ? false : true;

			//-- If there is anoter default StateItem, set it as not default in the UI
			State * actualReset = relatedScene->getFsm()->getStatebyID(relatedScene->getFsm()->getResetState());
			if (actualReset!=NULL && reset) {
			    actualReset->setReset(false);
			    relatedScene->findStateItem(actualReset)->modelChanged();
			}


			//-- Update Model
			const_cast<QAbstractItemModel*>(index.model())->setData(index,reset,Qt::EditRole);


		}
		//-- Color
		else if (index.row()==ROW_STATE_COLOR) {

			//-- Update model
		    const_cast<QAbstractItemModel*>(index.model())->setData(index,
		                    static_cast<QColorDialog*>(editor)->currentColor(), Qt::EditRole);

		} else {
			QStyledItemDelegate::setModelData(editor, model, index);
		}

		//-- Update UI
		static_cast<StateItem*>(editingItem)->modelChanged();



	}

	// Transition
	//---------------------------
	else if (editingItemType==FSMDesigner::TRANS) {

		//-- Start or END
		if (index.row() == ROW_TRANS_START || index.row() == ROW_TRANS_END) {

			//-- Update Model
			StatesComboBox *box=static_cast<StatesComboBox*>(editor);
			if (box->getSelectedState()!=NULL)
			    bool updated = const_cast<QAbstractItemModel*>(index.model())->setData(index,box->getSelectedState()->getId(),Qt::EditRole);


		}
		//-- Default
		else if (index.row() == ROW_TRANS_DEFAULT) {

			//-- Get result
			bool reset = static_cast<QComboBox*> (editor)->currentIndex() == 0 ? false : true;

			//-- Update model
			const_cast<QAbstractItemModel*>(index.model())->setData(index,reset,Qt::EditRole);

			//-- Update UI
			QList<Transline *> translines = relatedScene->findTransline((Trans*)static_cast<Transline*>(editingItem)->getModel());
			for (QList<Transline *>::iterator it=translines.begin();it!=translines.end();it++)
				(*it)->preparePath();


		} else {
			QStyledItemDelegate::setModelData(editor, model, index);
		}

	}

	// HTRANS
    //------------------
    else if (editingItemType==FSMDesigner::HYPERTRANS) {

        //-- Target
        if (index.row()==ROW_HTRANS_END) {

            //-- Update Model
            StatesComboBox *box=static_cast<StatesComboBox*>(editor);
            if (box->getSelectedState()!=NULL)
                bool updated = const_cast<QAbstractItemModel*>(index.model())->setData(index,box->getSelectedState()->getId(),Qt::EditRole);

        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

	// Link
	//--------------
	else if (editingItemType==FSMDesigner::LINKDEPARTURE) {

		//-- Link
		if (index.row()==ROW_LINK_COLOR) {
			ColorButton *b=static_cast<ColorButton*> (editor);
			const_cast<QAbstractItemModel*>(index.model())->setData(index,b->getColor(),Qt::EditRole);
		}
		//-- Target
		else if (index.row()==ROW_LINK_TARGET) {

			StatesComboBox *box=static_cast<StatesComboBox*>(editor);
			QString qs = box->currentText();
			FilterKeywords* fk;
			fk = new FilterKeywords();
			qs = fk->RemoveBlanks(qs);
			qs = fk->FilterSpecialChars(qs);
			qs = fk->FilterHDLKeywords(qs);
			qs = fk->FilterBusChars(qs);
			const_cast<QAbstractItemModel*>(index.model())->setData(index,qs,Qt::EditRole);

		} else {
			QStyledItemDelegate::setModelData(editor, model, index);
		}

	}



	// Other Rows, color alternate and use a normal default editor
	//-----------------------------------
	else {
		 QStyledItemDelegate::setModelData(editor, model, index);
	}



}


void PropertiesDelegate::setRelatedScene(Scene * scene) {

	//-- Set Scene
	SceneRelatedObject::setRelatedScene(scene);

}



void PropertiesDelegate::selectionUpdated() {

}
