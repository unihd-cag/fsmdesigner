/**
 * \file RenameOutputAction.h
 *
 * 
 *
 */

#ifndef RENAMEOUTPUTACTION_H_
#define RENAMEOUTPUTACTION_H_

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
class RenameOutputAction : public ObjectFocusedAction<Fsm> {

    protected:

           /// Position of modified item
           int position;

           /// Name to restore
           QString name;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        RenameOutputAction(QString name,int position,Fsm * object,QUndoCommand * parentCommand = NULL,QWidget * relatedWidget = NULL);

        /**
         * Default Destructor
         */
        virtual ~RenameOutputAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
