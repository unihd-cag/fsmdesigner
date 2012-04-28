/*
 * GUIUtils.h
 *
 *  Created on: Aug 19, 2011
 *      Author: rleys
 */

#ifndef GUIUTILS_H_
#define GUIUTILS_H_

// Includes
//--------------

//-- Qt
#include <QtGui>
#include <QtCore>

class GUIUtils {
public:
	GUIUtils();
	virtual ~GUIUtils();


	/**
	 *
	 * @param path
	 * @return The QString wit The content of the file - Empty String if file not found or anything happens
	 */
	static QString readFileToQString(QString path) ;

};

#endif /* GUIUTILS_H_ */
