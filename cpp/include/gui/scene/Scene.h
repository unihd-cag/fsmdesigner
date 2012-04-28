#ifndef SCENE_H
#define SCENE_H

// Includes
//----------------------

//-- Std

//-- Core
#include <core/FSMDesigner.h>
class Fsm;
class Link;
class Trans;
class State;
class Join;


//-- Gui
class StateItem;
class LinkArrival;
class Transline;
class TranslineText;
class LinkDeparture;
class JoinItem;

//-- Verification
class FSMVerificator;

//-- Qt
#include <QtGui>
#include <QtCore>

class Scene: public QGraphicsScene {

    Q_OBJECT

    private:

        /// The designed FSM
        Fsm * fsm;

        /** \defgroup FSM Items Placement*/
        /**@{*/

    protected:

        /// What kind of item we want to place
        FSMDesigner::Item placeMode;

        /// Used for drawing of multiple objects when locked
        FSMDesigner::PlaceLockMode placeLock;

        ///
        /** Stack to place transitions
         *  Should contain a stack of the elements to join together. Ex:
         *  - State
         *  - Trackpoint
         *  - Trackpoint
         *  - State
         */
        QList<QGraphicsItem*> placeTransitionStack;

        /**
         * Stack of elements that have to follow the mouse to be placed
         *
         * The first one is shown and follows the mouse.
         * On click it is placed and the next one is shown and sticks to mouse etc...
         *
         */
        QList<QGraphicsItem*> toPlaceStack;

        /**\defgroup Place link */

        StateItem * placeLinkStartState;
        StateItem * placeLinkEndState;

        /**@} */

        /**
         * Add the provided item to the stack of items to be placed on the scene
         * This method applies important rules, like setting elements to not visible
         * @param
         */
        void addToToPlaceStack(QGraphicsItem *);

        /**
         * Place the provided item under the mouse, centering it by using its dimensions
         * @param
         * @param mousePosition
         */
        void placeUnderMouse(QGraphicsItem *, QPointF mousePosition);

        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

        /**
         * Handle Key events like suppressions
         * @param keyEvent
         */
        void keyReleaseEvent(QKeyEvent * keyEvent);

    public:

        /// @warning Does not initialize the Scene
        Scene(Fsm *, QObject *parent = 0);

        /// First draws all the components of provided FSM on the scene
        void initializeScene();

        /** \defgroup Place mode methods */
        /**@{*/

    public slots:
        void setPlaceUnlocked() {
            this->placeLock = FSMDesigner::UNLOCKED;
        }
        void setPlaceLocked() {
            this->placeLock = FSMDesigner::LOCKED;
        }

        void setPlaceMode(FSMDesigner::Item mode);

        void startPlaceState();

        void startPlaceTransition();

        void startPlaceHyperTransition();

        void startPlaceLink();

        void startPlaceJoin();

    public:

        FSMDesigner::Item getPlaceMode() {
            return this->placeMode;
        }

        FSMDesigner::PlaceLockMode getPlaceModeLock() {
            return this->placeLock;
        }

        /**@}*/

        Fsm * getFsm();
        void setFsm(Fsm *);

        /** \addtogroup FSM Items Placement*/
        /**@{*/

    protected:

        LinkArrival * placeLinkToState(StateItem * state, Link * link);

        /**@}*/

        /** \defgroup Layouting methods */
        /**@{*/

    public slots:

        /**
         * Align vertically only States
         */
        void alignSelectionVertical();

        /**
         * Align horizontally only States
         */
        void alignSelectionHorizontal();

        /**@}*/

        /** \defgroup Undo Methods/Fields */
        /**@{*/
    protected:

        /**
         * The stack to store undo/redo operations
         */
        QUndoStack undoStack;

    public:
        QUndoStack * getUndoStack();

        /**@}*/

        /** \defgroup Utilities Signals/Slots */
        /**@{*/

    public slots:

        /**
         *  Triggers a selection empty signal if nothing is selected
         */
        void selectionMonitor() {
            if (this->selectedItems().size() == 0) {
                this->selectionEmpty();
            }
        }

    Q_SIGNALS:

        void selectionEmpty();

        /** \defgroup Verification */
        /**@{*/

    protected:

        /// Verificator to execute verification on FSM
        FSMVerificator * fsmVerificator;

    public:

        void setFSMVerificator(FSMVerificator * verificator) {
            this->fsmVerificator = verificator;
        }

        FSMVerificator * getFSMVerificator() {
            return this->fsmVerificator;
        }

    public slots:
        /**
         * Execute verification scripts
         */
        void verify();

        /**@}*/

        /** \defgroup Find Find Objects Utilities */
        /** @{ */


        /**
         * Finds the Transline matching the provided model
         * @param transitionModel
         * @return The found translines or an empty list if not found
         */
        public: QList<Transline *> findTransline(Trans * transitionModel);

        /**
         * Finds the text matching a Transition
         */
        public: TranslineText * findTranslineText(Trans * transitionModel);

        /**
         * Find the State matching the provided model
         * @param state
         * @return The found State or NULL if not found
         */
        public: StateItem * findStateItem(State * state);

        /**
         * Find the JoinItem item matching the provided model
         * @param joinmodel
         * @return The found join point or NULL if not found
         */
        JoinItem * findJoinItem(Join * joinmodel);

        /**
         * Find all LinkDepartures that link to this Link * model
         * @param link the base link model
         * @return All the LinkDepartures (State -> Link) that target this link
         */
        public: QList<LinkDeparture*> findLinkDepartures(Link * link);

};

#endif // SCENE_H
