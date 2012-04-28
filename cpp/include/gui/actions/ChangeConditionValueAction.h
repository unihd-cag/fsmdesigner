/**
 * \file ChangeConditionValueAction.h
 *
 * 
 *
 */

#ifndef CHANGECONDITIONVALUEACTION_H_
#define CHANGECONDITIONVALUEACTION_H_

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
class ChangeConditionValueAction : public ObjectFocusedAction<Condition> {

    protected:

        // New // Old name
        QString value;


    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeConditionValueAction(QString ,Condition *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeConditionValueAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
