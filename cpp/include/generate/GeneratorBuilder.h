/**
 * GeneratorBuilder.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef GENERATORBUILDER_H_
#define GENERATORBUILDER_H_

// Includes
//-----------------


//-- Generate
#include <generate/Generator.h>

/**
 * Pure virtual class to build Generators of a specific Type
 *
 * This class is though to be implemented on the same class that implements the Generator,
 * to limit the implementation overhead
 *
 */
class GeneratorBuilder {

    public:
        GeneratorBuilder() {

        }
        virtual ~GeneratorBuilder(){

        }

        /**
         * Implemented by GeneratorBuilder to create an instance of a generator.
         *
         * The returned instance MUST be unique and can be deleted by the user
         *
         * @return
         */
        virtual Generator * newInstance() = 0;

};

#endif /* GENERATORBUILDER_H_ */
