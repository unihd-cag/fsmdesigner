/**
 * \file <%= puts $eout $::actionName %>.h
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
#include <gui/actions/FSMAction.h>

/** Description Here
 *
 */
class <%= puts $eout $::actionName %> : public FSMAction {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        <%= puts $eout $::actionName %>(Scene *,QUndoCommand * parentCommand = NULL);

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
