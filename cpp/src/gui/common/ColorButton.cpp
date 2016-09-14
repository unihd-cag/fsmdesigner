/**
 * ColorButton.cpp
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

#include "ColorButton.h"

ColorButton::ColorButton(QColor c, QWidget* parent) :
        QPushButton(parent) {

    // Init
    this->color = c;
    this->wasClicked = false;

    // Connect
    connect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

ColorButton::~ColorButton() {
    disconnect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

void ColorButton::setColor(QColor newColor) {

    //-- Set color and emmit signal if changed
    if (newColor != this->color) {

        this->color = newColor;
        this->setText(color.name());
        this->colorChanged(color);
    }

}

void ColorButton::paintEvent(QPaintEvent * event) {

    //QPushButton::paintEvent(event);




    QPainter painter (this);
    painter.fillRect(event->rect(),this->color);


}


QColor ColorButton::getColor() {

    return this->color;
}

void ColorButton::buttonClicked() {

    //-- Show Selector
    QColor newColor = QColorDialog::getColor(this->color,this);

    //-- Update if valid
    if (newColor.isValid()) {
         this->setColor(newColor);
    }
}

