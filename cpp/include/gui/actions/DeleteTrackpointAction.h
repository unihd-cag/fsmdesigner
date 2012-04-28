/**
 * \file DeleteTrackpointAction.h
 *
 * 
 *
 */

#ifndef DELETETRACKPOINTACTION_H_
#define DELETETRACKPOINTACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/ItemFocusedAction.h>

//-- Gui Items
class TrackpointItem;

/** Description Here
 *
 */
class DeleteTrackpointAction : public ItemFocusedAction<TrackpointItem> {

    protected:

        /**
         * The previous item where the trackpoint was
         */
        QGraphicsItem * previousItem;

        /**
         * The Item after the trackpoint
         */
        QGraphicsItem * nextItem;

        /**
         * The Trackpoint after in the model (to readd at the right position
         */
        Trackpoint * nextModel;


    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        DeleteTrackpointAction(TrackpointItem *,QUndoCommand * parentCommand = NULL);

        /**
         * Default Destructor
         */
        virtual ~DeleteTrackpointAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
