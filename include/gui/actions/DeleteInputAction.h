/**
 * \file DeleteInputAction.h
 *
 * 
 *
 */

#ifndef DELETEINPUTACTION_H_
#define DELETEINPUTACTION_H_

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
class DeleteInputAction : public ObjectFocusedAction<Fsm> {

    protected:

        /// Position Of Deleted Input
        int position;

        /// Name of Deleted Input
        string name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteInputAction(int position,Fsm *,QUndoCommand * parentCommand = NULL,QTreeView * relatedWidget = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteInputAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
