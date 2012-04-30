/**
 * GeneratorFactory.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//----------------------

//-- Qt
#include <QtCore>

//-- Generate
#include <generate/GeneratorBuilder.h>

#include "GeneratorFactory.h"

// Singleton initialisation
//--------------------------------
GeneratorFactory GeneratorFactory::factory;

GeneratorFactory::GeneratorFactory() {


}

GeneratorFactory::~GeneratorFactory() {

}

GeneratorFactory * GeneratorFactory::getInstance() {

    return &factory;

}

void GeneratorFactory::registerGenerator(QString name,GeneratorBuilder * generator) {

    this->generatorBuilders[name] = generator;

}

Generator * GeneratorFactory::newGenerator(QString name) {

    Generator * result = NULL;

    //-- Generate only if there is a generator registered
    if (this->generatorBuilders.contains(name)) {

        result = (this->generatorBuilders[name])->newInstance();
    }


    return result;

}

