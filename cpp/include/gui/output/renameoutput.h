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

#ifndef RENAMEOUTPUT_H_
#define RENAMEOUTPUT_H_

// Includes
//----------------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>

//-- Ui
#include <ui_outputname.h>


class RenameOutput : public QDialog, public Ui_outputname, public SceneRelatedObject {

protected:
	/// The output to be renamed's index
	int index;

 public:

  /**
   *
   * @param fsm
   * @param outputIndex The index in the list of outputs to be renamed
   * @param parent
   */
  RenameOutput(Scene * scene,int outputIndex,QWidget* parent = NULL);

  void accept();


};


#endif

