/**
 * \file DeleteOutputAction.h
 *
 * 
 *
 */

#ifndef DELETEOUTPUTACTION_H_
#define DELETEOUTPUTACTION_H_

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
class DeleteOutputAction : public ObjectFocusedAction<Fsm> {

    protected:

        /// Position Of Deleted Output
        int position;

        /// Name of Deleted Output
        string name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteOutputAction(int position,Fsm *,QUndoCommand * parentCommand = NULL,QTreeView * relatedWidget = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteOutputAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
