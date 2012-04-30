/**
 * RuleError.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef RULEERROR_H_
#define RULEERROR_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
#include <FSMDesigner.h>

/**
 * This represent an error occured while executing a verification rule.
 * It holds enough information for the user to find back what king of objects where concerned and such
 *
 */
class RuleError {

    protected:

        /**
         * The Object from Core has commited to an error
         */
        void * concernedObject;

        /**
         * The type of the concerned Object
         */
        FSMDesigner::Item concernedObjectType;

        /**
         * The content of the error
         */
        QString message;

    public:
        RuleError();
        virtual ~RuleError();


        template<class T> T * getConcernedObject(){
            return (T*)this->concernedObject;
        }
        template<class T> void setConcernedObject(T * object){
            this->concernedObject = object;
        }

       /* void * getConcernedObject();
        void setConcernedObject(void * object);*/

        FSMDesigner::Item getConcernedObjectType();
        void setConcernedObjectType(FSMDesigner::Item type);

        QString& getMessage();
        void setMessage(QString message);


};

#endif /* RULEERROR_H_ */
