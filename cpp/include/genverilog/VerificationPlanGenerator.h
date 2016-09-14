/*
 * VerificationPlanGenerator.h
 *
 *  Created on: Oct 24, 2010
 *      Author: rleys
 */

#ifndef VERIFICATIONPLANGENERATOR_H_
#define VERIFICATIONPLANGENERATOR_H_

// Includes
//-------------

//-- Std
#include <iostream>
#include <fstream>

//-- Core
class Fsm;

//-- Generator
#include <generate/Generator.h>
#include <generate/GeneratorBuilder.h>

/**
 * This class generates a verification Plan for FSM to provided output stream
 * The generated file is to be used with Cadence verification tools
 */
class VerificationPlanGenerator : public Generator, public GeneratorBuilder {

    public:

        /**
         *
         */
        VerificationPlanGenerator();

        /**
         *
         */
        virtual ~VerificationPlanGenerator();

        /** \defgroup Builder Builder Methods */
        /** @{ */

        virtual Generator * newInstance();

        /** @} */

        /** \defgroup Generator Generator Methods */
        /** @{ */

        /**
         * Returns a short name of this Generator
         * @return
         */
        virtual QString getName();

        /**
         * Returns a short description of this Generator
         * @return
         */
        virtual QString getDescription();

        /**
         * Do the actual generation
         * @param fsm
         * @param dataStream
         */
        virtual void generate(Fsm * fsm, QDataStream * dataStream);

        /**
         * Generate an example instanciation of the created verilog module
         */
        QString createInstance(Fsm * fsm);

        /**
         * Cleans the input string: <br/>
         *  - Spaces are replaced with _
         * @param an input string
         * @return The input string, cleaned to match verilog rules
         */
        string cleanString(string);

        /** @} */




};

#endif /* VERIFICATIONPLANGENERATOR_H_ */
