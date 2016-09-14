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

#ifndef NEWOUTPUT_H_
#define NEWOUTPUT_H_

// Includes
//-------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Core
#include <core/FSMRelatedObject.h>

//-- Qt
#include <QtGui>
#include <QtCore>

//-- UI
#include <ui_outputnameselect.h>

/**
 * FIXME Support for bus names in FSM name?
 */
class NewOutput: public QDialog,
        public Ui_outputnameselect,
        public FSMRelatedObject {

    public:
        NewOutput(Fsm * fsm, QWidget * parent = NULL);

        void accept();

};

#endif
