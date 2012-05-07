/**
 * Generator.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */


// Includes
//------------------

//-- Qt
#include <QtCore>

//-- Generate

#include "Generator.h"

Generator::Generator() {

}

Generator::~Generator() {

}


QVariant Generator::getParameter(QString name) {

    QVariant res;
    if (this->parametersMap.contains(name)) {
        return this->parametersMap[name];
    } else
        return res;

}


void Generator::setParameter(QString name,QVariant value) {

    if (value.isValid()) {
        this->parametersMap[name] = value;
    }

}

