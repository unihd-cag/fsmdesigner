/**
 * \file ChangeStateNameAction.h
 *
 * 
 *
 */

#ifndef CHANGESTATENAMEACTION_H_
#define CHANGESTATENAMEACTION_H_

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
class ChangeStateNameAction : public ItemFocusedAction<StateItem> {

    protected:

        // New // Old name
        QString name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeStateNameAction(QString name,StateItem *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeStateNameAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
