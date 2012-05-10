/**
 * StateTransitions.cpp
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
#include <core/Trans.h>

//-- Verification
#include <verification/RuleError.h>

#include "StateTransitions.h"

StateTransitions::StateTransitions() {

}

StateTransitions::~StateTransitions() {
}

QString StateTransitions::getName() {
    return "StateTransitions";
}


QList<RuleError*> StateTransitions::applyRule(Fsm * fsm) {
    QList<RuleError*> result;

    // Foreach States transitions and conditions
    //-----------------------------
    FOREACH_STATE(fsm)

        //-- Have we seen a default transition ?
        bool  hasDefaultTransition = false;
        bool  hasMultipleDefaultTransition = false;

        FOREACH_STATE_STARTING_TRANSITIONS(state)

            //-- Default transition check
            if (transition->isDefault()) {
                if (hasDefaultTransition)
                    hasMultipleDefaultTransition = true;
                hasDefaultTransition = true;

                //-- If transition is default and has condition -> fail
                if (transition->getConditions().size()>0) {
                    RuleError * error = new RuleError();
                    error->setConcernedObject(state);
                    error->setConcernedObjectType(FSMDesigner::STATE);
                    error->setMessage(QString("State ")+state->getName().c_str()+" default transition's has conditions");
                    result.append(error);
                }

            } else {

                //-- If transition is not default and has no conditions -> fails
                if (transition->getConditions().size()==0) {
                    RuleError * error = new RuleError();
                    error->setConcernedObject(state);
                    error->setConcernedObjectType(FSMDesigner::STATE);
                    error->setMessage(QString("State ")+state->getName().c_str()+" transition "+transition->getName().c_str()+" is not default and has no conditions");
                    result.append(error);
                }

                //-- Verify Conditions don't overlap
                FOREACH_TRANSITION_CONDITIONS(transition)

                END_FOREACH_TRANSITION_CONDITIONS
            }



        END_FOREACH_STATE_STARTING_TRANSITIONS

        //-- Add error if no default transition
        if (!hasDefaultTransition) {
            RuleError * error = new RuleError();
            error->setConcernedObject(state);
            error->setConcernedObjectType(FSMDesigner::STATE);
            error->setMessage(QString("State ")+state->getName().c_str()+" has no default transition");
            result.append(error);
        }
        //-- Add error if more than one default transition
        else if (hasMultipleDefaultTransition) {
            RuleError * error = new RuleError();
            error->setConcernedObject(state);
            error->setConcernedObjectType(FSMDesigner::STATE);
            error->setMessage(QString("State ")+state->getName().c_str()+" has multiple default transitions");
            result.append(error);
        }

    END_FOREACH_STATE


    return result;
}
