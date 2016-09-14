/**
 * VerilogGenerator2.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */



// Includes
//------------------

//-- Std
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

//-- Qt
#include <QtCore>

//-- Core
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Hypertrans.h>
#include <core/Condition.h>

//-- Verification
#include <verification/verify.h>
#include <verification/logicmin.h>
#include <verification/invertDNF.h>


#include "VerilogGenerator2.h"

VerilogGenerator2::VerilogGenerator2() {
    // TODO Auto-generated constructor stub

}

VerilogGenerator2::~VerilogGenerator2() {
    // TODO Auto-generated destructor stub
}



Generator * VerilogGenerator2::newInstance() {

    return new VerilogGenerator2();

}


QString VerilogGenerator2::getName()  {

    return "Verilog Generator 2";
}


QString VerilogGenerator2::getDescription()  {

    return "Verilog Generator v2 from CAG";

}

void VerilogGenerator2::generate(Fsm * fsm, QDataStream * dataStream) {

    //-- Open Target file
    QTextStream out (dataStream->device());

    // Module Declaration
    //-----------------------------

    //-- Name
    out << "module " << fsm->getFsmName().c_str() << "(" << endl ;

    //-- I/O
    out << "    // Sys" << endl << "    //------------ " << endl;
    out << "    input wire " << fsm->getClockName().c_str() << ", "
                << endl;
    out << "    input wire " << fsm->getResetName().c_str() << ", "
            << endl << endl;


    out << "    // Inputs" << endl << "    //------------ " << endl;
    for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
        out << "    input wire " << fsm->getInputName(i).c_str() << ", "
                << endl;
    }
    out << endl;
    out << "    // Outputs" << endl << "    //------------ " << endl;
    for (int i = 0; i < fsm->getNumberOfOutputs() - 1; i++) {
        out << "    output wire " << fsm->getOutputName(i).c_str() << ", "
                << endl;
    }
    out << "    output wire "
            << fsm->getOutputName(fsm->getNumberOfOutputs() - 1).c_str();

    out << ");" << endl << endl;


    // Wirings
    //--------------------

    //-- States localparam
    FOREACH_STATE(fsm)
        out << "localparam " << this->cleanString(state->getName()) << " = "
            << fsm->getNumberOfOutputs() << "'b" << state->getOutput().c_str() << ";"
            << endl;
    END_FOREACH_STATE

    //-- Transition matched wire
    out << "reg transition_matched; // Async Reg" << endl;
    out << "reg [" << (fsm->getNumberOfOutputs() - 1)
                    << ":0] next_state; // Async Reg" << endl << endl;
    //-- Output State
    out << "reg [" << (fsm->getNumberOfOutputs() - 1)
            << ":0] state; // Async Reg" << endl;
    out << "reg [" << (fsm->getNumberOfOutputs() - 1)
                << ":0] output_state; // Reg" << endl << endl;

    // Assign outputs
    for (int i = 0; i < fsm->getNumberOfOutputs(); i++) {
        out << "assign "<< fsm->getOutputName(fsm->getNumberOfOutputs()-1-i).c_str() <<" = output_state["<< i <<"];" << endl;
    }
    out << endl;

    //-- Input Vector
    out << "wire [" << (fsm->getNumberOfInputs() - 1)
            << ":0] input_vector;" << endl;
    out << "assign input_vector = {";
    for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
        out << fsm->getInputName(i).c_str();
        if (i<fsm->getNumberOfInputs()-1)
            out << ", ";
    }
    out << "};" << endl << endl;

    // Transitions Matching
    // -- For each State (current state), the set of conditions on non default transitions define changes to a next state
    //--------------------------------

    out << "always @(*) begin" << endl;
    out << "   if (!res_n) begin" << endl;
    out << "       transition_matched = 0;" << endl;
    out << "       next_state = "<< this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName()) <<";" << endl;
    out << "   end else begin" << endl << endl;



    //---- Transition Matched case
    //----   - Generate a case to determine if we match a transition or not
    out << "       casex({input_vector,output_state})" << endl;
    FOREACH_STATE(fsm)
        FOREACH_STATE_STARTING_TRANSITIONS(state)
            if (transition->isDefault())
                continue;

            FOREACH_TRANSITION_CONDITIONS(transition)

                QString input = QString::fromStdString(condition->getInput());
                input = input.replace('-','x');
                out << "            " << "{" << fsm->getNumberOfInputs() << "'b" << input <<"," << this->cleanString(state->getName()) << "}: transition_matched = 1;" << endl;

            END_FOREACH_TRANSITION_CONDITIONS


        END_FOREACH_STATE_STARTING_TRANSITIONS
    END_FOREACH_STATE

    //-- Per default, transition is not matched
    out << "            default: transition_matched = 0;" << endl;
    out << "        endcase"<<endl <<  endl;

    //---- State setting case
    //---    - Generate a full_case to determine which state is the next one
    //---    - We cqn use a full_case because the final state decision is covered by the transition_matched signal
    out << "       next_state = " << this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName())<< ";" << endl;
    out << "       casex({input_vector,output_state}) // cadence full_case" << endl;
    FOREACH_STATE(fsm)
        FOREACH_STATE_STARTING_TRANSITIONS(state)
            if (transition->isDefault())
                continue;

            FOREACH_TRANSITION_CONDITIONS(transition)

                QString input = QString::fromStdString(condition->getInput());
                input = input.replace('-','x');
                out << "            " << "{" << fsm->getNumberOfInputs() << "'b" << input <<"," << this->cleanString(state->getName()) << "}: next_state = "<< this->cleanString(transition->getEndState()->getName()) << ";" << endl;

            END_FOREACH_TRANSITION_CONDITIONS


        END_FOREACH_STATE_STARTING_TRANSITIONS
    END_FOREACH_STATE

    out << "        endcase"<<endl <<  endl;
    out << "    end" << endl << endl;
    out << "end" << endl << endl;

    // Output State = Matched next state from case, or default value
    //----------------------
    out << "`ifdef ASYNC_RES" << endl;
    out << " always @(posedge " << fsm->getClockName().c_str()
            << " or negedge " << fsm->getResetName().c_str() << " ) begin"
            << endl;
    out << "`else" << endl;
    out << " always @(posedge " << fsm->getClockName().c_str()
            << ") begin" << endl;
    out << "`endif" << endl;
    out << "    if (!res_n) begin" << endl;
    out << "        output_state <= "<< this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName()) <<";" << endl;
    out << "    end else if (!transition_matched) begin" << endl << endl;

    //---- Default transition if no match
    //-----------------------
    out << "        case(output_state) // cadence full_case" << endl;
    FOREACH_STATE(fsm)
            FOREACH_STATE_STARTING_TRANSITIONS(state)
                if (!transition->isDefault())
                    continue;
    out << "            "<< this->cleanString(state->getName()) <<" : output_state <= "<< this->cleanString(transition->getEndState()->getName())  <<" ;" << endl;
                break;

            END_FOREACH_STATE_STARTING_TRANSITIONS
    END_FOREACH_STATE


    out << "        endcase" << endl;


    out << "    end else begin" << endl << endl;

    //---- Resulting state if match
    //--------------------------
    out << "        output_state <= next_state;" << endl;

    out << "    end" << endl;
    out << "end" << endl;

    // Module end
    //------------------
    out << "endmodule" << endl;



}



QString VerilogGenerator2::createInstance(Fsm * fsm) {

    stringstream ss;

    //-- Wires declarations
    //----------------------------

    //-- Inputs
    for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
        ss << "reg fsm_" << fsm->getInputName(i) << ";" << endl;
    }
    ss << endl;

    //-- Outputs
    for (int i = 0; i < fsm->getNumberOfOutputs(); i++) {
        ss << "wire fsm_" << fsm->getOutputName(i) << ";" << endl;
    }

    //-- Forward declaration
    /*if (this->getForwardState()) {
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
    }*/
    ss << endl;

    // Instance
    //------------------------------

    //-- Instance declaration
    ss << fsm->getFsmName() << " " << fsm->getFsmName() << "_I (" << endl << endl;

    //-- Inputs
    ss << "\t//-- Inputs" << endl;
    for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
        ss << "\t." << fsm->getInputName(i) << "(" << "fsm_"
                << fsm->getInputName(i) << "), " << endl;
    }
    ss << endl;

    //-- Outputs
    ss << "\t//-- Outputs" << endl;
    for (int i = 0; i < fsm->getNumberOfOutputs() - 1; i++) {
        ss << "\t." << fsm->getOutputName(i) << "(" << "fsm_"
                << fsm->getOutputName(i) << "), " << endl;
    }
    ss
            << "\t."
            << fsm->getOutputName(fsm->getNumberOfOutputs() - 1)
            << "("
            << "fsm_"
            << fsm->getOutputName(fsm->getNumberOfOutputs() - 1)
            << ")" << endl ;
            /*<< ((this->getForwardStateAsync() || this->getForwardStateDelayed()
                    || this->getForwardState()) ? "," : "") << endl;*/

    //-- Forward declaration
    /*if (this->getForwardStateAsync()) {
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
    }*/

    //-- End instance
    ss << ");" << endl;

    return QString::fromStdString(ss.str());
}

QString VerilogGenerator2::cleanString(string input) {

    //-- Use QString
    QString inputQString(input.c_str());

    // Replace spaces with _
    //------------------------
    inputQString = inputQString.replace(QRegExp("\\s+"), QChar('_'));

    //-- replaces then non \\W characters by nothing
    //inputQString = inputQString.replace(QRegExp("\\W"),"");

    //-- Only Allow letters and numbers
    inputQString = inputQString.replace(QRegExp("[^A-Za-z0-9_]*"), "");

    return inputQString;

}

