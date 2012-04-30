/**
 * VerificationListener.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VERIFICATIONLISTENER_H_
#define VERIFICATIONLISTENER_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Verification
#include <verification/VerificatorRule.h>
#include <verification/RuleError.h>

/**
 * This is the virtual class to be implemented as callback to a verification run
 */
class VerificationListener {
    public:
        VerificationListener();
        virtual ~VerificationListener();

        /**
         * Notified when the verificator entered a new Rule
         * @param rule
         */
        virtual void enteredRule(VerificatorRule * rule) = 0;

        /**
         *
         * @param rule
         */
        virtual void ruleSuccessful(VerificatorRule * rule) = 0;

        /**
         * Notified when a rule execution has failed
         * @param rule
         * @param errors The List of errors that happened.
         * @warning Pointer to Errors are deleted by the verificator. Don't persist them
         */
        virtual void ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors) = 0;

};

#endif /* VERIFICATIONLISTENER_H_ */
