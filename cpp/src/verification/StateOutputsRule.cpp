/**
 * StateOutputsRule.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */
// Includes
//-----------------

//-- Std
#include <map>
#include <string>
using namespace std;

//-- Qt
#include <QtCore>

//-- Core
#include <core/Fsm.h>
#include <core/State.h>

//-- Verification
#include <verification/RuleError.h>

#include "StateOutputsRule.h"

StateOutputsRule::StateOutputsRule() {
    // TODO Auto-generated constructor stub

}

StateOutputsRule::~StateOutputsRule() {
    // TODO Auto-generated destructor stub
}

QString StateOutputsRule::getName() {
    return "Unique State Outputs";
}

QList<RuleError*> StateOutputsRule::applyRule(Fsm * fsm) {

    QList<RuleError*> errors;


    //-- Map with outputs and states
    map<string,State*> outputsMap;

    //-- Go through All State Items
    FOREACH_STATE(fsm)

        //-- Do we find Output in map
        if (outputsMap[state->getOutput()]==NULL) {

            //-- No, it is unique up to now
            outputsMap[state->getOutput()] = state;

        } else {

            //-- Already found
            //--------------

            //-- Create new error
            RuleError * error = new RuleError();
            error->setConcernedObject(state);
            error->setConcernedObjectType(FSMDesigner::STATE);

            //-- Set error message
            error->setMessage(QString("State ")+state->getName().c_str()+ " has the same output as "+(outputsMap[state->getOutput()]->getName().c_str()));
            //error->setMessage(QString("State "));

            //-- Add error to resulting list
            errors.append(error);

            //qDebug() << "Found two states with same output";
        }


    END_FOREACH_STATE



    return errors;


}
