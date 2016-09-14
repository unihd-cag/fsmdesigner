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
#include <core/Trans.h>


//-- Verification
#include <verification/logicmin.h>

//-- Qt
#include <QtGui>
#include <QtCore>

#include "min.h"

Min::Min(Fsm * fsm, QWidget* parent) :
        QDialog(parent) {
    setupUi(this);
    this->fsm = fsm;
    label->setText(
            QApplication::translate("Min", "<u>Choose Trans:</u>", 0,
                    QApplication::UnicodeUTF8));
    this->setWindowTitle(
            QApplication::translate("Min", "Minimization", 0,
                    QApplication::UnicodeUTF8));
    int count = 0;
    int i = 0;

    //-- Foreach Transitions
    FOREACH_TRANSITIONS(this->fsm)

        //-- Add id list if at least two conditions
        if (transition->getConditions().size()>=2) {
            string name = transition->getName();
            QString qs = QString::fromStdString(name);
            idlist->insertItem(i++, qs);
        }

    END_FOREACH_TRANSITIONS
    idlist->insertItem(i++, "all");

    if (count == 0) {
        okButton->setEnabled(false);
    }
}

Min::~Min() {

}

void Min::accept() {
    Fsm& f = *(this->fsm);
    vector<string> input;
    pair<string, string> result;
    string conditionInput;
    string name;
    int id;
    int i = -1;
    string transname = (idlist->currentText()).toStdString();

    // Transition "all"
    //------------------------
    if (strcmp(transname.c_str(), "all") == 0) {
        // Foreach transitions
        //---------------------------------
        FOREACH_TRANSITIONS(this->fsm)

            //-- continue If there are two conditions
            if (transition->getConditions().size()<2) {
                continue;
            }

            i = transition->getId();

            //-- Foreach transition conditions
            FOREACH_TRANSITION_CONDITIONS(transition)

                conditionInput = condition->getInput();
                string result = "";
                char X;
                for (int i = 0; i < f.getNumberOfInputs();
                        i++) {
                    X = conditionInput[i];
                    if (X == 'x' || X == 'X') {
                        result = result + '-';
                    } else {
                        result = result + X;
                    }
                }
                input.push_back(result);
            END_FOREACH_TRANSITION_CONDITIONS


            /// Remove all Conditions
            transition->getConditions().clear();
//            while (f.getTransFirstCondition()) {
//                id = f.getTransConditionID();
//                f.deleteConditionOfTrans(i, id);
//            }

            // Call logic min
            logicMin(input, result);
            int count = 1;
            conditionInput = result.first;
            int mlen = conditionInput.length();
            int len = mlen;
            if (len > 0) {

                // Readd all Conditions
                while (len > f.getNumberOfInputs()) {
                    string s = conditionInput;
                    do {
                        len--;
                    } while ((s[len] != '+') && (len != 0));
                    s.assign(s, (len + 1), (mlen - len - 1));
                    conditionInput.assign(conditionInput, 0, len);
                    Condition * addedCondition = transition->addCondition(i);
                    addedCondition->setInput(s);

                    name =
                            "min_"
                                    + (QString::number(count, 10)).toStdString();
                    count++;
                    addedCondition->setName(name);

                }
            }

            Condition * addedCondition = transition->addCondition(i);
            addedCondition->setInput(conditionInput);
            name = "min_"
                    + (QString::number(count, 10)).toStdString();
            addedCondition->setName(name);

        END_FOREACH_TRANSITIONS
    }
    // Not "all" transition
    //-------------------------
    else {
        /*if (f.getFirstTrans()) {
            do {
                if (strcmp(transname.c_str(), f.getTransName().c_str()) == 0) {
                    i = f.getTransID();
                }
            } while (f.getNextTrans());
        }
        if (i > -1) {
            f.getTransbyID(i);
            if (f.getTransFirstCondition()) {
                do {
                    condition = f.getTransConditionValue();
                    string result = "";
                    char X;
                    for (int i = 0; i < f.getNumberOfInputs(); i++) {
                        X = condition[i];
                        if (X == 'x' || X == 'X') {
                            result = result + '-';
                        } else {
                            result = result + X;
                        }
                    }
                    input.push_back(result);
                } while (f.getTransNextCondition());
            }
            while (f.getTransFirstCondition()) {
                id = f.getTransConditionID();
                f.deleteConditionOfTrans(i, id);
            }
            logicMin(input, result);
            int count = 1;
            condition = result.first;
            int mlen = condition.length();
            int len = mlen;
            if (len > 0) {
                while (len > f.getNumberOfInputs()) {
                    string s = condition;
                    do {
                        len--;
                    } while ((s[len] != '+') && (len != 0));
                    s.assign(s, (len + 1), (mlen - len - 1));
                    condition.assign(condition, 0, len);
                    f.addConditionToTrans(i);
                    id = f.getTransConditionID();
                    f.modifyTransConditionValue(i, id, s);
                    name = "min_" + (QString::number(count, 10)).toStdString();
                    count++;
                    f.modifyTrans(i, id, name);
                }
            }
            f.addConditionToTrans(i);
            id = f.getTransConditionID();
            f.modifyTransConditionValue(i, id, condition);
            name = "min_" + (QString::number(count, 10)).toStdString();
            f.modifyTrans(i, id, name);
        }*/
    }

    this->done(0);
}

