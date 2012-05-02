/**
 * Verificator.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VERIFICATOR_H_
#define VERIFICATOR_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
class Fsm;

//-- Verification
class VerificationListener;
class VerificatorRule;

/**
 * This class Proceeds in the verification of a FSM, by calling Rules on it.
 * The rules are registered as object instances (not a factory), and execution can be followed using a Callback listener
 *
 * This class uses a singleton Pattern
 *
 */
class Verificator {

    protected:

        /// List containing all the rules to be applied
        QList<VerificatorRule *> rules;

    public:
        Verificator();
        virtual ~Verificator();

        /**
         * Register a new rule in this verificator
         * @param rule
         */
        void addRule(VerificatorRule * rule);

        /**
         * Performs Verification using all registered Rules
         * @param fsm
         * @param callbackListener
         * @return true if no errors have been seen, false otherwise
         */
        bool verify(Fsm * fsm, VerificationListener * callbackListener);


};

#endif /* VERIFICATOR_H_ */
