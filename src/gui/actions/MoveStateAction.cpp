/**
 * \file MoveStateAction.cpp
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

#include "MoveStateAction.h"

MoveStateAction::MoveStateAction(QPointF position,StateItem * item,QUndoCommand * parentCommand) :ItemFocusedAction<StateItem>(item,parentCommand) {
    this->position = position;
}

MoveStateAction::~MoveStateAction() {

}

int MoveStateAction::id() {
    // Call parent
    return ItemFocusedAction<StateItem>::id();
}
bool MoveStateAction::mergeWith(const QUndoCommand * command) {

    // Call parent
    return ItemFocusedAction<StateItem>::mergeWith(command);
}
void MoveStateAction::redo(){


//    // If target position and actual position are the same, record target position for undo as the actual one in model
//    if (this->item->pos()==this->position) {
//        this->position = QPointF(this->item->getModel()->getPosition().first,this->item->getModel()->getPosition().second);
//    }
//    // Otherwise, redo position change
//    else {

        //-- Keep Position
        QPointF savedPosition = QPointF(this->item->getModel()->getPosition().first,this->item->getModel()->getPosition().second);

        //-- Set new to Model and notify if there was an actual change
        this->item->getModel()->setPosition(pair<double,double>(this->position.x(),this->position.y()));
        this->item->modelChanged();

        //-- Save position to class
        this->position = savedPosition;
//    }
}
void MoveStateAction::undo(){

    //-- Keep Position
    QPointF savedPosition = this->item->pos();

    //-- Set Old one to Model and notify to trigger the change back to UI
    this->item->getModel()->setPosition(pair<double,double>(this->position.x(),this->position.y()));
    this->item->modelChanged();

    //-- Save position to class
    this->position = savedPosition;


}


