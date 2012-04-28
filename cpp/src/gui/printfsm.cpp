/***********************************************************************
*                                                                      *
* (C) 2007,Mondrian Nuessle, Computer Architecture Group,              *
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

#include "printfsm.h"
#include <QtGui/QPrintDialog>

PrintFSM::PrintFSM(QPrinter* printer,QWidget* p) :QDialog(p),Ui_printfsm()
{
  //define printdialog with default values.
  printDialog=new QPrintDialog(printer, this); 
  setupUi(this);
  connect(printerButton,SIGNAL(clicked()),this,SLOT(printer()));
}
PrintFSM::~PrintFSM()
{
  delete printDialog;
}

void PrintFSM::printer(){
  //show the print dialog

  if (singlePage->isChecked())
    printDialog->setMinMax(1,1);
  else if (doublePage->isChecked())
    printDialog->setMinMax(1,2);
  else 
    printDialog->setMinMax(1,4);

  printDialog->exec();
}
