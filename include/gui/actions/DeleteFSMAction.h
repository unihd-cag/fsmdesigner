/**
 * \file DeleteFSMAction.h
 *
 * 
 *
 */

#ifndef DELETEFSMACTION_H_
#define DELETEFSMACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>

//-- Gui Items
class Fsm;

/** Description Here
 *
 */
class DeleteFSMAction : public ObjectFocusedAction<Fsm> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteFSMAction(Fsm *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteFSMAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
