/**
 * \file DeleteLinkDepartureAction.h
 *
 * 
 *
 */

#ifndef DELETELINKDEPARTUREACTION_H_
#define DELETELINKDEPARTUREACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class LinkDeparture;

/**
 * To Delete a link departure (From state to link)
 *
 * A Connection to a link is a transition from a Start Item (State, Join etc...) to a Link Item
 * The model is a transition with last trackpoint containing the link id
 *
 * So the actions is the same as a DeleteTransitionAction
 *
 */
class DeleteLinkDepartureAction : public ItemFocusedAction<LinkDeparture> {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteLinkDepartureAction(LinkDeparture *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteLinkDepartureAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
