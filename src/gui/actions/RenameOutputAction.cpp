/**
 * \file RenameOutputAction.cpp
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

#include "RenameOutputAction.h"

RenameOutputAction::RenameOutputAction(QString name, int position, Fsm * object,
        QUndoCommand * parentCommand, QWidget * relatedWidget) :
        ObjectFocusedAction<Fsm>(object, parentCommand, relatedWidget) {
    this->position = position;
    this->name = name;
}

RenameOutputAction::~RenameOutputAction() {

}

int RenameOutputAction::id() {
    // Call parent
    return ObjectFocusedAction<Fsm>::id();
}
bool RenameOutputAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ObjectFocusedAction<Fsm>::mergeWith(command);
}
void RenameOutputAction::redo() {

    //-- Hold name
    QString previousName = QString::fromStdString(this->object->getOutputName(this->position));

    //-- Set name
    this->object->setOutputName(this->position,this->name.toStdString());

    //-- Save Name
    this->name = previousName;

    //-- Update
    //dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();

}


void RenameOutputAction::undo() {

    //-- Hold name
    QString previousName = QString::fromStdString(this->object->getOutputName(this->position));

    //-- Set name
    this->object->setOutputName(this->position,this->name.toStdString());

    //-- Save Name
    this->name = previousName;

    //-- Update
    if (this->relatedWidget!=NULL)
        dynamic_cast<QTreeView*>(this->relatedWidget)->doItemsLayout();
}

