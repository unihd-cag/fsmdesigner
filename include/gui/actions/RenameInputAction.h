/**
 * \file RenameInputAction.h
 *
 * 
 *
 */

#ifndef RENAMEINPUTACTION_H_
#define RENAMEINPUTACTION_H_

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
class RenameInputAction : public ObjectFocusedAction<Fsm> {

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
        RenameInputAction(QString name,int position,Fsm * object,QUndoCommand * parentCommand = NULL,QWidget * relatedWidget =NULL);

        /**
         * Default Destructor
         */
        virtual ~RenameInputAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
