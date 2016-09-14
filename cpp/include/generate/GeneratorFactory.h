/**
 * GeneratorFactory.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef GENERATORFACTORY_H_
#define GENERATORFACTORY_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>

//-- Generate
#include <generate/GeneratorBuilder.h>

/**
 * This is the generator Factory, to allow easy registration and creation of generator builder.
 *
 *
 *
 */
class GeneratorFactory {

    private:

        /**
         * Singleton reference
         */
        static GeneratorFactory factory;


        QMap<QString,GeneratorBuilder *> generatorBuilders;

        /**
         * Private constructor for Singleton
         */
        GeneratorFactory();

    public:

        virtual ~GeneratorFactory();

        /**
         * Singleton Interface
         * @return
         */
        static GeneratorFactory * getInstance();


        /**
         * Register a GeneratorBuilder mapped to a name
         * @param name
         * @param
         */
        void registerGenerator(QString name,GeneratorBuilder * generator);

        /**
         * This methods calls a registered GeneratorBuilder under the given name, to create a new Instance of a Generator.
         * @param name
         * @return The new Generator, or NULL if name does not refer to any generator
         */
        Generator * newGenerator(QString name);


};

#endif /* GENERATORFACTORY_H_ */
