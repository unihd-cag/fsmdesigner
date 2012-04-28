/**
 * \file ItemFocusedAction.h
 *
 */

#ifndef ITEMFOCUSEDACTION_H_
#define ITEMFOCUSEDACTION_H_

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
template<class T> class ItemFocusedAction : public QUndoCommand, public SceneRelatedObject {


    protected:

        /// Reference to the concerned FSM Graphic Item
        T * item;

    public:

        /**
         * Default constructor
         * Must refer to a scene
         */
        ItemFocusedAction(T * item,QUndoCommand * parentCommand = NULL) : QUndoCommand(parentCommand),SceneRelatedObject(dynamic_cast<Scene*>(item->scene())) {
            this->item = item;
        }

        /**
         * Default Destructor
         */
        virtual ~ItemFocusedAction() {

        }

        /**
         * Get the UI Item stored
         * @return
         */
        T * getItem() {
            return this->item;
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
