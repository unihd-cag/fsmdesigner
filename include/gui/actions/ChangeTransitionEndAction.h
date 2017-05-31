/**
 * \file ChangeTransitionEndAction.h
 *
 * 
 *
 */

#ifndef CHANGETRANSITIONENDACTION_H_
#define CHANGETRANSITIONENDACTION_H_

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
class ChangeTransitionEndAction : public ItemFocusedAction<Transline> {

    protected:

        State * targetState;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeTransitionEndAction(State * targetState,Transline *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeTransitionEndAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
