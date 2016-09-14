/**
 * \file ItemFocusedAction.h
 *
 */

#ifndef OBJECTFOCUSEDACTION_H_
#define OBJECTFOCUSEDACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Actions
#include <gui/actions/FSMAction.h>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

/**
 *
 * \brief Base Action Class for Actions that are focused around a Graphic Item
 *
 */
template<class T> class ObjectFocusedAction : public QUndoCommand, public SceneRelatedObject {

    protected:

        /// Reference to the concerned FSM Graphic Item
        T * object;

        QWidget * relatedWidget;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ObjectFocusedAction(T * object,QUndoCommand * parentCommand = NULL,QWidget * relatedWidget = NULL) : QUndoCommand(parentCommand) {
            this->object = object;
            this->relatedWidget = relatedWidget;
        }

        /**
         * Default Destructor
         */
        virtual ~ObjectFocusedAction() {

        }


        /** \defgroup UndoRedo Undo Redo Methods to be implemented */
        /** @{ */

        virtual int     id() {
            return 0;
        }
        virtual bool    mergeWith(const QUndoCommand * command){
            return false;
        }
        virtual void    redo(){
            //-- Call Parent
            QUndoCommand::redo();
        }
        virtual void    undo(){
            //-- Call Parent
            QUndoCommand::undo();
        }

        /** @} */

};

#endif /* UNDODELETELINKJOIN_H_ */
