/**
 * StateTransitions.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef STATETRANSITIONS_H_
#define STATETRANSITIONS_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Verification
#include <verification/VerificatorRule.h>

/**
 * This Rule checks every state has:
 *
 *  - One and only one default transition
 *  - no non default transition without condition
 *  - no conditions of all outgoing transitions are overlapping
 */
class StateTransitions: public VerificatorRule {
    public:
        StateTransitions();
        virtual ~StateTransitions();

        virtual QString getName();

        /**
         * Verifies all the states have a different output
         * @param fsm
         * @return
         */
        virtual QList<RuleError*> applyRule(Fsm * fsm);
};

#endif /* STATETRANSITIONS_H_ */
