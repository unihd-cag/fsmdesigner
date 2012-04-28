/**
 * \file DeleteLinkArrivalAction.h
 *
 * 
 *
 */

#ifndef DELETELINKARRIVALACTION_H_
#define DELETELINKARRIVALACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class LinkArrival;

/** Description Here
 *
 */
class DeleteLinkArrivalAction : public ItemFocusedAction<LinkArrival> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteLinkArrivalAction(LinkArrival *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteLinkArrivalAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
