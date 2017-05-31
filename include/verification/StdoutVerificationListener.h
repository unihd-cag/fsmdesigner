/**
 * StdoutVerificationListener.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 * \brief This  is a verification listener that prints the results to the Std Out
 *
 */

#ifndef STDOUTVERIFICATIONLISTENER_H_
#define STDOUTVERIFICATIONLISTENER_H_

// Includes
//-----------------

#include <verification/VerificationListener.h>

class StdoutVerificationListener :  public VerificationListener {
    public:
        StdoutVerificationListener();
        virtual ~StdoutVerificationListener();


        /**
         * Notified when the verificator entered a new Rule
         * @param rule
         */
        virtual void enteredRule(VerificatorRule * rule);

        /**
         *
         * @param rule
         */
        virtual void ruleSuccessful(VerificatorRule * rule);

        /**
         * Notified when a rule execution has failed
         * @param rule
         * @param errors The List of errors that happened.
         * @warning Pointer to Errors are deleted by the verificator. Don't persist them
         */
        virtual void ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors);
};

#endif /* STDOUTVERIFICATIONLISTENER_H_ */
