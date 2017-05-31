/**
 * \file ChangeConditionValueAction.cpp
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

#include "ChangeConditionValueAction.h"

ChangeConditionValueAction::ChangeConditionValueAction(QString value,
        Condition * item, QUndoCommand * parentCommand) :
        ObjectFocusedAction<Condition>(item, parentCommand) {
    this->value = value;
}

ChangeConditionValueAction::~ChangeConditionValueAction() {

}

int ChangeConditionValueAction::id() {
    // Call parent
    return ObjectFocusedAction<Condition>::id();
}
bool ChangeConditionValueAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Condition>::mergeWith(command);
}
void ChangeConditionValueAction::redo() {

    // Call parent
    ObjectFocusedAction<Condition>::redo();

    //-- Save Old Value
    QString oldValue = QString::fromStdString(this->object->getInput());

    //-- Change value to new value
    this->object->setInput(this->value.toStdString());

    //-- Save old value to class field for undo
    this->value = oldValue;

}
void ChangeConditionValueAction::undo() {

    //-- Save Old Value
    QString oldValue = QString::fromStdString(this->object->getInput());

    //-- Change value to new value
    this->object->setInput(this->value.toStdString());

    //-- Save old value to class field for undo
    this->value = oldValue;

    // Call parent
    ObjectFocusedAction<Condition>::undo();
}

