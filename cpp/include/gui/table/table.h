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

#ifndef TABLE_H
#define TABLE_H

// Includes
//-----------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>

//-- Ui
class TableStateModel;
class TableStateDelegate;
class TableTransModel;
class TableTransDelegate;

//-- Scene
#include <gui/scene/SceneRelatedObject.h>

#include <ui_tables.h>

/**
 * A table showing states and transitions of an FSM
 * \warning Always create a new Table for every usage. The setRelatedScene doesn't propagate correctly to all functions
 */
class Table: public QDialog, public Ui_Tables , public SceneRelatedObject {

    Q_OBJECT

    private:

    /// Index of the scene's undo stack, to undo everything convienently when reject is pressed
    int undoIndex;

    public:
        Table(Scene * scene,QWidget* parent = NULL);
        virtual ~Table();

        void accept();
        void reject();

    public slots:
        void addState();
        void deleteState();
        void addTrans();
        void deleteTrans();
        void addCondition();
        void deleteCondition();

        void checkTransitionsOverlapping();


    private:
        void createconnections();

};

#endif
