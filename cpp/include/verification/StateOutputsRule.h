/**
 * StateOutputsRule.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef STATEOUTPUTSRULE_H_
#define STATEOUTPUTSRULE_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Verification
#include <verification/VerificatorRule.h>

/**
 * This Rule checks the States of the FSM all have a different output
 */
class StateOutputsRule: public VerificatorRule {
    public:
        StateOutputsRule();
        virtual ~StateOutputsRule();


        virtual QString  getName();

        /**
         * Verifies all the states have a different output
         * @param fsm
         * @return
         */
        virtual QList<RuleError*> applyRule(Fsm * fsm);


};

#endif /* STATEOUTPUTSRULE_H_ */
