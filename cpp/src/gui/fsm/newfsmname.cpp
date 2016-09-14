/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

// Includes
//----------------------

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Gui
#include <gui/common/filterkeywords.h>

//-- Qt
#include <QtGui>

#include "newfsmname.h"
FsmName::FsmName(Fsm * fsm , QWidget * parent) :
	QDialog(parent) ,FSMRelatedObject(fsm) {

	//-- Init UI
	setupUi(this);

}

FsmName::~FsmName() {

}


void FsmName::accept() {

	QString qName = lineEditFSMName->text();

	FilterKeywords* fk  = new FilterKeywords();
	qName = fk->RemoveBlanks(qName);
	qName = fk->FilterSpecialChars(qName);
	qName = fk->FilterBusChars(qName);
	this->getRelatedFsm()->setName(qName.toStdString());

	this->done(QDialog::Accepted);
}

