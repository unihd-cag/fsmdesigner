/**
 * \file ChangeStateOutputAction.cpp
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

#include "ChangeStateOutputAction.h"

ChangeStateOutputAction::ChangeStateOutputAction(QString output,StateItem * item) :ItemFocusedAction(item) {
    this->output = output;
}

ChangeStateOutputAction::~ChangeStateOutputAction() {

}

int ChangeStateOutputAction::id() {

    return this->item->getModel()->getId();
}
bool ChangeStateOutputAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void ChangeStateOutputAction::redo(){

    // Call parent
    //-------------------
    ItemFocusedAction<StateItem>::redo();


    //-- Save actual value
    QString oldOutput = QString::fromStdString(this->item->getModel()->getOutput());

    //-- Change
    this->item->getModel()->setOutput(this->output.toStdString());
    this->item->modelChanged();

    //-- Save global output to old Value
    this->output = oldOutput;

}
void ChangeStateOutputAction::undo(){

    // Call parent
    //-------------------
    ItemFocusedAction<StateItem>::undo();


    //-- Save actual value
    QString oldOutput = QString::fromStdString(this->item->getModel()->getOutput());

    //-- Change
    this->item->getModel()->setOutput(this->output.toStdString());
    this->item->modelChanged();

    //-- Save global output to old Value
    this->output = oldOutput;


}


