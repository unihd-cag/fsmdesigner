/**
 * \file DeleteHyperTransitionAction.h
 *
 * 
 *
 */

#ifndef DELETEHYPERTRANSITIONACTION_H_
#define DELETEHYPERTRANSITIONACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class HyperTransition;

/** Description Here
 *
 */
class DeleteHyperTransitionAction : public ItemFocusedAction<HyperTransition> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteHyperTransitionAction(HyperTransition *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteHyperTransitionAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
