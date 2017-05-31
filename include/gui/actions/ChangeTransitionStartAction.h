/**
 * \file ChangeTransitionStartAction.h
 *
 * 
 *
 */

#ifndef CHANGETRANSITIONSTARTACTION_H_
#define CHANGETRANSITIONSTARTACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class Transline;
class StateItem;

/** Description Here
 *
 */
class ChangeTransitionStartAction : public ItemFocusedAction<Transline> {

    protected:

       State * targetState;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeTransitionStartAction(State * targetState,Transline *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeTransitionStartAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
