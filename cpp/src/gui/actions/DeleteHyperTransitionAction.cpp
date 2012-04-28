/**
 * \file DeleteHyperTransitionAction.cpp
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
#include <gui/items/HyperTransition.h>

#include "DeleteHyperTransitionAction.h"

DeleteHyperTransitionAction::DeleteHyperTransitionAction(HyperTransition * item,QUndoCommand * parentCommand) :ItemFocusedAction<HyperTransition>(item,parentCommand) {

}

DeleteHyperTransitionAction::~DeleteHyperTransitionAction() {

}

int DeleteHyperTransitionAction::id() {
    // Call parent
    return ItemFocusedAction<HyperTransition>::id();
}
bool DeleteHyperTransitionAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return ItemFocusedAction<HyperTransition>::mergeWith(command);
}
void DeleteHyperTransitionAction::redo(){
    // Call parent
    ItemFocusedAction<HyperTransition>::redo();
}
void DeleteHyperTransitionAction::undo(){
    // Call parent
    ItemFocusedAction<HyperTransition>::undo();
}


