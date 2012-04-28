/**
 * \file DeleteConditionAction.h
 *
 * 
 *
 */

#ifndef DELETECONDITIONACTION_H_
#define DELETECONDITIONACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>

//-- Gui Items
class Condition;
class Trans;

/** Description Here
 *
 */
class DeleteConditionAction : public ObjectFocusedAction<Condition> {

    protected:

        /// The transition we are deleting from
        Trans * transition;

        /// The position at which the condition was (to allow reinserting)
        unsigned int position;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteConditionAction(Trans * transition,Condition *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteConditionAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
