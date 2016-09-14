/**
 * \file DeleteTransitionAction.h
 *
 * 
 *
 */

#ifndef DELETETRANSITIONACTION_H_
#define DELETETRANSITIONACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class Transline;

/** Description Here
 *
 */
class DeleteTransitionAction : public ItemFocusedAction<Transline> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteTransitionAction(Transline *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteTransitionAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
