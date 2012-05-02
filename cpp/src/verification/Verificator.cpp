/**
 * Verificator.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
#include <core/Fsm.h>

//-- Verification
#include <verification/VerificationListener.h>
#include <verification/VerificatorRule.h>
#include <verification/RuleError.h>

#include "Verificator.h"

Verificator::Verificator() {
    // TODO Auto-generated constructor stub

}

Verificator::~Verificator() {
    // TODO Auto-generated destructor stub
}

void Verificator::addRule(VerificatorRule * rule) {
    if (rule != NULL)
        this->rules.append(rule);
}

bool Verificator::verify(Fsm * fsm, VerificationListener * callbackListener) {

    bool result = true;

    // Foreach All Rules
    //--------------------------
    qDebug() << "There are " << this->rules.size() << " to apply";
    for (int i = 0 ; i < this->rules.size() ; i++) {

        // Pick Rule
        VerificatorRule * rule = this->rules.at(i);

        // Callback: Enter
        //----------------------
        if (callbackListener!=NULL)
            callbackListener->enteredRule(rule);

        // Execute
        //----------------
        QList<RuleError*> errors = rule->applyRule(fsm);

        //-- if there are errors -> return false
        if (errors.size()>0)
            result = false;

        // Callback:
        //----------------
        if (callbackListener!=NULL && errors.size() == 0) {

            //----- Success (errors list is empty)
            callbackListener->ruleSuccessful(rule);

        } else if (callbackListener!=NULL) {

            //----- Error (errors list is not empty)
            callbackListener->ruleFailed(rule,errors);

        }


        // Clean Errors
        //--------------
        while (!errors.isEmpty()) {
            delete errors.takeFirst();
        }


    }


    return result;

}

