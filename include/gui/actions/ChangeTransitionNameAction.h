/**
 * \file ChangeTransitionNameAction.h
 *
 * 
 *
 */

#ifndef CHANGETRANSITIONNAMEACTION_H_
#define CHANGETRANSITIONNAMEACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
#include <gui/items/Transline.h>

/** Description Here
 *
 */
class ChangeTransitionNameAction : public ItemFocusedAction<Transline> {


    protected:

        // New // Old name
        QString name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeTransitionNameAction(QString name ,Transline *tr,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~ChangeTransitionNameAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
