/*
 * GUIUtils.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: rleys
 */

#include "GUIUtils.h"

GUIUtils::GUIUtils() {
	// TODO Auto-generated constructor stub

}

GUIUtils::~GUIUtils() {
	// TODO Auto-generated destructor stub
}


QString GUIUtils::readFileToQString(QString path) {

	QFile scriptFile(path);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();

	return contents;

}
