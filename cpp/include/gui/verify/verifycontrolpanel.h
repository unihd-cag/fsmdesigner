/**
 *
 */
#ifndef VERIFY_CONTROL_PANEL
#define VERIFY_CONTROL_PANEL

// Includes
//----------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>


/**
 * This Panel layouts control widgets to activate dynamic scene verification on a Scene
 */
class VerifyControlPanel: public QWidget, public SceneRelatedObject {

    Q_OBJECT

    protected:

        /** \defgroup Widgets Controls of this Widget */
        /** @{ */

        /// Button to activate verification
        QPushButton * verifyButton;

        /// Button to reset verification result
        QPushButton * verifyResetButton;

        /** @} */

    public:
        VerifyControlPanel(QWidget * parent = NULL);
        virtual ~VerifyControlPanel();

    public Q_SLOTS:

        /// Disconnects/Connects buttons to Verification in Scene upon scene change
        virtual void setRelatedScene(Scene * scene);

};

#endif
