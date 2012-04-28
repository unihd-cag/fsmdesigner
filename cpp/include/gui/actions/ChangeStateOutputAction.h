/**
 * \file ChangeStateOutputAction.h
 *
 * 
 *
 */

#ifndef CHANGESTATEOUTPUTACTION_H_
#define CHANGESTATEOUTPUTACTION_H_

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
class ChangeStateOutputAction : public ItemFocusedAction<StateItem> {

    private:

        /// Stateoutput to change/undo to
        QString output;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeStateOutputAction(QString output,StateItem *);

        /**
         * Default Destructor
         */
        virtual ~ChangeStateOutputAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
