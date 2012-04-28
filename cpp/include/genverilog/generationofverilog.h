/***********************************************************************
 *                                                                      *
 * (C) 2007, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

#ifndef GENERATIONOFVERILOG_H_
#define GENERATIONOFVERILOG_H_

// Includes
//----------

//-- STD
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//-- QT
#include <QtGui>
#include <QtCore>

//-- Core
class Fsm;


class GenerationOfVerilog {

public:

	/**
	 *
	 * @param fsm
	 * @param w
	 * @param sc
	 * @param tc
	 * @param fileName File name to offer to generate to
	 * @param projectDir Default dir where to offer to generate to
	 */
	GenerationOfVerilog(Fsm * fsm, QWidget* parentWidget, bool sc = false,
			bool tc = false, QString fileName = QString(),QString projectDir = QString());

	~GenerationOfVerilog() {
	}
	;

	/**
	 * Creates the verilog and asks for a path to save
	 */
	void createVerilog();


	/**
	 * Creates an instance model for this FSM, with signal declarations
	 * @return
	 */
	string createInstance();

	/**
	 * Try to recreate verilog using last provided file path
	 * @throw Exception if no path is available
	 * @throw logic_error if an error during process occurs
	 */
	void updateVerilog();

	/**
	 * This method implements verilog generation
	 * Output file is not checked, calling methods have to do this
	 *
	 * @param outputFile Complete path to output file
	 *
	 * @throw logic_error if an error during process occurs
	 *
	 */
	void generateVerilog(string outputFile);

	/**
	 * generates a F file for the FSM verilog, with mmap definition scripts
	 * @param outputFile
	 */
	void generateFFile(string outputFile);

	/**
	 * generates a F file for the FSM verilog, with mmap definition scripts or not
	 * @param outputFile
	 * @param withMap true: Also generates the maps for states, don't generate maps for state
	 */
	void generateFFile(string outputFile,bool withMap);

	/**
	 * Generates a string representing an Mmap for simvision
	 * @return
	 */
	string generateMmap();

	/** \defgroup Forware State **/
	/** @{**/
	void setForwardState(bool forward);
	bool getForwardState();
	void setForwardStateDelayed(bool delayed);
	bool getForwardStateDelayed();
	void setForwardStateAsync(bool delayed);
	bool getForwardStateAsync();

	/** @}**/

	///Adds a new include to be added before module definition
	void addGenerateInclude(QString include);

	/**
	 * Sets the referencce to the system array with environment variables
	 * @param envp
	 */
	void setEnvp(char ** envp);

	/**
	 *  Returns the referencce to the system array with environment variables
	 * @param envp
	 */
	char** getEnvp();

private:

	/// The FSM to work with
	Fsm * fsm;

	/// The Calling Widget
	QWidget * parentWidget;

	/// Location of the project File
	QString projectDir;

	/// File Name we would like to generate to
	QString fileName;

	/// Includes to be added when generating
	QList<QString> generateIncludes;

	/// The system environment provided by the system.
	/// Will be initialized to an empty array to avoid problems
	char** envp;

	string fsmdata;
	string modulename;
	int resetstate;
	int reset;
	int clock;
	int numberofinputs;
	int numberofoutputs;
	string xorDifference;
	int desired_distance;
	int min_distance;
	int ext_min_distance;
	int extendedBy;
	int current_extension_int;
	QString current_extension_string;
	bool g;

	bool Statecov;
	bool Transcov;

	/// Forward state output: Delayed
	bool forwardStateDelayed;

	///Forward state output: async
	bool forwardStateAsync;

	/// Forward State output: normal
	bool forwardState;

	void calcHammingDistance();
	int calcExtendedHammingDistance(QString testedString);
	int calcXOR(string stringA, string stringB);
	void extendStateEncoding();
	void createStateVariations();
	QStringList originalStateOutputs;
	QStringList originalStateNames;
	vector<int> originalStateName_index;
	QStringList extendedStateEncoding;

	bool wanttogenver();

	/**
	 * Ask for a path to save verilog to
	 * @return The path
	 */
	string askVerilogFileName();

	/**
	 * Cleans the input string: <br/>
	 *  - Spaces are replaced with _
	 * @param an input string
	 * @return The input string, cleaned to match verilog rules
	 */
	string cleanString(string);


};

#endif
