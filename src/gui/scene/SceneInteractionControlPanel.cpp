/*
 * SceneInteractionControlPanel.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: rleys
 */

// Includes
//------------------------

//-- Qt
#include <QtCore>
#include <QtGui>

#include "SceneInteractionControlPanel.h"

SceneInteractionControlPanel::SceneInteractionControlPanel() {

	// Set Grid Layout
	//-----------
	QGraphicsGridLayout * layout = new QGraphicsGridLayout();
	this->setLayout(layout);

	//-- Add Title
	QTextItem * title = new QTextItem();

	QGraphicsProxyWidget * titleProxy = new QGraphicsProxyWidget();
	//layout->addItem(titleProxy->createProxyForChildWidget(title),0,0);

	//-- Add Shortcuts panel

	//-- Add info text

}

SceneInteractionControlPanel::~SceneInteractionControlPanel() {
	// TODO Auto-generated destructor stub
}
