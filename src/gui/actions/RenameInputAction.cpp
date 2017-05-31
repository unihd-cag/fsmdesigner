/**
 * \file RenameInputAction.cpp
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
#include <core/Fsm.h>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>


#include "RenameInputAction.h"

RenameInputAction::RenameInputAction(QString name,int position,Fsm * object,QUndoCommand * parentCommand,QWidget * relatedWidget) : ObjectFocusedAction<Fsm>(object,parentCommand,relatedWidget) {
    this->position = position;
    this->name = name;
}

RenameInputAction::~RenameInputAction() {

}

int RenameInputAction::id() {
    // Call parent
    return ObjectFocusedAction<Fsm>::id();
}
bool RenameInputAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Fsm>::mergeWith(command);
}
void RenameInputAction::redo(){

    //-- Hold name
    QString previousName = QString::fromStdString(this->object->getInputName(this->position));

    //-- Set name
    this->object->setInputName(this->position,this->name.toStdString());

    //-- Save Name
    this->name = previousName;


}
void RenameInputAction::undo(){

    //-- Hold name
    QString previousName = QString::fromStdString(this->object->getInputName(this->position));

    //-- Set name
    this->object->setInputName(this->position,this->name.toStdString());

    //-- Save Name
    this->name = previousName;

    //-- Update
    if (this->relatedWidget!=NULL)
        dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();


}


