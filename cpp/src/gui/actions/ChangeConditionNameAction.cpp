/**
 * \file ChangeConditionNameAction.cpp
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
#include <gui/actions/ObjectFocusedAction.h>


//-- Gui Items
#include <core/Condition.h>

#include "ChangeConditionNameAction.h"

ChangeConditionNameAction::ChangeConditionNameAction(QString name,Condition * item,QUndoCommand * parentCommand) :ObjectFocusedAction<Condition>(item,parentCommand) {
    this->name = name;
}

ChangeConditionNameAction::~ChangeConditionNameAction() {

}

int ChangeConditionNameAction::id() {
    // Call parent
    return ObjectFocusedAction<Condition>::id();
}
bool ChangeConditionNameAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Condition>::mergeWith(command);
}
void ChangeConditionNameAction::redo(){


    // Call parent
    ObjectFocusedAction<Condition>::redo();

    //-- Save Old name
    QString oldName = QString::fromStdString(this->object->getName());

    //-- Change Name to new name
    this->object->setName(this->name.toStdString());

    //-- Save old name to class field for undo
    this->name = oldName;



}
void ChangeConditionNameAction::undo(){



    //-- Save Old name
    QString oldName = QString::fromStdString(this->object->getName());

    //-- Change Name to new name
    this->object->setName(this->name.toStdString());

    //-- Save old name to class field for undo
    this->name = oldName;


    // Call parent
    ObjectFocusedAction<Condition>::undo();
}


