/**
 * UniqueIDObject.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef UNIQUEIDOBJECT_H_
#define UNIQUEIDOBJECT_H_

// Includes
//-----------------

/**
 * A unique ID object has a Unique ID attributed by an IDManager
 * It should be referenced to an ID Manager that is going to resolve IDs for all reference UniqueIDObjects
 * it has knowledge of
 */
class UniqueIDObject {

    protected:

        /// ID : 0 means not set
        unsigned long int id;

    public:
        UniqueIDObject();
        virtual ~UniqueIDObject();

        /**
         * Returns the ID of this object
         * @return the ID or 0 if not set
         */
        unsigned int getId();

        /**
         * Set the Id for this object
         * @param id
         */
        void setId(unsigned int id);

        /**
         * Tests the id: 0 means not set, not 0 means set
         * @return true if set, false if not
         */
        bool isIdSet();

};

#endif /* UNIQUEIDOBJECT_H_ */
