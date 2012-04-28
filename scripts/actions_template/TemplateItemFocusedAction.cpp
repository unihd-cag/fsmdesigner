/**
 * \file <%= puts $eout $::actionName %>.cpp
 *
 * <%= regexp {(?:[a-zA-Z]+)<(.+)>} $::actionBaseClass -> itemClass %>
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
#include <gui/items/<%= puts $eout $itemClass %>.h>

#include "<%= puts $eout $::actionName %>.h"

<%= puts $eout $::actionName %>::<%= puts $eout $::actionName %>(<%= puts $eout $itemClass %> * item,QUndoCommand * parentCommand) :ItemFocusedAction<<%= puts $eout $itemClass %>>(item,parentCommand) {

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


