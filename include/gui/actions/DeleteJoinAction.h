/**
 * \file DeleteJoinAction.h
 *
 * 
 *
 */

#ifndef DELETEJOINACTION_H_
#define DELETEJOINACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class JoinItem;

/** Description Here
 *
 */
class DeleteJoinAction : public ItemFocusedAction<JoinItem> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteJoinAction(JoinItem *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteJoinAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
