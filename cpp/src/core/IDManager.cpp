/**
 * IDManager.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-------------------

//-- Std
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <typeinfo>
#include <time.h>
using namespace std;

//-- Qt
#include <QtCore>


#include "IDManager.h"

IDManager::IDManager() {

    //-- Ensure srand has been called
    srand(time(0));
    this->idToReferenceMap.clear();
    this->referenceToIdMap.clear();

}

IDManager::~IDManager() {
    // TODO Auto-generated destructor stub
}


unsigned int IDManager::assignID(UniqueIDObject * object) {

    QMutexLocker locker(&(this->mutex));

    // Verify the object is not already referenced here
    // If it is the case, throw an exception
    //-----------------------------------
    if (this->referenceToIdMap.count(object)>0) {
        stringstream ss;
        ss << "Trying to assign an ID to an object (type:"<<typeid(*object).name() <<", id: "<< object->getId() <<", count: "<< this->referenceToIdMap.count(object) <<",ptr: "<< object << ") that has already been assigned in the IDManager";
        throw invalid_argument(ss.str());
    }

    // FIXME If object already has an ID -> record the reference
    //----------------------------------
    if (object->isIdSet()) {
        //-- Reference and return
        this->idToReferenceMap[object->getId()] = object;
        this->referenceToIdMap[object] = object->getId();
        return object->getId();
    }

    // Generate and ID and check if it is in the map
    //-------------------------

    unsigned int id = 1;
    while (true) {

        //-- Generate
        //unsigned long int id = abs(random());

        //-- Test if nothing registered at given ID, it's okay
        if ( this->idToReferenceMap.count(id)==0) {

            //-- Reference and break
            object->setId(id);
            this->idToReferenceMap[id] = object;
            this->referenceToIdMap[object] = id;

            //cout << "Assigned id " << id << "to object pointing at: " << object << endl;

            break;
        }

        //-- If up to here, then try again
        id++;
    }

    return id;
}


void IDManager::derefenceObject(UniqueIDObject * reference) {

    QMutexLocker locker(&(this->mutex));

    //-- Find objects ID
    if (this->referenceToIdMap.count(reference)>0) {

        //-- Delete
        this->idToReferenceMap.erase(this->referenceToIdMap[reference]);
        this->referenceToIdMap.erase(reference);

        //-- FIXME Remove ID from Object
        reference->setId(0);

    }



}
