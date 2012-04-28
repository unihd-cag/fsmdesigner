/*
 * RoundedPanel.cpp
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

// Includes
//----------

//-- Qt
#include <QtGui>
#include <QtCore>

#include "RoundedPanel.h"

RoundedPanel::RoundedPanel(QWidget * parent) : QWidget(parent) {
	// TODO Auto-generated constructor stub

}

RoundedPanel::~RoundedPanel() {
	// TODO Auto-generated destructor stub
}


void RoundedPanel::paintEvent ( QPaintEvent * event ) {

	/// Build a paint engine
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	//-- Rounded rect path
	QPainterPath roundedRectPath;
	roundedRectPath.addRoundedRect(this->rect(),20,20);

	//-- The Brush
	QBrush roundedRectBrush(this->palette().background());

	//-- Paint
	painter.fillPath(roundedRectPath,this->palette().background());
	//painter.drawPath(roundedRectPath);
	// Fill with parent
	//QWidget::paintEvent(event);

	painter.end();

}
