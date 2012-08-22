/**
 * RuleError.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
#include <FSMDesigner.h>

#include "RuleError.h"

RuleError::RuleError() {
}

RuleError::~RuleError() {
}

/*FSMDesigner::Item RuleError::getConcernedObjectType() {
    return this->concernedObjectType;
}
void RuleError::addConcernedObjectType(FSMDesigner::Item type) {
    this->concernedObjectTypes = type;
}*/

QString& RuleError::getMessage() {
    return this->message;
}

void RuleError::setMessage(QString message) {
   this->message = message;
}
