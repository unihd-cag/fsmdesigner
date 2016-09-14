/**
 * \file ChangeTransitionNameAction.cpp
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

//-- Core
#include <core/Trans.h>

//-- Gui
#include <gui/items/Transline.h>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>


#include "ChangeTransitionNameAction.h"

ChangeTransitionNameAction::ChangeTransitionNameAction(QString name,Transline * item,QUndoCommand * parentCommand) :
                ItemFocusedAction<Transline>(item,parentCommand) {
    this->name = name;
}


ChangeTransitionNameAction::~ChangeTransitionNameAction() {

}

int ChangeTransitionNameAction::id() {
    // Call parent
    return ItemFocusedAction<Transline>::id();
}
bool ChangeTransitionNameAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<Transline>::mergeWith(command);
}
void ChangeTransitionNameAction::redo(){

    // Call parent
    ItemFocusedAction<Transline>::redo();

    //-- Save name in model
    QString newName = QString::fromStdString(this->item->getModel()->getName());

    //-- Restore old name from class field
    this->item->getModel()->setName(this->name.toStdString());

    //-- Save new name from model into class field for redo
    this->name = newName;

    //-- Notify
    this->item->modelChanged();

}
void ChangeTransitionNameAction::undo(){

   //-- Save name in model
   QString newName = QString::fromStdString(this->item->getModel()->getName());

   //-- Restore old name from class field
   this->item->getModel()->setName(this->name.toStdString());

   //-- Save new name from model into class field for redo
   this->name = newName;

   //-- Notify
   this->item->modelChanged();

   // Call parent
   ItemFocusedAction<Transline>::undo();

}


