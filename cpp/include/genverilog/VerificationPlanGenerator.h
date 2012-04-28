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

/**
 * This class generates a verification Plan for FSM to provided output stream
 * The generated file is to be used with Cadence verification tools
 */
class VerificationPlanGenerator {
public:
	VerificationPlanGenerator(Fsm * fsm);
	virtual ~VerificationPlanGenerator();


	/**
	 * Generate to output
	 *
	 * @param output
	 */
	void generate(ostream& output);


protected:

	/// FSM For which to generate verification plan
	Fsm * fsm;

	/**
	 * Cleans the input string: <br/>
	 *  - Spaces are replaced with _
	 * @param an input string
	 * @return The input string, cleaned to match verilog rules
	 */
	string cleanString(string);



};

#endif /* VERIFICATIONPLANGENERATOR_H_ */
