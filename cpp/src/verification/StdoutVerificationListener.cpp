/**
 * StdoutVerificationListener.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
 #include <iostream>

using namespace std;

#include "StdoutVerificationListener.h"

StdoutVerificationListener::StdoutVerificationListener() {
    // TODO Auto-generated constructor stub

}

StdoutVerificationListener::~StdoutVerificationListener() {
    // TODO Auto-generated destructor stub
}

/**
 * Notified when the verificator entered a new Rule
 * @param rule
 */
void StdoutVerificationListener::enteredRule(VerificatorRule * rule) {

    cout << "-- Entering Rule " << rule->getName().toStdString() << endl;

}

/**
 *
 * @param rule
 */
void StdoutVerificationListener::ruleSuccessful(VerificatorRule * rule) {

    cout << "---- Success" << endl;

}

/**
 * Notified when a rule execution has failed
 * @param rule
 * @param errors The List of errors that happened.
 * @warning Pointer to Errors are deleted by the verificator. Don't persist them
 */
void StdoutVerificationListener::ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors) {

    for (QList<RuleError*>::iterator it = errors.begin(); it != errors.end(); it++) {

        cout << "---- Error: " << (*it)->getMessage().toStdString() << endl;
    }


}
