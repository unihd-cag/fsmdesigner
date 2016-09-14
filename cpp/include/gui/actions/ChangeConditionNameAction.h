/**
 * \file ChangeConditionNameAction.h
 *
 * 
 *
 */

#ifndef CHANGECONDITIONNAMEACTION_H_
#define CHANGECONDITIONNAMEACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ObjectFocusedAction.h>

//-- Gui Items
class Condition;

/** Description Here
 *
 */
class ChangeConditionNameAction : public ObjectFocusedAction<Condition> {

    protected:

        // New // Old name
        QString name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeConditionNameAction(QString,Condition *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeConditionNameAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
