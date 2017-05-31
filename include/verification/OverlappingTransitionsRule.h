/**
 * OverlappingTransitionsRule.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef OVERLAPPINGTRANSITIONSRULE_H_
#define OVERLAPPINGTRANSITIONSRULE_H_

// Includes
//-----------------



#include "VerificatorRule.h"

/**
 * This rule verifies that for each State, none of its transitions + hypertransition are overlapping
 */
class OverlappingTransitionsRule: public VerificatorRule {
    public:
        OverlappingTransitionsRule();
        virtual ~OverlappingTransitionsRule();

        virtual QString getName();

        /**
         * Verifies all the overlapping transitions + hypertransitions
         * @param fsm
         * @return
         */
        virtual QList<RuleError*> applyRule(Fsm * fsm);



};

#endif /* OVERLAPPINGTRANSITIONSRULE_H_ */
