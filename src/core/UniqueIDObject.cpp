/**
 * UniqueIDObject.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//------------------


#include "UniqueIDObject.h"

UniqueIDObject::UniqueIDObject() {

    //-- Init to not set
    this->setId(0);


}

UniqueIDObject::~UniqueIDObject() {
    // TODO Auto-generated destructor stub
}


unsigned int UniqueIDObject::getId() {
    return this->id;
}


void UniqueIDObject::setId(unsigned int id) {
    this->id=id;
}

bool UniqueIDObject::isIdSet() {
    return this->id==0 ? false : true;
}

