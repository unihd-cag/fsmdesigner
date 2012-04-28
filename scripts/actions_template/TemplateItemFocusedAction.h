/**
 * \file <%= puts $eout $::actionName %>.h
 *
 * <%= regexp {(?:[a-zA-Z]+)<(.+)>} $::actionBaseClass -> itemClass %>
 *
 */

#ifndef <% string toupper $::actionName %>_H_
#define <% string toupper $::actionName %>_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class <%= puts $eout $itemClass %>;

/** Description Here
 *
 */
class <%= puts $eout $::actionName %> : public <%= puts $eout $::actionBaseClass %> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        <%= puts $eout $::actionName %>(<%= puts $eout $itemClass %> *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~<%= puts $eout $::actionName %>();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
