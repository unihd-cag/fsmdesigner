/**
 * VerificatorRule.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VERIFICATORRULE_H_
#define VERIFICATORRULE_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
class Fsm;

//-- Verification
#include <verification/RuleError.h>

/**
 * This is the base virtual class for Implementation of Rules
 */
class VerificatorRule {

    protected:

        /// A read-only name to nicely identify this rule
        QString name;

    public:
        VerificatorRule();
        virtual ~VerificatorRule() ;


        /**
         * Return the name of this rule
         * @return
         */
        virtual QString getName() = 0;

        /**
         * Applies the Rule
         *
         * @param fsm
         * @return A List a Errors as result of the rule run. The verificator will clean the created objects
         */
        virtual QList<RuleError*> applyRule(Fsm * fsm) = 0;




};

#endif /* VERIFICATORRULE_H_ */
