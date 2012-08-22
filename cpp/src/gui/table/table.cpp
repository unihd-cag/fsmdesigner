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

//-- Scene
#include <gui/scene/SceneRelatedObject.h>
#include <gui/scene/Scene.h>

//-- Verify
#include <verification/Verificator.h>
#include <verification/OverlappingTransitionsRule.h>

#include "table.h"
Table::Table(Scene * scene, QWidget* parent) :
        QDialog(parent), SceneRelatedObject(scene) {
    setupUi(this);


    // Propagate Scene
    //----------------------
    this->StateView->setRelatedScene(scene);
    this->TransitionView->setRelatedScene(scene);


    // Record Index in stack, to allow undoing all that has been done if reject
    //-------------------
    this->undoIndex = this->getRelatedScene()->getUndoStack()->index();


    // Set Icon on Transitions Page Check button
    //---------
    this->checkTransitionsButton->setIcon(QIcon(":/icons/Check"));
    this->connect(this->checkTransitionsButton,SIGNAL(clicked ()),SLOT(checkTransitionsOverlapping()));

}

Table::~Table() {

}


void Table::checkTransitionsOverlapping() {

    qDebug() << "Verifing overlapping";

    // Create Verificator
    //--------------------------
    Verificator verificator;
    verificator.addRule(new OverlappingTransitionsRule());
    verificator.verify(this->getRelatedScene()->getFsm(),this->TransitionView);

}

void Table::createconnections() {

    /*
    connect(StateView->addState, SIGNAL(triggered()), this, SLOT(addState()));
    connect(StateView->deleteState, SIGNAL(triggered()), this,
            SLOT(deleteState()));
    connect(TransitionView->addTrans, SIGNAL(triggered()), this,
            SLOT(addTrans()));
    connect(TransitionView->deleteTrans, SIGNAL(triggered()), this,
            SLOT(deleteTrans()));
    connect(TransitionView->addCondition, SIGNAL(triggered()), this,
            SLOT(addCondition()));
    connect(TransitionView->deleteCondition, SIGNAL(triggered()), this,
            SLOT(deleteCondition()));
            */
}

void Table::addState() {
/*
    Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    f.addState();
    int id = f.getStateID();
    f.modifyState(id, (double) 50, (double) 50);
    unsigned int color = dynamic_cast<MainguiWindow*>(wid)->stateColor().rgb();
    f.modifyState(id, color);
    StateView->doItemsLayout();
    StateView->sortByColumn(2, Qt::AscendingOrder);
    dynamic_cast<TableStateModel*>(StateView->model())->sort(1,
            Qt::AscendingOrder);
            */
}

void Table::deleteState() {

    /*
    Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    int counter = 0;
    int deleteid;
    if (f.getFirstState()) {
        do {
            if (counter == (StateView->currentIndex()).row()) {
                deleteid = f.getStateID();
            }
            counter++;
        } while (f.getNextState());
        f.deleteState(deleteid);
    }
    StateView->doItemsLayout();
    TransitionView->doItemsLayout();
    */
}

void Table::addTrans() {
    /*
    Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    f.addTrans(-1, -1);
    TransitionView->doItemsLayout();*/
}

void Table::deleteTrans() {
    /*
    Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    int counter = 0;
    int deleteid;
    if (f.getFirstTrans()) {
        do {
            if (counter == (TransitionView->currentIndex()).row()) {
                deleteid = f.getTransID();
            }
            counter++;
        } while (f.getNextTrans());
        f.deleteTrans(deleteid);
    }
    TransitionView->doItemsLayout();*/
}

void Table::addCondition() {
    /*Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    int counter = 0;
    int id;
    if (f.getFirstTrans()) {
        do {
            if (counter == (TransitionView->currentIndex()).row()) {
                id = f.getTransID();
            }
            counter++;
        } while (f.getNextTrans());
        f.addConditionToTrans(id);
    }
    TransitionView->doItemsLayout();*/
}

void Table::deleteCondition() {
   /* Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    int counter = 0;
    int id;
    if (f.getFirstTrans()) {
        do {
            if (counter == (TransitionView->currentIndex()).parent().row()) {
                id = f.getTransID();
            }
            counter++;
        } while (f.getNextTrans());
    }
    f.getTransbyID(id);
    counter = 0;
    int cid;
    if (f.getTransFirstCondition()) {
        do {
            if (counter == (TransitionView->currentIndex()).row()) {
                cid = f.getTransConditionID();
            }
            counter++;
        } while (f.getTransNextCondition());
        f.deleteConditionOfTrans(id, cid);
    }
    TransitionView->doItemsLayout();*/
}

void Table::accept() {
    /*Fsm& f = *(CoreFactory::getCore().getProject()->getSelectedFSM());
    int id = -1;
    bool found = true;
    while (found) {
        found = false;
        if (f.getFirstTrans()) {
            do {
                if (!f.getTransFirstTrackpoint()) {
                    if (f.getTransStart() == -1) {
                        id = f.getTransID();
                        found = true;
                    }
                    if (f.getTransEnd() == -1) {
                        id = f.getTransID();
                        found = true;
                    }
                }
            } while (f.getNextTrans() && !found);
        }
        if (found) {
            f.deleteTrans(id);
            id = -1;
        }
    }
    dynamic_cast<MainguiWindow*>(wid)->refresh();
    this->done(0);*/

    //-- Quit
    this->done(0);
}

void Table::reject() {

    //-- Reset UNdo stack to initial index
    this->getRelatedScene()->getUndoStack()->setIndex(this->undoIndex);

    //-- Quit
    this->done(0);
}
