/**
 * OverlappingTransitionsRule.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

// Includes
//-----------------
//-- Std
#include <sstream>
using namespace std;

//-- Qt
#include <QtCore>

//-- FSM
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>

#include "OverlappingTransitionsRule.h"

OverlappingTransitionsRule::OverlappingTransitionsRule() {
    // TODO Auto-generated constructor stub

}

OverlappingTransitionsRule::~OverlappingTransitionsRule() {
    // TODO Auto-generated destructor stub
}

QString OverlappingTransitionsRule::getName() {

    return "Overlapping Transitions' Conditions Rule";
}

QList<RuleError*> OverlappingTransitionsRule::applyRule(Fsm * fsm) {

    QList<RuleError*> result;

    // Gather All Hypertransitions
    //---------------------------


    // Foreach States transitions and conditions
    //-----------------------------
    FOREACH_STATE(fsm)

        //-- Make a copy of all the transitions which are the ones with which we make the overlapping verifications
        list    <Trans*> remainingCompareTransitions = state->getStartingTransitions();



        FOREACH_STATE_STARTING_TRANSITIONS(state)

            //-- If we are in the default transition -> skip
            if (transition->isDefault())
                continue;

            //-- For Each of this transition's conditions, compare with all conditions of all remaining Transitions
            //--
            int sourceConditionsCount = 0;
            FOREACH_TRANSITION_CONDITIONS(transition)

                for (list<Trans*>::iterator it = remainingCompareTransitions.begin();it!=remainingCompareTransitions.end();it++) {

                    //-- Take transition
                    Trans * comparingTransition = *it;

                    //-- Don't compare with a default transition
                    if (comparingTransition->isDefault())
                        continue;


                    //-- Compare with all the comparingTransition's condition
                    //------------------------------------------------------------------------
                    int comparingConditionsCount = 0;
                    for (vector<Condition*>::iterator cit = comparingTransition->getConditions().begin(); cit != comparingTransition->getConditions().end();cit++) {

                        //-- Don't compare with yourself
                        if (condition == (*cit)) {

                        }
                        //-- If comparing two same transition, all conditions with index below source conditino index
                        //-- have already been checked
                        else if (comparingTransition==transition && comparingConditionsCount<=sourceConditionsCount ) {

                        }
                        //-- Otherwise -> can try to check for overlapping
                        //--------------
                        else if(condition->isOverlappingWith((*cit)->getInput())) {

                            //-- Create Error
                            RuleError * error = new RuleError();
                            result+=error;

                            //-- Target objects are 1st source condition, then target condition
                            error->addConcernedObject(condition,FSMDesigner::CONDITION);
                            error->addConcernedObject((*cit),FSMDesigner::CONDITION);

                            //-- Message
                            stringstream message;
                            message << "State: "<< state->getName() <<", Transition: "<< transition->getName() <<" , Condition: "<< condition->getName() <<" ,with input: "<< condition->getInput() <<" overlaps with Transition: "<< comparingTransition->getName() <<" , Condition: "<< (*cit)->getName() <<" ,with input: "<< (*cit)->getInput();
                            error->setMessage(QString::fromStdString(message.str()));


                        }
                        comparingConditionsCount++;



                    } // EOF Comparing transition condition's againsts other transitions's conditions


                } // EOF Comparing against other Transitions
                sourceConditionsCount++;



                // Compare with Hypertransitions' Conditions
                //----------------------------------------
                FOREACH_HYPERTRANSITIONS(fsm)

                    //-- Foreach conditions
                    for (vector<Condition*>::iterator hcit = hypertransition->getConditions().begin();hcit!=hypertransition->getConditions().end();hcit++) {

                        ::Condition* hTransCondition = *hcit;
                        if(hTransCondition->isOverlappingWith(condition->getInput())) {

                            //-- Create Error
                            RuleError * error = new RuleError();
                            result+=error;

                            //-- Target objects are 1st source condition, then target condition
                            error->addConcernedObject(hTransCondition,FSMDesigner::HYPERTRANS);
                            error->addConcernedObject(condition,FSMDesigner::CONDITION);

                            //-- Message
                            stringstream message;
                            message << "State: "<< state->getName() <<", Transition: "<< transition->getName() <<" , Condition: "<< condition->getName() <<" ,with input: "<< condition->getInput() <<" overlaps with Hypertransition: "<< hypertransition->getName() <<" , Condition: "<< hTransCondition->getName() <<" ,with input: "<< hTransCondition->getInput();
                            error->setMessage(QString::fromStdString(message.str()));


                        }
                     }

                 END_FOREACH_HYPERTRANSITIONS
                // EOF Comparing current condition with Hypertransitions

            END_FOREACH_TRANSITION_CONDITIONS
            // EOF Transitions's conditions comparison

            //-- First Remaining Compare transition is the current one -> remove it because done
            remainingCompareTransitions.remove(transition);

        END_FOREACH_STATE_STARTING_TRANSITIONS

    END_FOREACH_STATE

    return result;

}
