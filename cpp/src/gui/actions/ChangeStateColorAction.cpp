/**
 * \file ChangeStateColorAction.cpp
 *
 * 
 *
 */

// Includes
//--------------------

//-- Std
#include <iostream>
#include <ostream>
#include <sstream>
using namespace std;


//-- Qt
#include <QtGui>
#include <QtCore>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>


//-- Gui Items
#include <gui/items/StateItem.h>

#include "ChangeStateColorAction.h"

ChangeStateColorAction::ChangeStateColorAction(QColor color,StateItem * item) : ItemFocusedAction<StateItem>(item) {
    this->color = color;
}

ChangeStateColorAction::~ChangeStateColorAction() {

}

int ChangeStateColorAction::id() {
    // Call parent
    //-----------------
    return ItemFocusedAction<StateItem>::id();
}
bool ChangeStateColorAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    //------------------
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void ChangeStateColorAction::redo(){

    // Call parent
    //------------------
    ItemFocusedAction<StateItem>::redo();

    //-- Save actual value
    QColor oldColor = QColor::fromRgb(this->item->getModel()->getColor());

    //-- Change
    this->item->getModel()->setColor(this->color.rgb());
    this->item->modelChanged();

    //-- Save global output to old Value
    this->color = oldColor;

}
void ChangeStateColorAction::undo(){

    // Call parent
    //----------------
    ItemFocusedAction<StateItem>::undo();

    //-- Save actual value
    QColor oldColor = QColor::fromRgb(this->item->getModel()->getColor());

    //-- Change
    this->item->getModel()->setColor(this->color.rgb());
    this->item->modelChanged();

    //-- Save global output to old Value
    this->color = oldColor;
}


