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
#include <gui/common/filterkeywords.h>

#include <gui/fsm/newfsm.h>

//-- Qt
#include <QtGui>
#include <QtCore>


#include "newinputinit.h"
NewInputInit::NewInputInit(QWidget* parent) : QDialog(parent) {
  setupUi(this);
}


NewInputInit::~NewInputInit() {

}

void NewInputInit::accept() {
    QString qs = lineEditInput->text();
    FilterKeywords* fk = new FilterKeywords();
    qs = fk->FilterSpecialChars(qs);
    qs = fk->FilterHDLKeywords(qs);
    string name = qs.toStdString();
    Fsm& f = *(Core::getInstance()->getProject()->getSelectedFSM());
    while(!name.empty()) {
        string::size_type len = name.length();
        string::size_type sty = name.find_first_not_of(' ',0);
        string::size_type sty2 = name.find_first_of(' ',sty);
        if((sty != string::npos) && (sty2 != string::npos) ) {
            string next;
            next.assign(name,sty,(sty2 - sty));
            QStringList nextList = fk->ExpandBusNames(next);
            for (int sli = 0; sli < nextList.size(); ++sli)
            {
#ifdef DEBUGNEWINPUT
                qDebug()<<"Bus name to be included: "<<nextList.at(sli).toLocal8Bit().constData()  ;
#endif
                QString currentofNextListQS = nextList.at(sli).toLocal8Bit().constData();
                currentofNextListQS = fk->FilterBusChars(currentofNextListQS);
                string currentofNextLists = currentofNextListQS.toStdString();
                int i = f.getNumberOfInputs();
                i++;
//                f.numberOfIns(i, -1000);
//                f.addInputName();
//                f.inputName(i - 1, currentofNextLists);
                //qs = QString::fromStdString(next);
                (dynamic_cast<NewFsm*>(this->parent()))->inText(currentofNextListQS);
            }
            name.assign(name,sty2, len - 1);
        } else {
            if(sty != string::npos ) {
                name.assign(name,sty, (len - sty));
                QStringList nextList = fk->ExpandBusNames(name);
                for (int sli = 0; sli < nextList.size(); ++sli)
                {
#ifdef DEBUGNEWINPUTINIT
                    qDebug()<<"Bus name to be included: "<<nextList.at(sli).toLocal8Bit().constData()  ;
#endif
                    QString currentofNextListQS = nextList.at(sli).toLocal8Bit().constData();
                    currentofNextListQS = fk->FilterBusChars(currentofNextListQS);
                    string currentofNextLists = currentofNextListQS.toStdString();
                    int i = f.getNumberOfInputs();
                    i++;
//                    f.numberOfIns(i, -1000);
//                    f.addInputName();
//                    f.inputName(i - 1, currentofNextLists);
                    //qs = QString::fromStdString(name);
                    (dynamic_cast<NewFsm*>(this->parent()))->inText(currentofNextListQS);
                }
            }
            name = "";
        }
    }
    this->close();
}
