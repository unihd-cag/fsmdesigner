/***********************************************************************
 *                                                                      *
 * (C) 2007, Mondrian Nuessle, Computer Architecture Group,             *
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

#ifndef PRINTFSM_H_
#define PRINTFSM_H_

// Includes
//---------------

//-- Qt
#include <QtGui>
#include <QtCore>

#include <ui_printui.h>

class PrintFSM: public QDialog, public Ui_printfsm {
    Q_OBJECT
        ;
    public:
        PrintFSM(QPrinter* printer, QWidget* p);
        virtual ~PrintFSM();
    public slots:
        void printer();
    protected:
        QPrintDialog* printDialog;
};

#endif
