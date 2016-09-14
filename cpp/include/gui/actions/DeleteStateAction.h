/**
 * \file DeleteStateAction.h
 *
 * 
 *
 */

#ifndef DELETESTATEACTION_H_
#define DELETESTATEACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class StateItem;

/** Description Here
 *
 */
class DeleteStateAction : public ItemFocusedAction<StateItem> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteStateAction(StateItem *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteStateAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
