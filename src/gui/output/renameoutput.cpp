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

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/FSMRelatedObject.h>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>

#include <gui/actions/RenameOutputAction.h>


#include "renameoutput.h"
RenameOutput::RenameOutput(Scene * scene, int index, QWidget* parent) :
		QDialog(parent), SceneRelatedObject(scene) {

	//-- Set UI
	setupUi(this);

	//-- Init
	this->index = index;
	this->lineEditOutput->setText(QString::fromStdString(this->getRelatedScene()->getFsm()->getOutputName(index)));
}


void RenameOutput::accept() {

	//-- Update FSM
	this->getRelatedScene()->getUndoStack()->push(
	                        new RenameOutputAction(lineEditOutput->text(),index,this->getRelatedScene()->getFsm(),NULL,this->parentWidget()));

	this->done(0);
}
