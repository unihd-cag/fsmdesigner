/**
 * StatesComboBox.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//--------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Project.h>
#include <core/Fsm.h>
#include <core/State.h>

#include "StatesComboBox.h"

StatesComboBox::StatesComboBox(Fsm * fsm, bool closeClicked ,State * preselectedState,QWidget * parent) :
        QComboBox(parent) {

    //-- Init variables
    this->wasClicked = false;
    this->toClose = closeClicked;
    this->fsm = fsm;


    // Populate (item text with its ID)
    //-----------------------
    int count = 0;
    FOREACH_STATE(this->fsm)

        // Add
        insertItem(count, QString::fromStdString(state->getName()), state->getId());


        // If Preselection is matching, set as selected
        if (state==preselectedState) {
            this->setCurrentIndex(count);
        }

        count++;

    END_FOREACH_STATE

    if (toClose)
        connect(this, SIGNAL(currentIndexChanged(int)), this,
                SLOT(changed(int)));
}

StatesComboBox::~StatesComboBox() {
    if (toClose)
        disconnect(this, SIGNAL(currentIndexChanged(int)), this,
                SLOT(changed(int)));
}

State * StatesComboBox::getSelectedState() {

    return this->fsm->getStatebyID(this->itemData(this->currentIndex(), Qt::UserRole).toInt());

}

void StatesComboBox::changed(int idx) {
    this->wasClicked = true;
    this->close();
}
