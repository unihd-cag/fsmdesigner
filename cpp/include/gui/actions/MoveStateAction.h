/**
 * \file MoveStateAction.h
 *
 * 
 *
 */

#ifndef MOVESTATEACTION_H_
#define MOVESTATEACTION_H_

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
class MoveStateAction : public ItemFocusedAction<StateItem> {

    protected:

        QPointF position;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        MoveStateAction(QPointF position,StateItem *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~MoveStateAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
