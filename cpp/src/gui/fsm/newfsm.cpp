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
#include <gui/output/newoutputinit.h>
#include <gui/input/newinputinit.h>

//-- Qt
#include <QtGui>
#include <QtCore>

#include "newfsm.h"
NewFsm::NewFsm(Fsm * fsm,QWidget* parent, Qt::WFlags f) :
	QDialog(parent, f) {
	setupUi(this);
	connections();
	incount = 0;
	outcount = 0;


}

void NewFsm::connections() {
	connect(newinput, SIGNAL(clicked()), this, SLOT(newInput()));
	connect(newoutput, SIGNAL(clicked()), this, SLOT(newOutput()));
	connect(deleteinput, SIGNAL(clicked()), this, SLOT(deleteInput()));
	connect(deleteoutput, SIGNAL(clicked()), this, SLOT(deleteOutput()));
	connect(inputup, SIGNAL(clicked()), this, SLOT(moveInputUp()));
	connect(outputup, SIGNAL(clicked()), this, SLOT(moveOutputUp()));
	connect(inputdown, SIGNAL(clicked()), this, SLOT(moveInputDown()));
	connect(outputdown, SIGNAL(clicked()), this, SLOT(moveOutputDown()));
}

void NewFsm::deleteOutput() {

	QTreeWidgetItem *it = outputlist->currentItem();
	int i = outputlist->indexOfTopLevelItem(it);

	this->fsm->deleteOutput(i);

}

void NewFsm::deleteInput() {
	QTreeWidgetItem *it = inputlist->currentItem();
	int i = inputlist->indexOfTopLevelItem(it);

	this->fsm->deleteInput(i);


}

void NewFsm::newInput() {
	NewInputInit* nii;
	nii = new NewInputInit(this);
	nii->exec();
}

void NewFsm::newOutput() {
	NewOutputInit* noi;
	noi = new NewOutputInit(this);
	noi->exec();
}

void NewFsm::inText(QString qs) {
	QTreeWidgetItem *it = new QTreeWidgetItem(inputlist);
	it->setText(0, qs);
	incount++;
}

void NewFsm::outText(QString qs) {
	QTreeWidgetItem *it = new QTreeWidgetItem(outputlist);
	it->setText(0, qs);
	outcount++;
}

void NewFsm::reject() {
	Fsm& f = *(this->fsm);
	// for (int i = f.getNumberOfInputs(); i > 0; i--) {
	//     f.deleteInputName();
	//   }
	//   for(int i = f.getNumberOfOutputs(); i > 0; i--) {
	//     f.deleteOutputName();
	//   }
	//f.numberOfIns(0, -1000);
	//f.numberOfOuts(0, false);
	//this->close();
	this->done(0);
}

void NewFsm::moveInputUp() {
	QTreeWidgetItem *it = inputlist->currentItem();
	int i = inputlist->indexOfTopLevelItem(it);

	this->fsm->moveupInput(i);


}

void NewFsm::moveInputDown() {
	QTreeWidgetItem *it = inputlist->currentItem();
	int i = inputlist->indexOfTopLevelItem(it);
	this->fsm->movedownInput(i);
}

void NewFsm::moveOutputUp() {
	QTreeWidgetItem *it = outputlist->currentItem();
	int i = outputlist->indexOfTopLevelItem(it);
	this->fsm->moveupOutput(i);
}

void NewFsm::moveOutputDown() {
	QTreeWidgetItem *it = outputlist->currentItem();
	int i = outputlist->indexOfTopLevelItem(it);
	this->fsm->movedownOutput(i);
}

void NewFsm::accept() {
	this->done(QDialog::Accepted);
}

