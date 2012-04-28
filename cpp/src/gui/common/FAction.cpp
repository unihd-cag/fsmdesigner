/**
 * \file FAction.cpp
 *
 * 
 *
 */

// Includes
//--------------------

//-- Std
#include <iostream>
#include <ostream>
#include <sstream>
using namespace std;


//-- Qt
#include <QtGui>
#include <QtCore>


#include "FAction.h"

FAction::FAction(QIcon icon, QString text) : QAction(icon,text,NULL) {
    this->setText(text);

    // Init Actions
    //-----------------------


    //-- Connect hover signal to generate an hover signal with this object
    this->connect(this,SIGNAL(hovered()),SLOT(fetchHover()));


}

FAction::FAction(QString text) : QAction(text,NULL) {

    // Init Actions
    //-----------------------

    //-- Connect hover signal to generate an hover signal with this object
    this->connect(this,SIGNAL(hovered()),SLOT(fetchHover()));

}

FAction::~FAction() {

}

bool FAction::event ( QEvent * e ) {



    // If Leaving this component, send signal
    //----------------
    if (e->type()==QEvent::HoverLeave) {
        this->hoveredLeave(this);
    }

    // Call Parent
   //------------------
   return QAction::event(e);
}

void FAction::fetchHover() {

    // Propagate to hover signal with reference to this object
    this->hovered(this);

}


