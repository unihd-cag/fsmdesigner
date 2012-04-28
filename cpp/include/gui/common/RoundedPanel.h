/*
 * RoundedPanel.h
 *
 *  Created on: May 18, 2011
 *      Author: rleys
 */

#ifndef ROUNDEDPANEL_H_
#define ROUNDEDPANEL_H_

// Includes
//----------

//-- Qt
#include <QtGui>
#include <QtCore>

class RoundedPanel : public QWidget {
public:
	RoundedPanel(QWidget * parent = NULL);
	virtual ~RoundedPanel();

protected:

	/// Paint the rounded panel
	virtual void paintEvent ( QPaintEvent * event );

};

#endif /* ROUNDEDPANEL_H_ */
