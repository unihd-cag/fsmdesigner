/**
 * \file ChangeFSMResetStateAction.h
 *
 */

#ifndef CHANGEFSMRESETSTATEACTION_H_
#define CHANGEFSMRESETSTATEACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/FSMAction.h>

/** Description Here
 *
 */
class ChangeFSMResetStateAction : public FSMAction {

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ChangeFSMResetStateAction(Scene *);

        /**
         * Default Destructor
         */
        virtual ~ChangeFSMResetStateAction();


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id();
        virtual bool    mergeWith(const QUndoCommand * command);
        virtual void    redo();
        virtual void    undo();

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
