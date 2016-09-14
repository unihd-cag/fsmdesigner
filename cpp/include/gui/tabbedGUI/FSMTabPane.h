/*
 * FSMTabPane.h
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

#ifndef FSMTABPANE_H_
#define FSMTABPANE_H_

// Includes
//-----------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
class Fsm;

//-- Gui
class Scene;

/**
 * This class is a tabbed pane to open the multiple FSMs of a project
 */
class FSMTabPane: public QTabWidget {

    Q_OBJECT


    protected:

        /**
         * Maintain a pointer to the actual displayed scene
         */
        Scene * selectedScene;

    public:
        FSMTabPane(QWidget * parent = NULL);
        virtual ~FSMTabPane();


    public:

        /**
         * Returns the selected scene
         * @return
         */
        Scene * getSelectedScene();

    protected:
        virtual void keyPressEvent(QKeyEvent * e);
        virtual void tabInserted(int index);





    Q_SIGNALS:

        /**
         * This signal is triggered when the tab selection changes,
         * Reporting the pointer to the now displayed scene.
         * This is useful for external components that act depending on the displayed scene
         */
        void sceneSelectionChanged(Scene *);

    protected slots:

        /**
         * This slot received the currentChanged signal to propagate to the #sceneSelectionChanged(Scene*) signal
         */
        void tabSelectionChanged(int index);

    public Q_SLOTS:

        /**
         * \defgroup SceneAction Current Scene Actions
         *  This group gathers slots relative to interations withe the displayed scene
         */
        /**@{*/

        void placeSetState();
        void placeSetTransition();
        void placeSetHyperTransition();
        void placeSetLink();
        void placeSetJoin();

        void zoomIn();
        void zoomOut();

        /**
         * Makes a simple Generate Verilog call
         */
        void generateVerilog();

        /**
         * Calls undo on scene
         */
        void undo();

        /**
         * Calls redo on scene
         */
        void redo();

        /**@}*/

        /**
         * Delete the FSM at tab index
         * Deleting a FSM means removing it from project
         * @param index The index of the tab containing the FSM to delete
         */
        void deleteFSM(int index);

        /**
         * Closes the FSM managed by the tab at the provided index
         *
         * @param fsm
         */
        void closeFSM(int index);

        /**
         * Add A new FSM To project
         */
        void addFSM();

        /**
         * Opens an FSM in the TabPane
         * @param fsm
         */
        void openFSM(Fsm * fsm);

        /// If the UI settings have changed
        void settingsChanged();

};

#endif /* FSMTABPANE_H_ */
