/**
 * VHDLGenerator.cpp
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


#include "VHDLGenerator.h"

VHDLGenerator::VHDLGenerator() {
    this->Statecov = true;
    this->Transcov = true;

}

VHDLGenerator::~VHDLGenerator() {
}

Generator * VHDLGenerator::newInstance() {

    return new VHDLGenerator();

}


QString VHDLGenerator::getName()  {

    return "VHDL Generator";
}


QString VHDLGenerator::getDescription()  {

    return "VHDL Generator";

}

void VHDLGenerator::generate(Fsm * fsm, QDataStream * dataStream) {

    //-- Open Target file
    QTextStream out (dataStream->device());

    int numberofinputs = fsm->getNumberOfInputs();
    int numberofoutputs = fsm->getNumberOfOutputs();
    int resetstate = fsm->getResetState();

    // utils
    QString tab = QString(" ").repeated(4);
    QString tab2 = QString(" ").repeated(4*2);
    QString tab3 = QString(" ").repeated(4*3);
    QString tab4 = QString(" ").repeated(4*4);
    QString tab5 = QString(" ").repeated(4*5);

    //calcHammingDistance();
    //extendStateEncoding();
    //createStateVariations();

    // Write license
    //-----------------
    out << "--" << endl;

    //FIXME  Write Instance pattern
    //----------------------------
    //out << " " << this->createInstance(fsm) << endl;
    //out << " */" << endl;

    // Write includes
    //----------------------
    /*if (this->getParameter("includes").isValid()) {
        QStringList includes = this->getParameter("includes").toStringList();
        for (int i = 0; i < includes.size(); i++) {
            out << "`include \""
                    << includes.at(i) << "\""
                    << endl;
        }
    }*/

    // Write Libraries import
    //-----------------------------
    out << "library ieee ;" << endl;
    out << "use ieee.std_logic_1164.all;" << endl;



    // Write entity description
    //----------------------------
    out << "entity " << fsm->getFsmName().c_str() << " is " << endl;
    out << "    port ( "  << endl;
    out << "            " << fsm->getClockName().c_str() << ": in std_logic; " << endl;
    out << "            " << fsm->getResetName().c_str() << ": in std_logic; " << endl << endl;


    //---- Input
    out << "    -- Inputs" << endl << "    -------------- " << endl;
    for (int i = 0; i < numberofinputs; i++) {

    out << "            " << fsm->getInputName(i).c_str() << ": in std_logic; "   << endl;
    }
    out << endl;

    //---- Outputs
    out << "    -- Outputs" << endl << "    -------------- " << endl;
    for (int i = 0; i < numberofoutputs - 1; i++) {

    out << "              " << fsm->getOutputName(i).c_str() << ": out std_logic;"   << endl;
    }
    out << "              " << fsm->getOutputName(numberofoutputs - 1).c_str() << ": out std_logic";

    //---- Forward declaration?
    /*if (this->getParameter("forward.async").toBool()) {
        out << ", " << endl << endl;
        out << "    //-- Async Forward state" << endl;
        out << "    output wire [" << (numberofoutputs - 1)
                << ":0] forward_state_async";

    }
    if (this->getParameter("forward.delayed").toBool()) {
        out << ", " << endl << endl;
        out << "    //-- Delayed Forward state" << endl;
        out << "    output reg [" << (numberofoutputs - 1)
                << ":0] forward_state_delayed ";

    }
    if (this->getParameter("forward.sync").toBool()) {
        out << ", " << endl << endl;
        out << "    //-- Forwarded state (Actual State)" << endl;
        out << "    output wire [" << (numberofoutputs - 1)
                << ":0] forward_state ";
    }*/
    out << endl;

    out << " );" << endl;
    out << endl;

    // EOF entity
    out << "end "  << fsm->getFsmName().c_str() << ";" << endl << endl;

    // Declare Architecture
    //------------------------------
    out << "architecture behavorial of " << fsm->getFsmName().c_str() << " is " << endl;


    // Write Parameters (States)
    //-----------------------------
    FOREACH_STATE(fsm)
        out << "constant " << state->getName().c_str() << " : std_logic_vector ( "<< (numberofoutputs-1) <<" downto 0) := \"" << state->getOutput().c_str() << "\"; " << endl;
    END_FOREACH_STATE
    out << endl;

    // current_state Declaration
    //----------------------------------
    out  << "signal current_state :  std_logic_vector ( "<< (numberofoutputs-1) <<" downto 0) := " << fsm->getStatebyID(fsm->getResetState())->getName().c_str() << ";" << endl;
    /*for (int i = 0; i < numberofoutputs - 1; i++) {
        out << fsm->getOutputName(i).c_str() << " & ";
    }
    out << fsm->getOutputName(numberofoutputs - 1).c_str();
    out << ";" << endl ;*/

    out << "signal next_state :  std_logic_vector ( "<< (numberofoutputs-1) <<" downto 0) := " << fsm->getStatebyID(fsm->getResetState())->getName().c_str() << ";" << endl;
    out << endl << endl;

    // input vector declaration
    //----------------------
    out << "signal inputvector : std_logic_vector(" << (numberofinputs - 1) << " downto 0); ";

    

    // Begin
    //----------------------
    out << "begin" << endl << endl;

    // Input vector assignment
    //----------------------
    out << "inputvector <= ";
    for (int i = 0; i < numberofinputs - 1; i++) {
        out << fsm->getInputName(i).c_str() << " & ";
    }
    out << fsm->getInputName(numberofinputs - 1).c_str();
    out << ";" << endl << endl;

    // Write Process
    //--------------------
    out << tab << "process(clk,res_n) begin" << endl;

    //----- Reset
    out << tab2 << "if (res_n = '0') then" << endl;
    for (int i = 0; i < fsm->getNumberOfOutputs(); i++) {
        out << tab3 << fsm->getOutputName(i).c_str() << " <= current_state("<< i <<");" << endl;
    }

    //----- Clock process
    out << tab2 << "elsif (rising_edge(clk)) then" << endl;

    //---- Write an IF Per current state, with the case in the IF
    //----------------------
    FOREACH_STATE(fsm)
        cout << "Working on State " << state->getName() << endl;

        out << tab3 << "if (current_state = " << cleanString(state->getName()) << ") then" << endl;

        out << tab4 << "case inputvector is" << endl;


        //---- Foreach Transitions that start from this state
        //-----------------
        vector<string> input;

        //FOREACH_TRANSITIONS(fsm)

        // Determine Inputs for the transitions starting from this state, and non default
        //  - The conditions' inputs are then inverted, then minimized to determine all the cases matching the default transition
        //---------------------
        State * targetDefaultState = NULL;
        FOREACH_STATE_STARTING_TRANSITIONS(state)

            //-- Record default transition target
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
                                    < fsm->getNumberOfInputs();
                            i++) {
                        X = conditionInput[i];
                        if (X == 'x' || X == 'X') {
                            result = result + '-';
                        } else {
                            result = result + X;
                        }
                    }
                    input.push_back(result);

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

            cout << "\tInvertDNF on : " << input.size() << "inputs" << endl;


            //-- Invert the input vector of all defined transitions to get back all the inputs leading to the default transition
            InvertDNF INV(this->getParameter("removeIntersections").toBool());
            string defaultvalue = INV.invert(input);

            //-- Split the defaultValue string at "+" to get the default input vectors
            QStringList defaultInputs = QString::fromStdString(defaultvalue).split("+");

            //-- Write out the case WHEN matching the inputs leading to the default transition
            for (int i = 0; i < defaultInputs.size(); ++i) {
                
                QString inputValue = defaultInputs.at(i);
                cout << "\tDefault value: " << inputValue.toStdString() << endl;

                // Out
                out << tab5 << "WHEN \"" << inputValue <<"\"  => next_state <= " << this->cleanString(targetDefaultState->getName()) << ";" << endl;
            }
          

            


/*
            int maxlen = defaultvalue.length();
            int len = maxlen;
            if (len > 0) {

                out << "WHEN (\"";

                while (len > fsm->getNumberOfInputs()) {
                    string s = defaultvalue;
                    do {
                        len--;
                    } while ((s[len] != '+') && (len != 0));
                    s.assign(s, (len + 1),
                            (maxlen - len - 1));
                    defaultvalue.assign(defaultvalue, 0,
                            len);
                    char X;
                    for (int i = 0; i < fsm->getNumberOfInputs();
                            i++) {
                        X = s[i];
                        if (X == '-' || X == 'X') {
                            out << "x";
                        } else {
                            out << X;
                        }
                    }
                    out
                            << "\" & "
                            << state->getName().c_str()
                            << ")" << endl;
                    out << "    {" << fsm->getNumberOfInputs()
                            << "'b";
                }

                // Output value
                char X;
                for (int i = 0; i < fsm->getNumberOfInputs(); i++) {

                    X = defaultvalue[i];
                    if (X == '-' || X == 'X') {
                        out << "x";
                    } else {
                        out << X;
                    }
                }

                out
                        << ", "
                        << state->getName().c_str()
                        << "}";


                // End of default conditions
                // Write end case
                //-------------------
                out  << " =>   next_state = " << this->cleanString(targetDefaultState->getName()) << ";" << endl;

            }*/

        } else {

            //-- If no transitions are defined, always transition to the default state (matching input vector is xxxxx)            
            QString invec = QString("-").repeated(fsm->getNumberOfInputs());
            out << tab5 << "WHEN  \"" << invec << "\"  =>   next_state <= " << this->cleanString(targetDefaultState->getName()) << ";" << endl;
  

        }

        // Write out non default transitions
        //---------------------------
        FOREACH_STATE_STARTING_TRANSITIONS(state)
            State * transEndState = transition->getEndState();
            if(!transition->isDefault()) {

                // Foreach conditions and write out transition
                //--------------
                FOREACH_TRANSITION_CONDITIONS(transition)

                    // Prepare input vector
                    QString conditionInput = QString::fromStdString(condition->getInput());

                    // Clean
                    conditionInput = conditionInput.toLower().replace("x","-");

                    // Write
                    out << tab5 << "WHEN  \"" << conditionInput << "\"  =>   next_state <= " << this->cleanString(transEndState->getName()) << ";" << endl;
  
                    /*out << "    {" << fsm->getNumberOfInputs() << "'b";
                    
                    char X;
                    for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
                        X = conditionInput[i];
                        if (X == '-' || X == 'X') {
                            out << "x";
                        } else {
                            out << X;
                        }
                    }
                    out << ", " << state->getName().c_str()
                            << "}:   next_state = "
                            << this->cleanString(transEndState->getName())
                            << ";" << endl;*/

                END_FOREACH_TRANSITION_CONDITIONS
            }
        END_FOREACH_STATE_STARTING_TRANSITIONS

        // ADD Others to default transition
        //---------------
        out << tab5 << "WHEN OTHERS => next_state <= " << state->getDefaultTransition()->getEndState()->getName().c_str() << ";" << endl;

        out << tab4 << "end case;" << endl;
        out << tab3 << "end if;" << endl << endl;

        

        

    END_FOREACH_STATE
    // EOF States transitions //


    // FOREACH Hyper transitions
    //----------------------------------
    out << tab3 << "-- Hypertransitions (if some)" << endl;
    FOREACH_HYPERTRANSITIONS(fsm)

        //-- Foreach conditions
        FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

            // Prepare input vector
            QString conditionInput = QString::fromStdString(condition->getInput());

            // Clean
            conditionInput = conditionInput.toLower().replace("x","-");

            // Current State value is xxx because hypertransitions don't care about the current state
            QString invec = QString("-").repeated(fsm->getNumberOfOutputs());

            // Write
            out << tab3 << "if (inputvector = \"" << conditionInput << "\") then" << endl;
            out << tab4 <<  "next_state <= " << this->cleanString(hypertransition->getName()) << ";" << endl;
            out << tab3 << "end if;" << endl;
             
            //out << "WHEN  (\"" << conditionInput << "\" & "<< invec << ") =>   next_state = " << this->cleanString(hypertransition->getName()) << ";" << endl;
  

                /*out << "    {" << fsm->getNumberOfInputs() << "'b";
                string conditionInput = condition->getInput();
                char X;
                for (int i = 0; i < fsm->getNumberOfInputs(); i++) {
                    X = conditionInput[i];
                    if (X == '-' || X == 'X') {
                        out << "x";
                    } else {
                        out << X;
                    }
                }
                out << ", " << fsm->getNumberOfOutputs() << "'b";
                for (int i = 0; i < fsm->getNumberOfOutputs(); i++) {
                    out << "x";
                }
                out << "}:   next_state = "
                        << this->cleanString(hypertransition->getTargetState()->getName())
                        << ";" << endl;*/

         END_FOREACH_HYPERTRANSITION_CONDITIONS

     END_FOREACH_HYPERTRANSITIONS

     // Update current_state and signal output
     //------------------------
     out << endl;
     out << tab3 << "current_state <= next_state;" << endl;
     for (int i = 0; i < fsm->getNumberOfOutputs(); i++) {
        out << tab3 << fsm->getOutputName(i).c_str() << " <= current_state("<< i <<");" << endl;
     }
     


    // EOF Process
    //--------------------
    out << tab2 << "end if;" << endl;
    out << tab << "end process;" << endl << endl;



    out << "end behavorial;" << endl;



    return;

  /*  out << "assign {";
    for (int i = 0; i < numberofoutputs - 1; i++) {
        out << fsm->getOutputName(i).c_str() << ", ";
    }
    out << fsm->getOutputName(numberofoutputs - 1).c_str();
    out << "} = current_state;" << endl;

    //-- If Async Forward declaration, forward_state_async is the next_state
    if (this->getParameter("forward.async").toBool()) {
        out << "assign forward_state_async = next_state;" << endl;
    }
    //-- If normal forward, forward_state is current_state
    if (this->getParameter("forward.sync").toBool()) {
        out << "assign forward_state = current_state;" << endl;
    }
    out << endl;*/



    // Write combinational logic always-block
    //--------------------------------------
    out << "always @(*) begin" << endl;

    //-- If we are in delayed forward mode, use the forward_state declaration for computing
    if (this->getParameter("forward.delayed").toBool()) {
        out << "  casex({inputvector, forward_state_delayed})" << endl;
    } else {
        out << "  casex({inputvector, current_state})" << endl;
    }



    

    // Default Reset State
    // Not necessary if number of states is a power of 2 and
    // the number of output states is equal to number of outputs as a power of 2 !!!
    //------------------------
    int a;
    a = 1 << numberofoutputs;
    if (!(a == fsm->getStateCount())) {
        out << "    default:  next_state = ";
        out << this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName()) << ";" << endl;
    }

    out << "  endcase" << endl;
    out << "end" << endl << endl;

    // Write always-block => assigns next_state to state
    //------------------------------

    // Ifdef ASYNC_RES like always definition
    out << "`ifdef ASYNC_RES" << endl;
    out << " always @(posedge " << fsm->getClockName().c_str()
            << " or negedge " << fsm->getResetName().c_str() << " ) begin"
            << endl;
    out << "`else" << endl;
    out << " always @(posedge " << fsm->getClockName().c_str()
            << ") begin" << endl;
    out << "`endif" << endl;

    out << "    if (!" << this->cleanString(fsm->getResetName())
            << ")" << endl;
    out << "    begin" << endl;

    //-- Reset: Current State
    out << "        current_state <= ";
    out << this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName())<< ";" << endl;


    //-- Reset: Current state delayed
    if (this->getParameter("forward.delayed").toBool()) {

        out << "        forward_state_delayed <= ";
        out << this->cleanString(fsm->getStatebyID(fsm->getResetState())->getName()) << ";" << endl;

    }

    out << "    end" << endl;
    out << "    else begin" << endl;

    if (this->getParameter("forward.delayed").toBool()) {
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
            FOREACH_STATE(fsm)
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
            FOREACH_TRANSITIONS(fsm)

                //-- Get target state and source state
                QString sstate = this->cleanString(transition->getStartState()->getName());
                QString tstate = this->cleanString(transition->getEndState()->getName());

                // Prepare Transition Name
                //-------------------------
                stringstream transName;
                transName << "tc_"
                        << this->cleanString(transition->getName()).toStdString();

                //-- Unnamed = get a dummy name
                if (transition->getName().size() == 0) {
                    transName << this->cleanString(sstate.toStdString()).toStdString() << "_to_"
                            << this->cleanString(tstate.toStdString()).toStdString();
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
                        << transName.str().c_str() << "_default"
                        << ": cover property( @(posedge clk) disable iff (!res_n)"
                        << " (current_state == " << sstate << ")"
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
                                    << fsm->getNumberOfInputs() << "'b"
                                    << conditionInput;

                        //-- Property Name
                        stringstream propertyName;
                        propertyName << transName.str();

                        //-- Add Condition index and condition name (if one)
                        if (transition->getConditions().size()>1)
                            propertyName << "_c" << cCount;
                        if (condition->getName().size()>0)
                            propertyName << "_" << this->cleanString(condition->getName()).toStdString();

                        out << "\t"
                            << propertyName.str().c_str()
                            << ": cover property( @(posedge clk) disable iff (!res_n)"
                            << "(" << inputVector.str().c_str() << ") &&"
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


    // Save file path
   // fsm->setLastGeneratedVerilogFile(out);

}



QString VHDLGenerator::createInstance(Fsm * fsm) {

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

    //-- Clocks and Resets
    ss << "    ." << fsm->getClockName().c_str() << "(), "
                << endl;
    ss << "    ." << fsm->getResetName().c_str() << "(), "
                   << endl;

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

QString VHDLGenerator::cleanString(string input) {

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
