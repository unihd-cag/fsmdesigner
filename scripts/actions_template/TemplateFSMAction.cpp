/**
 * \file <%= puts $eout $::actionName %>.cpp
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

#include "<%= puts $eout $::actionName %>.h"

<%= puts $eout $::actionName %>::<%= puts $eout $::actionName %>(Scene * scene,QUndoCommand * parentCommand) : FSMAction(scene,parentCommand) {

}

<%= puts $eout $::actionName %>::~<%= puts $eout $::actionName %>() {

}

int <%= puts $eout $::actionName %>::id() {
    // Call parent
    <%= puts $eout "return ${::actionBaseClass}::id();" %>
}
bool <%= puts $eout $::actionName %>::mergeWith(const QUndoCommand * command) {
    // Call parent
    <%= puts $eout "return ${::actionBaseClass}::mergeWith(command);" %>
}
void <%= puts $eout $::actionName %>::redo(){
    // Call parent
    <%= puts $eout "${::actionBaseClass}::redo();" %>
}
void <%= puts $eout $::actionName %>::undo(){
    // Call parent
    <%= puts $eout "${::actionBaseClass}::undo();" %>
}


