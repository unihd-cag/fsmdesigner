/**
 * VPlanGenerator.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VPLANGENERATOR_H_
#define VPLANGENERATOR_H_

// Includes
//-----------------

#include "Generator.h"
#include "GeneratorBuilder.h"

class VPlanGenerator: public Generator, public GeneratorBuilder<VPlanGenerator> {
    public:
        VPlanGenerator();
        virtual ~VPlanGenerator();

        /** \defgroup Builder Builder Methods */
        /** @{ */

        virtual  VPlanGenerator * newInstance();

        /** @} */

        /** \defgroup Generator Generator Methods */
        /** @{ */

        virtual void generate(QDataStream * dataStream);

        /** @} */

};

#endif /* VPLANGENERATOR_H_ */
