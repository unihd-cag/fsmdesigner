/**
 * \file ChangeStateNameAction.cpp
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

#include "ChangeStateNameAction.h"

ChangeStateNameAction::ChangeStateNameAction(QString name,StateItem * item,QUndoCommand * parentCommand) :ItemFocusedAction<StateItem>(item,parentCommand) {
    this->name = name;
}

ChangeStateNameAction::~ChangeStateNameAction() {

}

int ChangeStateNameAction::id() {
    // Call parent
    return ItemFocusedAction<StateItem>::id();
}
bool ChangeStateNameAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void ChangeStateNameAction::redo(){

    //-- Save Old name
    QString oldName = QString::fromStdString(this->item->getModel()->getName());

    //-- Change Name to new name
    this->item->getModel()->setName(this->name.toStdString());

    //-- Save old name to class field for undo
    this->name = oldName;

    //-- Notify
    this->item->modelChanged();



}
void ChangeStateNameAction::undo(){

    //-- Save name in model
    QString newName = QString::fromStdString(this->item->getModel()->getName());

    //-- Restore old name from class field
    this->item->getModel()->setName(this->name.toStdString());

    //-- Save new name from model into class field for redo
    this->name = newName;

    //-- Notify
    this->item->modelChanged();
}


