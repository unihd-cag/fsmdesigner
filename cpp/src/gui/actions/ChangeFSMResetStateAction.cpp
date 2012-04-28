/**
 * \file ChangeFSMResetStateAction.cpp
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
#include <gui/actions/FSMAction.h>

#include "ChangeFSMResetStateAction.h"

ChangeFSMResetStateAction::ChangeFSMResetStateAction(Scene * scene) : FSMAction(scene) {

}

ChangeFSMResetStateAction::~ChangeFSMResetStateAction() {

}

int ChangeFSMResetStateAction::id() {
    // Call parent
    return FSMAction::id();
}
bool ChangeFSMResetStateAction::mergeWith(const QUndoCommand * command) {
    // Call parent
    return FSMAction::mergeWith(command);
}
void ChangeFSMResetStateAction::redo(){
    // Call parent
    FSMAction::redo();
}
void ChangeFSMResetStateAction::undo(){
    // Call parent
    FSMAction::undo();
}


