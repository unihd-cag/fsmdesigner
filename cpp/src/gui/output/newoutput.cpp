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
#include <QtCore>

#include "newoutput.h"

NewOutput::NewOutput(Fsm * fsm,QWidget* parent) : QDialog(parent) , FSMRelatedObject(fsm) {

	//-- Setup UI
	setupUi(this);
}


void NewOutput::accept() {

	//-- FIXME Verify we have a FSM provided

	//-- Get Offered name
	QString qName = this->lineEditOutput->text();

	//-- Filter
	FilterKeywords* fk = new FilterKeywords();
	qName = fk->RemoveBlanks(qName);
	qName = fk->FilterSpecialChars(qName);
	qName = fk->FilterHDLKeywords(qName);

	//-- To Std string
	string name = qName.toStdString();

	//-- Add to fsm
    this->getRelatedFsm()->addOutput(name,out0radioButton->isChecked()? 0 : 1);

	this->done(0);
}
