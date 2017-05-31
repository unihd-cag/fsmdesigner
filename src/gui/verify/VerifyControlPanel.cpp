/**
 *
 */

// Includes
//----------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>
#include <gui/verify/FSMVerificator.h>

#include "verifycontrolpanel.h"

VerifyControlPanel::VerifyControlPanel(QWidget * parent) : QWidget(parent) , SceneRelatedObject() {

    // Init GUI
    //--------------------------

    //-- Layout
    QGridLayout * verificationPanelLayout = new QGridLayout();
    this->setLayout(verificationPanelLayout);



    //------ Verify
    this->verifyButton = new QPushButton(QIcon(QPixmap(
                    ":/icons/verification.png")), "");
    this->verifyButton->setIconSize(QSize(24, 24));
    this->verifyButton->setFlat(true);
    this->verifyButton->setToolTip("Verify");
    verificationPanelLayout->addWidget(this->verifyButton, 0, 0, Qt::AlignLeft);

    //------ Reset
    this->verifyResetButton = new QPushButton(QIcon(QPixmap(
                        ":/icons/Clear-brush")), "");
    this->verifyResetButton->setIconSize(QSize(24, 24));
    this->verifyResetButton->setFlat(true);
    this->verifyResetButton->setToolTip("Reset Verification");
    verificationPanelLayout->addWidget(this->verifyResetButton, 0, 1, Qt::AlignLeft);


}

VerifyControlPanel::~VerifyControlPanel() {

}

void VerifyControlPanel::setRelatedScene(Scene * scene) {

    // If Actual Scene exists -> disconnect
    //-----------------
    if (this->getRelatedScene()!=NULL) {

        //-- Disconnect Verify
        this->verifyButton->disconnect(this->getRelatedScene()->getFSMVerificator(),SLOT(verify()));

        //-- Disconnect Reset
        this->verifyResetButton->disconnect(this->getRelatedScene()->getFSMVerificator(),SLOT(reset()));
    }


    // If Provided Scene is not null -> disconnect
    //------------------
    SceneRelatedObject::setRelatedScene(scene);
    if (this->getRelatedScene()!=NULL) {

        //-- Connect Verify
        this->getRelatedScene()->getFSMVerificator()->connect(this->verifyButton,SIGNAL(clicked()),SLOT(verify()));

        //-- Connect Reset
        this->getRelatedScene()->getFSMVerificator()->connect(this->verifyResetButton,SIGNAL(clicked()),SLOT(reset()));

    }


}
