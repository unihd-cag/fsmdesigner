/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

#ifndef FSMSceneView_H_
#define FSMSceneView_H_

// Includes
//------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>

//-- GUI
class SelectionAnimator;
class Scene;
class Transline;
class PropertyWidget;

//-- Gui Common
#include <gui/scene/SceneRelatedObject.h>
class FAction;
class RoundedPanel;
class StatusToolBar;

//-- Verification
#include <verification/VerificationListener.h>

class FSMSceneView: public QGraphicsView,
        public SceneRelatedObject{

    Q_OBJECT

    private:

        ///\defgroup SelectionArea Selection area variables
        /// @{

        /// ?
        Qt::ItemSelectionMode mode;

        /// Pen to draw selection area
        QPen * selectionAreaPen;

        /// Initial selected Area point. Set when selection drag started
        QPointF * areaSelectionInitial;

        /// @}

        ///\defgroup InfoWidgets Info Widgets
        /// @{

        /// Lock Icon to indicate control has been pressed
        QWidget * lockModeicon;
        QLabel * lockIcon;
        QCursor * lockCursor;

        /// Embedded Toolbar
        StatusToolBar * controlToolBar;

        /// Help Panel to show embedded help
        QTextBrowser * helpPanel;

        ///@}

        //// Animator on selection
        SelectionAnimator * selectionAnimator;

    public:
        FSMSceneView(Scene* scene, QWidget* parent = NULL);
        ~FSMSceneView();

        /** \defgroup Verification VerificationListener Methods */
        /** @{ */

        /** @} */

    protected:
        virtual void wheelEvent(QWheelEvent *event); //Zoom in and out with the  mouse Wheel
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void mouseMoveEvent(QMouseEvent* e);
        virtual void mouseReleaseEvent(QMouseEvent* e);
        virtual void keyPressEvent(QKeyEvent* ke);
        virtual void keyReleaseEvent(QKeyEvent* ke);
        virtual void drawForeground(QPainter * painter, const QRectF & rect);
        virtual void resizeEvent(QResizeEvent * event);
        virtual void showEvent(QShowEvent * event);

        /// Scroll Event to update control panel and other overlayed panels
        virtual void scrollContentsBy(int dx, int dy);

    private:

        void initVariables();

    public slots:

        /**
         * If the settings changed. Recheck some values
         */
        void settingsChanged();

        /** \defgroup Toolbar Embedded Tool Bar Actions */
        /** @{ */

        /**
         * Enables/Disables Help Pane
         */
        void toggleHelp();

        /**
         * Global Table edition action
         */
        void showGlobalTable();

        /** @} */

        /** \addtogroup InfoWidgets */
        /** @{ */

        /**
         * Updates the help based on the selection
         */
        void changeHelp();

        /** @} */

        /// Fit the scene
        void fit();

        /// Generate verilog for the currently displayed FSM
        void generateVerilog();

        /// Reload/Generate verilog for the currently displayed FSM
        void generateVerilogReload();

        /** \defgroup Place Place mode methods */
        /**@{*/

        void placeSetUnlocked();
        void placeSetLocked();

        void placeSetMode(FSMDesigner::Item w);
        /**@}*/

        /// Slot for when the selection on the scene has changed
        void sceneSelectionChanged();

        /**
         * Perform a save
         * This saves the project because single FSM save is not possible
         */
        void saveProject();

        /**
         * Performs a save to a specified location
         * This saves the project because single FSM save is not possible
         */
        void saveProjectAs();

        /**
         * Opens A control dialog to rename the FSM
         */
        void renameFsm();

        /** \defgroup Verify Verification */
        /**@{*/

        /**
         * Execute verification scripts with Results table
         */
        void verifyTable();

        /**@}*/

};

#endif
