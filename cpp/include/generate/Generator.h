/**
 * Generator.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Core
class Fsm;

/**
 * This class is the virtual interface for a Generator
 *
 */
class Generator {

    protected:

        /**
         * Map to store parameters for the generator
         */
        QMap<QString,QVariant> parametersMap;

    public:
        Generator();
        virtual ~Generator();

        /**
         * Returns a short name of this Generator
         * @return
         */
        virtual QString getName() = 0;

        /**
         * Returns a short description of this Generator
         * @return
         */
        virtual QString getDescription() = 0;


        /**
         * Perform Generation
         * @param fsm
         * @param dataStream
         */
        virtual void generate(Fsm * fsm,QDataStream * dataStream) = 0;


        /** \defgroup Parameters Parameters Manipulation Utilities */
        /** @{ */

        /**
         * Returns a reference to the recorded Variant
         * @param name
         * @return An invalid variant if nothing has been recorded for the parameter
         */
        QVariant& getParameter(QString name);

        /**
         * Set the value for the specified parameter.
         * Does nothing if the QVariant is invalid
         * @param name
         * @param value An invalid QVariant is ignored
         */
        void setParameter(QString name,QVariant value);


        /** @} */


};

#endif /* GENERATOR_H_ */
