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

//-- Qt
#include <QtGui>
#include <QtCore>

#include "selectinputs.h"

SelectInputs::SelectInputs() {
    setupUi(this);
    Core& c = *(Core::getInstance());
    if (c.hasFsm()) {
        Fsm& f = *(c.getProject()->getFSMs().first());
        QString qs;
        qs = QString::fromStdString(f.getResetName());
        resetline->setText(qs);
        qs = QString::fromStdString(f.getClockName());
        clockline->setText(qs);
    }
}

void SelectInputs::accept() {

    Core& c = *(Core::getInstance());
    if (c.hasFsm()) {
        Fsm& f = *(c.getProject()->getFSMs().first());
        QString qs;
        qs = resetline->text();
        FilterKeywords* fk;
        fk = new FilterKeywords();
        qs = fk->RemoveBlanks(qs);
        qs = fk->FilterSpecialChars(qs);
        qs = fk->FilterHDLKeywords(qs);
        qs = fk->FilterBusChars(qs);
        f.resetName(qs.toStdString());
        qs = clockline->text();
        qs = fk->RemoveBlanks(qs);
        qs = fk->FilterSpecialChars(qs);
        qs = fk->FilterHDLKeywords(qs);
        qs = fk->FilterBusChars(qs);
        f.clockName(qs.toStdString());
    }
    this->done(0);
}

