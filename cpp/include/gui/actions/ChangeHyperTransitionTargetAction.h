/**
 * \file ChangeHyperTransitionTargetAction.h
 *
 * 
 *
 */

#ifndef CHANGEHYPERTRANSITIONTARGETACTION_H_
#define CHANGEHYPERTRANSITIONTARGETACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class HyperTransition;

//-- Core
class State;

/** Description Here
 *
 */
class ChangeHyperTransitionTargetAction : public ItemFocusedAction<HyperTransition> {


    protected:

        State * targetState;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeHyperTransitionTargetAction(State * targetState,HyperTransition *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeHyperTransitionTargetAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
