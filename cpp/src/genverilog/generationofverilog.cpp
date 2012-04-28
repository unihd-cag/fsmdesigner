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

// Includes
//----------
//-- STD
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <map>

using namespace std;

//-- QT
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/Core.h>
#include <core/Load_I.h>
#include <core/LoadFactory.h>
#include <core/Utils.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/Trans.h>
#include <core/State.h>
#include <core/Link.h>

//-- Verification
#include <verification/verify.h>
#include <verification/logicmin.h>
#include <verification/invertDNF.h>

#include "generationofverilog.h"

GenerationOfVerilog::GenerationOfVerilog(Fsm * fsm, QWidget * parentWidget,
        bool sc, bool tc, QString fileName, QString projectDir) {

    //-- Save arguments
    this->fsm = fsm;
    this->parentWidget = parentWidget;
    this->projectDir = projectDir;
    this->fileName = fileName;
    this->Statecov = sc;
    this->Transcov = tc;

    //-- Init
    this->resetstate = 0;
    this->reset = 0;
    this->clock = 0;
    this->numberofinputs = 0;
    this->numberofoutputs = 0;
    this->g = false;

    // System environment
    this->envp = NULL;

    // Forward feature
    this->forwardStateAsync = false;
    this->forwardStateDelayed = false;
    this->forwardState = false;

}

bool GenerationOfVerilog::wanttogenver() {
    int genver = QMessageBox::question(this->parentWidget, "Generate Verilog",
            "Do you still want to generate Verilog?", QMessageBox::Yes,
            QMessageBox::Cancel, QMessageBox::NoButton);

    if (genver == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

string GenerationOfVerilog::askVerilogFileName() {

    // Ask where to save
    //------------
    QString startDirectory = this->projectDir;

    // Detect possible directory using last generated parameter
    if (this->fsm->getLastGeneratedVerilogFile().length() > 0) {

        // Create QFile to extract the folder
        QFileInfo lastFile = QFileInfo(
                QString(this->fsm->getLastGeneratedVerilogFile().c_str()));
        startDirectory = lastFile.dir().currentPath();

    }

    //-- If there is a fileName offer, include it
    if (this->fileName.length() > 0) {
        startDirectory += "/" + this->fileName;
    }

    // File name proposition
    QString file = QFileDialog::getSaveFileName(this->parentWidget,
            QString("Choose a file to generate Verilog to"), startDirectory,
            QString("Verilog file (*.v)"));

    return file.toStdString();

}

void GenerationOfVerilog::updateVerilog() {

    //-- Check we have a path
    if (this->fsm->getLastGeneratedVerilogFile().length() == 0) {
       return;
    }

    //-- CreateVerilog
    this->generateVerilog(this->fsm->getLastGeneratedVerilogFile());

}

void GenerationOfVerilog::createVerilog() {

    /// Data initialization
    cout << "Creating verilog" << endl;
    Fsm& f = *(this->fsm);

    //-- ASk for path to save to
    string filename = this->askVerilogFileName();
    if (!filename.empty()) {

        // Generate
        this->generateVerilog(filename);

    }
}

void GenerationOfVerilog::generateVerilog(string outputFile) {

    //-- Open Target file
    ofstream out(outputFile.c_str(), ios_base::out | ios_base::trunc);
    if (out.is_open()) {

        int maxlen = outputFile.length();
        int len = maxlen;
        if (len > 0) {
            string s;
            s = outputFile;
            do {
                len--;
            } while ((s[len] != '/') && (len != 0));
            if (len == 0) {
                modulename.assign(s, 0, maxlen - 2);
            } else {
                modulename.assign(s, (len + 1), (maxlen - len - 3));
            }
        }
        numberofinputs = this->fsm->getNumberOfInputs();
        numberofoutputs = this->fsm->getNumberOfOutputs();
        resetstate = this->fsm->getResetState();

        ///errordetection

        if (numberofinputs == 0 || numberofoutputs == 0) {
            cout << "Error: illegal input or output length" << endl;
            out.close();
        } else {

            //calcHammingDistance();
            //extendStateEncoding();
            //createStateVariations();

            // Write license
            //-----------------
            out << "/**" << endl;

            //FIXME  Write Instance pattern
            //----------------------------
            string generatedInstance = this->createInstance();
            out << " " << generatedInstance << endl;
            out << " */" << endl;

            // Write includes
            //----------------------
            for (int i = 0; i < this->generateIncludes.length(); i++) {
                out << "`include \""
                        << this->generateIncludes.at(i).toStdString() << "\""
                        << endl;
            }

            // Write module header (I/O)
            //----------------------------
            out << "module " << modulename << " ( " << endl;
            out << "    input wire " << this->fsm->getClockName() << ", "
                    << endl;
            out << "    input wire " << this->fsm->getResetName() << ", "
                    << endl << endl;

            //---- Input
            out << "    // Inputs" << endl << "    //------------ " << endl;
            for (int i = 0; i < numberofinputs; i++) {
                out << "    input wire " << this->fsm->getInputName(i) << ", "
                        << endl;
            }
            out << endl;

            //---- Outputs
            out << "    // Outputs" << endl << "    //------------ " << endl;
            for (int i = 0; i < numberofoutputs - 1; i++) {
                out << "    output wire " << this->fsm->getOutputName(i) << ", "
                        << endl;
            }
            out << "    output wire "
                    << this->fsm->getOutputName(numberofoutputs - 1);

            //---- Forward declaration?
            if (this->getForwardStateAsync()) {
                out << ", " << endl << endl;
                out << "    //-- Async Forward state" << endl;
                out << "    output wire [" << (numberofoutputs - 1)
                        << ":0] forward_state_async";

            }
            if (this->getForwardStateDelayed()) {
                out << ", " << endl << endl;
                out << "    //-- Delayed Forward state" << endl;
                out << "    output reg [" << (numberofoutputs - 1)
                        << ":0] forward_state_delayed ";

            }
            if (this->getForwardState()) {
                out << ", " << endl << endl;
                out << "    //-- Forwarded state (Actual State)" << endl;
                out << "    output wire [" << (numberofoutputs - 1)
                        << ":0] forward_state ";
            }
            out << endl;

            out << " );" << endl;
            out << endl;

            // Write Parameters (States)
            //-----------------------------
            FOREACH_STATE(this->fsm)
                out << "localparam " << state->getName() << " = "
                        << numberofoutputs << "'b" << state->getOutput() << ";"
                        << endl;

            END_FOREACH_STATE

            // current_state Declaration
            //----------------------------------
            out << endl << "reg [" << (numberofoutputs - 1)
                    << ":0] current_state, next_state;" << endl;
            out << "assign {";
            for (int i = 0; i < numberofoutputs - 1; i++) {
                out << this->fsm->getOutputName(i) << ", ";
            }
            out << this->fsm->getOutputName(numberofoutputs - 1);
            out << "} = current_state;" << endl;

            //-- If Async Forward declaration, forward_state_async is the next_state
            if (this->getForwardStateAsync()) {
                out << "assign forward_state_async = next_state;" << endl;
            }
            //-- If normal forward, forward_state is current_state
            if (this->getForwardState()) {
                out << "assign forward_state = current_state;" << endl;
            }
            out << endl;

            // input vector declaration
            //----------------------
            out << "wire [" << (numberofinputs - 1) << ":0] inputvector;"
                    << endl;
            out << "assign inputvector = {";
            for (int i = 0; i < numberofinputs - 1; i++) {
                out << this->fsm->getInputName(i) << ", ";
            }
            out << this->fsm->getInputName(numberofinputs - 1);
            out << "};" << endl << endl << endl;

            // Write combinational logic always-block
            //--------------------------------------
            out << "always @(*) begin" << endl;

            //-- If we are in delayed forward mode, use the forward_state declaration for computing
            if (this->getForwardStateDelayed()) {
                out << "  casex({inputvector, forward_state_delayed})" << endl;
            } else {
                out << "  casex({inputvector, current_state})" << endl;
            }

            // Foreach State
            //------------------------
            FOREACH_STATE(this->fsm)

                cout << "Working on State " << state->getName() << endl;

                //---- Foreach Transitions that start from this state
                //-----------------
                vector<string> input;

                //FOREACH_TRANSITIONS(this->fsm)

                // Determine Inputs for the transitions starting from this state, and non default
                //  - The condition vector is then minimized to determine all the cases matching default transition
                //---------------------
                State * targetDefaultState = NULL;
                FOREACH_STATE_STARTING_TRANSITIONS(state)

                    //-- Recpord default transition target
                    int transid = transition->getId();
                    targetDefaultState = transition->isDefault() ? transition->getEndState() : targetDefaultState;

                    //-- Non default transition input is stacked for minimization
                    if (!transition->isDefault()) {

                        //-- Foreach Conditions
                        FOREACH_TRANSITION_CONDITIONS(transition)

                            string conditionInput =
                                    condition->getInput();
                            string result = "";
                            char X;
                            for (int i = 0;
                                    i
                                            < this->fsm->getNumberOfInputs();
                                    i++) {
                                X = conditionInput[i];
                                if (X == 'x' || X == 'X') {
                                    result = result + '-';
                                } else {
                                    result = result + X;
                                }
                            }
                            //input.push_back(result);

                        END_FOREACH_TRANSITION_CONDITIONS


                    }
                END_FOREACH_STATE_STARTING_TRANSITIONS

                // Determine input for hypertrans
                //---------------------
                FOREACH_HYPERTRANSITIONS(fsm)

                    FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

                        string conditionInput =
                                condition->getInput();
                        string result = "";
                        char X;
                        for (int i = 0;
                                i < fsm->getNumberOfInputs(); i++) {
                            X = conditionInput[i];
                            if (X == 'x' || X == 'X') {
                                result = result + '-';
                            } else {
                                result = result + X;
                            }
                        }
                        input.push_back(result);

                    END_FOREACH_HYPERTRANSITION_CONDITIONS
                END_FOREACH_HYPERTRANSITIONS

                // Minimize input vector to find out bit conditions leading to default transition
                //-------------------------
                bool notfound = true;
                if (input.size() > 0) {

                    // use InverDNF to find the minimal bit vector not matching the ones that should be used as input
                    //-----------------------
                    InvertDNF INV;
                    string defaultvalue = INV.invert(input);
                    //string defaultvalue = input[0];
                    cout << "\tDefault value: " << defaultvalue
                            << endl;
                    int maxlen = defaultvalue.length();
                    int len = maxlen;
                    if (len > 0) {

                        out << "    {" << this->fsm->getNumberOfInputs() << "'b";

                        while (len > this->fsm->getNumberOfInputs()) {
                            string s = defaultvalue;
                            do {
                                len--;
                            } while ((s[len] != '+') && (len != 0));
                            s.assign(s, (len + 1),
                                    (maxlen - len - 1));
                            defaultvalue.assign(defaultvalue, 0,
                                    len);
                            char X;
                            for (int i = 0; i < this->fsm->getNumberOfInputs();
                                    i++) {
                                X = s[i];
                                if (X == '-' || X == 'X') {
                                    out << "x";
                                } else {
                                    out << X;
                                }
                            }
                            out
                                    << ", "
                                    << state->getName()
                                    << "}:   next_state = "
                                    << this->cleanString(
                                            targetDefaultState->getName()) << ";"
                                    << endl;
                            out << "    {" << this->fsm->getNumberOfInputs()
                                    << "'b";
                        }

                        char X;
                        for (int i = 0; i < this->fsm->getNumberOfInputs(); i++) {

                            X = defaultvalue[i];
                            if (X == '-' || X == 'X') {
                                out << "x";
                            } else {
                                out << X;
                            }
                        }

                        out
                                << ", "
                                << state->getName()
                                << "}:   next_state = "
                                << this->cleanString(
                                        targetDefaultState->getName()) << ";"
                                << endl;
                    }
                } else {


                    out << "    {" << this->fsm->getNumberOfInputs() << "'b";
                    string invec = "";
                    for (int i = 0; i < this->fsm->getNumberOfInputs(); i++) {
                        invec = invec + "x";
                    }
                    out << invec << ", " << state->getName()
                            << "}:   next_state = "
                            << this->cleanString(targetDefaultState->getName())
                            << ";" << endl;


                }

                // Write out non default transitions
                //---------------------------
                FOREACH_STATE_STARTING_TRANSITIONS(state)
                    State * transEndState = transition->getEndState();
                    if(!transition->isDefault()) {

                        // Foreach conditions and write out transition
                        //--------------
                        FOREACH_TRANSITION_CONDITIONS(transition)

                            out << "    {" << this->fsm->getNumberOfInputs() << "'b";
                            string conditionInput = condition->getInput();
                            char X;
                            for (int i = 0; i < this->fsm->getNumberOfInputs(); i++) {
                                X = conditionInput[i];
                                if (X == '-' || X == 'X') {
                                    out << "x";
                                } else {
                                    out << X;
                                }
                            }
                            out << ", " << state->getName()
                                    << "}:   next_state = "
                                    << this->cleanString(transEndState->getName())
                                    << ";" << endl;

                        END_FOREACH_TRANSITION_CONDITIONS
                    }
                END_FOREACH_STATE_STARTING_TRANSITIONS

            END_FOREACH_STATE
            // EOF States transitions //

            // FOREACH Hyper transitions
            //----------------------------------
            FOREACH_HYPERTRANSITIONS(this->fsm)

                //-- Foreach conditions
                FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

                        out << "    {" << this->fsm->getNumberOfInputs() << "'b";
                        string conditionInput = condition->getInput();
                        char X;
                        for (int i = 0; i < this->fsm->getNumberOfInputs(); i++) {
                            X = conditionInput[i];
                            if (X == '-' || X == 'X') {
                                out << "x";
                            } else {
                                out << X;
                            }
                        }
                        out << ", " << this->fsm->getNumberOfOutputs() << "'b";
                        for (int i = 0; i < this->fsm->getNumberOfOutputs(); i++) {
                            out << "x";
                        }
                        out << "}:   next_state = "
                                << this->cleanString(hypertransition->getTargetState()->getName())
                                << ";" << endl;
                 END_FOREACH_HYPERTRANSITION_CONDITIONS

             END_FOREACH_HYPERTRANSITIONS

            // Default Reset State
            // Not necessary if number of states is a power of 2 and
            // the number of output states is equal to number of outputs as a power of 2 !!!
            //------------------------
            int a;
            a = 1 << numberofoutputs;
            if (!(a == this->fsm->getStateCount())) {
                out << "    default:  next_state = ";
                out << this->cleanString(this->fsm->getStatebyID(this->fsm->getResetState())->getName()) << ";" << endl;
            }

            out << "  endcase" << endl;
            out << "end" << endl << endl;

            // Write always-block => assigns next_state to state
            //------------------------------

            // Ifdef ASYNC_RES like always definition
            out << "`ifdef ASYNC_RES" << endl;
            out << " always @(posedge " << this->fsm->getClockName()
                    << " or negedge " << this->fsm->getResetName() << " ) begin"
                    << endl;
            out << "`else" << endl;
            out << " always @(posedge " << this->fsm->getClockName()
                    << ") begin" << endl;
            out << "`endif" << endl;

            out << "    if (!" << this->cleanString(this->fsm->getResetName())
                    << ")" << endl;
            out << "    begin" << endl;

            //-- Reset: Current State
            out << "        current_state <= ";
            out << this->cleanString(this->fsm->getStatebyID(this->fsm->getResetState())->getName())<< ";" << endl;


            //-- Reset: Current state delayed
            if (this->getForwardStateDelayed()) {

                out << "        forward_state_delayed <= ";
                out << this->cleanString(this->fsm->getStatebyID(this->fsm->getResetState())->getName()) << ";" << endl;

            }

            out << "    end" << endl;
            out << "    else begin" << endl;

            if (this->getForwardStateDelayed()) {
                //-- Output Assign Delayed
                out << "        current_state         <= forward_state_delayed;"
                        << endl;
                out << "        forward_state_delayed <= next_state;" << endl;

            } else {
                //-- Output Assign: normal
                out << "        current_state <= next_state;" << endl;
            }

            out << "    end" << endl;
            out << "end" << endl;

            // Verification Extensions, can be used with Cadence formal verifier
            //-----------------------------
            if (Statecov || Transcov) {

                //-- Start of Coverage
                out << endl << "`ifdef CAG_COVERAGE" << endl;
                out << "// synopsys translate_off" << endl << endl;

                //-- Write State coverage
                //-----------------------
                if (this->Statecov) {

                    //-- Commentar
                    out << "\t// State coverage" << endl << "\t//--------"
                            << endl << endl;
                    out << "\t//-- Coverage group declaration" << endl;

                    //-- Start of coverage group
                    out << "\tcovergroup cg_states @(posedge clk);" << endl;
                    out << "\t\tstates : coverpoint current_state {" << endl;

                    //-- States
                    int statescount = 0;
                    FOREACH_STATE(this->fsm)
                        out << "\t\t\tbins "
                                << this->cleanString(state->getName()) << " = {"
                                << this->cleanString(state->getName()) << "};"
                                << endl;
                    END_FOREACH_STATE

                    //-- End of coverage group
                    out << "\t\t}" << endl;
                    out << "\tendgroup : cg_states" << endl << endl;

                    out << "\t//-- Coverage group instanciation" << endl;
                    out << "\tcg_states state_cov_I;" << endl;

                    out << "\tinitial begin" << endl;

                    out << "\t\tstate_cov_I = new();" << endl;
                    out << "\t\tstate_cov_I.set_inst_name(\"state_cov_I\");"
                            << endl;

                    out << "\tend" << endl << endl;

                }

                //-- Write transitions coverage
                //-----------------------
                if (this->Transcov) {

                    //-- Commentar
                    out << "\t// Transitions coverage" << endl << "\t//--------"
                            << endl << endl;

                    //---- Go through all possible transitions for every condition
                    //-------------------------
                    int transitionCount = 0;
                    map<string, int> transitionsNames;// Map to solve multiple identical transition names
                    FOREACH_TRANSITIONS(this->fsm)

                        //-- Get target state and source state
                        string sstate = this->cleanString(transition->getStartState()->getName());
                        string tstate = this->cleanString(transition->getEndState()->getName());

                        // Prepare Transition Name
                        //-------------------------
                        stringstream transName;
                        transName << "tc_"
                                << this->cleanString(transition->getName());

                        //-- Unnamed = get a dummy name
                        if (transition->getName().size() == 0) {
                            transName << this->cleanString(sstate) << "_to_"
                                    << this->cleanString(tstate);
                        }

                        //-- Check buildup name doesn_t already exists, if yes, then increment the counter

                        // Increment value in map
                        if (transitionsNames.count(transName.str()) == 0)
                            transitionsNames.insert(
                                    pair<string, int>(transName.str(), 0));
                        transitionsNames[transName.str()]++;

                        // Suffix count value
                        if (transitionsNames[transName.str()] > 1) {
                            transName << transitionsNames[transName.str()];
                        }

                        //-- Default transition (not condition on input to be checked)
                        //------------------------------
                        if (transition->isDefault()) {

                            out << "\t"
                                << transName.str() << "_default"
                                << ": cover property( @(posedge clk) disable iff (!res_n)"
                                << "if (current_state == " << sstate << "))"
                                << "|=> (current_state == " << tstate
                                << ") );" << endl << endl;

                        }
                        //-- Otherwise one cover property per condition
                        //------------------------------
                        else {
                            int cCount = 0 ;
                            FOREACH_TRANSITION_CONDITIONS(transition)

                                //-- Condition Input
                                string conditionInput = condition->getInput();

                                // Replace '-' with 'x'
                                conditionInput = QString::fromStdString(conditionInput).replace('-',"x").toStdString();

                                //-- Input vector
                                stringstream inputVector;
                                inputVector << "inputvector ==? "
                                            << this->fsm->getNumberOfInputs() << "'b"
                                            << conditionInput;

                                //-- Property Name
                                stringstream propertyName;
                                propertyName << transName.str();

                                //-- Add Condition index and condition name (if one)
                                if (transition->getConditions().size()>1)
                                    propertyName << "_c" << cCount;
                                if (condition->getName().size()>0)
                                    propertyName << "_" << this->cleanString(condition->getName());

                                out << "\t"
                                    << propertyName.str()
                                    << ": cover property( @(posedge clk) disable iff (!res_n)"
                                    << "(" << inputVector.str() << ") &&"
                                    << "(current_state == " << sstate << ")"
                                    << "|=> (current_state == " << tstate << ")"
                                    << ");" << endl << endl;

                                cCount++;

                            END_FOREACH_TRANSITION_CONDITIONS


                        }

                     END_FOREACH_TRANSITIONS

                } // End of Transcoverage

                //-- End of coverage
                out << "// synopsys translate_on" << endl;
                out << "`endif" << endl << endl;
            }

            out << endl << "endmodule" << endl;

        }

        // Close Write out
        out.close();

        // Save file path
        fsm->setLastGeneratedVerilogFile(outputFile);

    }

}

string GenerationOfVerilog::createInstance() {

    stringstream ss;

    //-- Wires declarations
    //----------------------------

    //-- Inputs
    for (int i = 0; i < numberofinputs; i++) {
        ss << "reg fsm_" << this->fsm->getInputName(i) << ";" << endl;
    }
    ss << endl;

    //-- Outputs
    for (int i = 0; i < numberofoutputs; i++) {
        ss << "wire fsm_" << this->fsm->getOutputName(i) << ";" << endl;
    }

    //-- Forward declaration
    if (this->getForwardState()) {
        ss << "wire [" << this->numberofoutputs - 1 << ":0] fsm_forward_state;"
                << endl;
    }
    if (this->getForwardStateAsync()) {
        ss << "wire [" << this->numberofoutputs - 1
                << ":0] fsm_forward_state_async;" << endl;
    }
    if (this->getForwardStateDelayed()) {
        ss << "wire [" << this->numberofoutputs - 1
                << ":0] fsm_forward_state_delayed;" << endl;
    }
    ss << endl;

    // Instance
    //------------------------------

    //-- Instance declaration
    ss << this->modulename << " " << this->modulename << "_I (" << endl << endl;

    //-- Inputs
    ss << "\t//-- Inputs" << endl;
    for (int i = 0; i < numberofinputs; i++) {
        ss << "\t." << this->fsm->getInputName(i) << "(" << "fsm_"
                << this->fsm->getInputName(i) << "), " << endl;
    }
    ss << endl;

    //-- Outputs
    ss << "\t//-- Outputs" << endl;
    for (int i = 0; i < numberofoutputs - 1; i++) {
        ss << "\t." << this->fsm->getOutputName(i) << "(" << "fsm_"
                << this->fsm->getOutputName(i) << "), " << endl;
    }
    ss
            << "\t."
            << this->fsm->getOutputName(numberofoutputs - 1)
            << "("
            << "fsm_"
            << this->fsm->getOutputName(numberofoutputs - 1)
            << ")"
            << ((this->getForwardStateAsync() || this->getForwardStateDelayed()
                    || this->getForwardState()) ? "," : "") << endl;

    //-- Forward declaration
    if (this->getForwardStateAsync()) {
        ss << endl << "\t//-- Forward Async" << endl;
        ss << "\t.forward_state_async(fsm_forward_state_async)";
        if (this->getForwardStateDelayed() || this->getForwardState())
            ss << ",";
        ss << endl;

    }
    if (this->getForwardStateDelayed()) {
        ss << endl << "\t//-- Forward delayed" << endl;
        ss << "\t.forward_state_delayed(fsm_forward_state_delayed)";
        if (this->getForwardState())
            ss << ",";
        ss << endl;
    }
    if (this->getForwardState()) {
        ss << endl << "\t//-- Forwarded actual state" << endl;
        ss << "\t.forward_state(fsm_forward_state)" << endl;
    }

    //-- End instance
    ss << ");" << endl;

    return ss.str();
}

void GenerationOfVerilog::generateFFile(string outputFile) {
    this->generateFFile(outputFile, true);
}

void GenerationOfVerilog::generateFFile(string outputFile, bool withMap) {

    // Prepare Outputs and datas
    //-------------------------
    ofstream fOut(outputFile.c_str(), ios_base::out | ios_base::trunc);
    stringstream mapOut;

    //-- Base name (file name without ".f" extension) as absolute path
    QFileInfo qFileInfo(outputFile.c_str());
    string fileBaseName = qFileInfo.absoluteFilePath().toStdString().substr(0,
            qFileInfo.absoluteFilePath().toStdString().size() - 2);

    // Prepare F file
    //----------------------

    //-- Add path to verilog file
    fOut << Utils::insertEnvironementVariables(fileBaseName, this->getEnvp())
            << ".v" << endl;

    //-- Add simvision arg for map path?
    if (withMap) {
        fOut
                << "-simvisargs \"-input "
                << string(
                        Utils::insertEnvironementVariables(fileBaseName,
                                this->getEnvp())).append(".mmap") << "\""
                << endl;

    }

    // Write Out
    //----------------
    fOut.close();

    //-- Map ?
    if (withMap) {

        //-- Prepare output file
        ofstream mapOutFile(string(fileBaseName).append(".mmap").c_str(),
                ios_base::out | ios_base::trunc);

        //-- Write out map
        mapOutFile << this->generateMmap();

        //-- End
        mapOutFile.close();
    }

}

string GenerationOfVerilog::generateMmap() {

    //-- Prepare output and datas
    stringstream out;
    int outputsCount = this->fsm->getNumberOfOutputs();

    // Write Map out
    //---------------------

    //-- Map name
    out << "mmap new -name " << this->fsm->getFsmName() << " -contents {"
            << endl;

    //-- States
    FOREACH_STATE(this->fsm)

    //-- Write state
        QColor stateColor(state->getColor());
        out << "    { 'b" << state->getOutput() << " -label \""
                << state->getName() << "\"" << " -linecolor "
                << ((QVariant) stateColor).toString().toStdString()
                << " -bgcolor "
                << ((QVariant) stateColor).toString().toStdString() << "}"
                << endl;

    END_FOREACH_STATE

    //-- Close
    out << "}" << endl;

    //------------------//

    //-- Return
    return out.str();

}

void GenerationOfVerilog::calcHammingDistance() {

    /// Data initialization
    cout << "calculating Hamming Distance of original States" << endl;
    Core& c = *(Core::getInstance());
    originalStateOutputs = QStringList();
    originalStateNames = QStringList();
    originalStateName_index.clear();
    desired_distance = 0;
    min_distance = numberofoutputs;
    int distance[numberofoutputs][numberofoutputs];
    int numberOfStates = 0;

    //USER Input simulation
    desired_distance = 3;

    if (c.hasFsm()) {

        //Extend and Change the parameter string containing the current_state to a bucket of similar states.
        //Put original State Names and its binary coding into QStringLists
        int numberOfStates = 0;
        FOREACH_STATE(this->fsm)

            originalStateNames << QString(state->getName().c_str());
            originalStateOutputs << QString(state->getOutput().c_str());
            originalStateName_index.push_back(numberOfStates);
            //cout << "parameter " << this->fsm->getStateName() << " = " << numberofoutputs << "'b" << this->fsm->getStateOutput() << ";" << endl;
            numberOfStates++;

        END_FOREACH_STATE

#ifdef DEBUGGENERATIONOFVERILOG
        qDebug() << "Number of State Outputs in originalStateOutputs :"
        << originalStateOutputs.size();
        qDebug() << "Number of State Names in originalStateNames :"
        << originalStateNames.size();
        string teststr1("11010000");
        string teststr2("01010011");
        qDebug() << "Hamming distance of a: " << QString(teststr1.c_str())
        << "and b: " << QString(teststr2.c_str()) << "is: " << calcXOR(
                teststr1, teststr2);
        //qDebug()<<"Number of States: "<<numberOfStates;
#endif
        //Generate 2D-table with the Hamming Distance of each State to the other States,
        //save in distance[i][j]
        for (int i = 0; i < originalStateOutputs.size(); i++) {
            for (int j = 0; j < originalStateOutputs.size(); j++) {
                distance[i][j] = calcXOR(
                        originalStateOutputs.at(i).toStdString(),
                        originalStateOutputs.at(j).toStdString());
                if ((distance[i][j] < min_distance) && (i != j)) {
                    min_distance = distance[i][j];
                }
#ifdef DEBUGGENERATIONOFVERILOG
                qDebug() << "Distance between: " << i << "th state = "
                << originalStateOutputs.at(i) << " and " << j
                << "th state = " << originalStateOutputs.at(j)
                << " is " << distance[i][j];
                qDebug() << "minimal Distance is :" << min_distance;
                qDebug() << "Number of States: " << numberOfStates;
#endif
            }
        }

    }
}

int GenerationOfVerilog::calcExtendedHammingDistance(QString testedString) {

    /// Data initialization
    cout << "calculating Hamming Distance of extended States" << endl;
    Core& c = *(Core::getInstance());
    int ext_distance[numberofoutputs][numberofoutputs];
    ext_min_distance = numberofoutputs;
    QStringList cur_extendedStateEncoding = extendedStateEncoding;
    cur_extendedStateEncoding << testedString;

#ifdef DEBUGGENERATIONOFVERILOG
    qDebug() << "Number of State Outputs in extendedStateEncoding :"
    << extendedStateEncoding.size();
#endif
    //Generate 2D-table with the Hamming Distance of each State to the other States,
    //save in distance[i][j]
    for (int i = 0; i < cur_extendedStateEncoding.size(); i++) {
        for (int j = 0; j < cur_extendedStateEncoding.size(); j++) {
            ext_distance[i][j] = calcXOR(
                    cur_extendedStateEncoding.at(i).toStdString(),
                    cur_extendedStateEncoding.at(j).toStdString());
            if ((ext_distance[i][j] < ext_min_distance) && (i != j)) {
                ext_min_distance = ext_distance[i][j];
            }
#ifdef DEBUGGENERATIONOFVERILOG
            qDebug() << "Distance between extended: " << i << "th state = "
            << cur_extendedStateEncoding.at(i) << " and " << j
            << "th state = " << cur_extendedStateEncoding.at(j)
            << " is " << ext_distance[i][j];
            qDebug() << "minimal Distance is :" << ext_min_distance;
#endif
        }
    }
    return ext_min_distance;
}

int GenerationOfVerilog::calcXOR(string stringA, string stringB) {

    int dist = 0;

    if (stringA.length() == stringB.length()) {
        int i;
        for (i = 0; i < stringA.length(); i++) {
            if (((stringA[i] == '0') && (stringB[i] == '0'))
                    || ((stringA[i] == '1') && (stringB[i] == '1'))) {
                xorDifference[i] = '0';
            } else if (((stringA[i] == '1') && (stringB[i] == '0'))
                    || ((stringA[i] == '0') && (stringB[i] == '1'))) {
                xorDifference[i] = '1';
                dist++;
            } else if ((stringA[i] == '-') || (stringB[i] == '-')
                    || (stringA[i] == 'x') || (stringB[i] == 'x')
                    || (stringA[i] == 'X') || (stringB[i] == 'X')) {
                xorDifference[i] = '0';
            }
        }

    }

    return dist;

}

void GenerationOfVerilog::extendStateEncoding() {

    /// Data initialization
    cout << "extending State Output encoding" << endl;
    if ((desired_distance - min_distance) > 0) {
        extendedBy = desired_distance - min_distance;
    } else {
        extendedBy = 0;
    }
    int cur_extendedBy = extendedBy;
    current_extension_int = 0;
    extendedStateEncoding = QStringList();
    QStringList already_used_extensions = QStringList();

    current_extension_int = 0; //test
    //current_extension_string = QString::number( current_extension_int , 2);
    current_extension_string = QString("%1").arg(current_extension_int,
            extendedBy, 2);
    current_extension_string.replace(QString(" "), QString("0")); // replace space character with 0s

#ifdef DEBUGGENERATIONOFVERILOG
    QString str2;
    str2 = QString("Decimal 10 is %1 in binary").arg(10, 12, 2);
    str2.replace(QString(" "), QString("0"));
    qDebug() << str2;
    qDebug() << current_extension_int << " as String: "
    << current_extension_string;
    qDebug() << "desired_distance: " << desired_distance << " extendedBy: "
    << extendedBy;
#endif
    //extendedStateEncoding
    //QString originalStateOutputs
    //        apppend

    for (cur_extendedBy = extendedBy;
            extendedStateEncoding.size() < originalStateOutputs.size();
            cur_extendedBy++) {
#ifdef DEBUGGENERATIONOFVERILOG
        qDebug() << "extendedStateEncoding size: "
        << extendedStateEncoding.size()
        << " originalStateOutputs size: "
        << originalStateOutputs.size();
        qDebug() << "cur_extendedBy: " << cur_extendedBy;
#endif
        for (current_extension_int = 0;
                current_extension_int < (2 ^ cur_extendedBy);
                current_extension_int++) {
            current_extension_string = QString("%1").arg(current_extension_int,
                    cur_extendedBy, 2);
            current_extension_string.replace(QString(" "), QString("0")); // replace space character with 0s
            QString complete_string = originalStateOutputs.at(
                    extendedStateEncoding.size() - 1); //?
            complete_string.append(current_extension_string);
#ifdef DEBUGGENERATIONOFVERILOG
            qDebug() << "current_extension_int: " << current_extension_int
            << " current_extension_string: "
            << current_extension_string;
            qDebug() << "2^cur_extendedBy: " << (2 ^ cur_extendedBy)
            << " complete_string: " << complete_string;
#endif
            bool in_ext_list = false;
            for (int ext_list_counter = 0;
                    ext_list_counter < already_used_extensions.size();
                    ++ext_list_counter) {
                if (already_used_extensions.at(ext_list_counter)
                        == current_extension_string) {
                    in_ext_list = true;
                }
#ifdef DEBUGGENERATIONOFVERILOG
                qDebug() << "already_used_extensions.at(i): "
                << already_used_extensions.at(ext_list_counter)
                << " current_extension_string: "
                << current_extension_string;
                qDebug() << "in_ext_list: " << in_ext_list;
#endif
            }
            if ((in_ext_list)
                    && (calcExtendedHammingDistance(complete_string)
                            >= desired_distance)) {
                extendedStateEncoding << complete_string;
                already_used_extensions << current_extension_string;
            }

        }
    }

}

void GenerationOfVerilog::createStateVariations() {

    /// Data initialization
    cout << "create Variations with SEUs of States" << endl;

}

void GenerationOfVerilog::setForwardStateDelayed(bool forward) {
    this->forwardStateDelayed = forward;
}
bool GenerationOfVerilog::getForwardStateDelayed() {
    return this->forwardStateDelayed;
}
void GenerationOfVerilog::setForwardStateAsync(bool forward) {
    this->forwardStateAsync = forward;
}
bool GenerationOfVerilog::getForwardStateAsync() {
    return this->forwardStateAsync;
}
void GenerationOfVerilog::setForwardState(bool forward) {
    this->forwardState = forward;
}
bool GenerationOfVerilog::getForwardState() {
    return this->forwardState;
}

void GenerationOfVerilog::addGenerateInclude(QString include) {
    this->generateIncludes.push_back(include);
}

string GenerationOfVerilog::cleanString(string input) {

    //-- Use QString
    QString inputQString(input.c_str());

    // Replace spaces with _
    //------------------------
    inputQString = inputQString.replace(QRegExp("\\s+"), QChar('_'));

    //-- replaces then non \\W characters by nothing
    //inputQString = inputQString.replace(QRegExp("\\W"),"");

    //-- Only Allow letters and numbers
    inputQString = inputQString.replace(QRegExp("[^A-Za-z0-9_]*"), "");

    return inputQString.toStdString();

}

void GenerationOfVerilog::setEnvp(char ** envp) {
    this->envp = envp;
}

char** GenerationOfVerilog::getEnvp() {
    return this->envp;
}
