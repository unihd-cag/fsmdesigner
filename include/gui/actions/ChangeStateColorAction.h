/**
 * \file ChangeStateColorAction.h
 *
 * 
 *
 */

#ifndef CHANGESTATECOLORACTION_H_
#define CHANGESTATECOLORACTION_H_

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
class ChangeStateColorAction : public ItemFocusedAction<StateItem> {

    private:

        /// Color to change/redo to
        QColor color;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeStateColorAction(QColor color,StateItem *);

        /**
         * Default Destructor
         */
        virtual ~ChangeStateColorAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
