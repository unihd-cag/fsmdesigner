/**
 * IDManager.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef IDMANAGER_H_
#define IDMANAGER_H_

// Includes
//-----------------

//-- Std
#include <list>
#include <map>
using namespace std;

//-- IDManager
#include <core/UniqueIDObject.h>

//-- Qt
#include <QtCore>

/**
 * ID Manager takes care that every element in an FSM has unique ids
 */
class IDManager {

    private:

        /// Map to store IDs and references
        map<unsigned int,UniqueIDObject *> idToReferenceMap;

        /// Map to store Reference to IDs
        map<UniqueIDObject *,unsigned int> referenceToIdMap;

        /// Mutex to protect accesses
        QMutex mutex;


    public:
        IDManager();
        virtual ~IDManager();


        /**
         * Generates a new ID that has not been taken by somebody else
         * Keeps a reference to the assigned Object to be able to generated next IDs correctly
         *
         * This method is Thread synchonized
         * @warning When deleting UniqueIdObject, deregister it from IDManager
         * @return
         */
        unsigned int assignID(UniqueIDObject *);

        /**
         * The concerned reference is deregistered from this ID manager
         *
         * This method is Thread synchonized
         * @param
         */
        void derefenceObject(UniqueIDObject * );

};

#endif /* IDMANAGER_H_ */
