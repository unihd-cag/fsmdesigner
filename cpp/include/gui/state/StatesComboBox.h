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

#ifndef STATESCOMBOXBOX_H_
#define STATESCOMBOXBOX_H_

// Includes
//----------------------

//-- Std

//-- Core
class Fsm;
class State;
//-- Gui

//-- Qt
#include <QtGui>
#include <QtCore>

/**
 * Combobox of all states
 */
class StatesComboBox : public QComboBox {


    Q_OBJECT

    protected:
        bool wasClicked;
        bool toClose;

        // Source FSM
        Fsm * fsm;

    public:
        // the closeClicked parameter can be used to automatically close the combobox when a selection
        // has been made. This is usefull for the property editor!
        StatesComboBox(Fsm * fsm,bool closeClicked = false,State * initialState = NULL,QWidget * parent=NULL);
        virtual ~StatesComboBox();

        /**
         * Returns the pointer to the actual selected state
         * @return
         */
        State * getSelectedState();

    public slots:
        void changed(int idx);



};

#endif
