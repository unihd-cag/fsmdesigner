/**
 * StatusToolBar.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Common
#include <gui/common/FAction.h>

#include "StatusToolBar.h"

StatusToolBar::StatusToolBar( QWidget * parent ) : QWidget(parent)  {

    //-- Set the Layout
    this->setLayout(new QVBoxLayout(this));

    //-- Add the Toolbar
    this->toolbar = new QToolBar(this);
    this->layout()->addWidget(this->toolbar);

    //-- Add the Label
    this->statusLabel = new QLabel(this);
    this->statusLabel->setUpdatesEnabled(true);
    this->layout()->addWidget(this->statusLabel);

}

StatusToolBar::~StatusToolBar() {

}

QToolBar * StatusToolBar::getToolBar() {
    return this->toolbar;
}

FAction * StatusToolBar::addAction(QIcon icon,QString statusLabel) {

    // Create Action and set text
    FAction * action = new FAction(icon);
    action->setWhatsThis(statusLabel);

    this->toolbar->addAction(action);

    // Connect to hover listener
    this->connect(action,SIGNAL(hovered(FAction*)),SLOT(toolbarActionHovered(FAction*)));

    // Return
    return action;

}

QToolButton * StatusToolBar::addActionButton(QIcon icon,QString statusLabel) {

    FAction * action = this->addAction(icon,statusLabel);
    action->setText(statusLabel);
    action->setToolTip("");

    QToolButton * toolButton = dynamic_cast<QToolButton*>(this->toolbar->widgetForAction(action));
    toolButton->setToolTip("");

    return toolButton;
}

void StatusToolBar::toolbarActionHovered(FAction * action) {

    this->statusLabel->setText(action->whatsThis());
}

void StatusToolBar::leaveEvent ( QEvent * event ) {

    // Parent
    QWidget::leaveEvent(event);

    // Clear Status Label
    this->statusLabel->setText("");
}

