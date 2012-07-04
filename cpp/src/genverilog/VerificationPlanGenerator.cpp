/*
 * VerificationPlanGenerator.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: rleys
 */

// Includes
//-------------

//-- Std
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/Fsm.h>
#include <core/Trans.h>
#include <core/State.h>
#include <core/Link.h>

//-- Generator
#include <generate/Generator.h>
#include <generate/GeneratorBuilder.h>

#include "VerificationPlanGenerator.h"

VerificationPlanGenerator::VerificationPlanGenerator() {


}

Generator * VerificationPlanGenerator::newInstance() {

    return new VerificationPlanGenerator();

}

QString VerificationPlanGenerator::getName()  {

    return "Cadence Vplan generator";
}


QString VerificationPlanGenerator::getDescription()  {

    return "Generates a cadence eplanner verification plan for transitions and states coverages";

}


void VerificationPlanGenerator::generate(Fsm * fsm, QDataStream * dataStream) {

    QTextStream output (dataStream->device());


	QString sectionInstance("");
	QString logicalInstance("FSM_");
	logicalInstance+= fsm->getFsmName().c_str();
	sectionInstance+= fsm->getFsmName().c_str();
	QString fileName(fsm->getFsmName().c_str());
	fileName+=".vplan";

	output << "ep_Label : \""<<fileName<<".vplan\";" << endl;
	output << "	section \""<< sectionInstance <<"\" {"<< endl;
	output << "		logical_instances : \""<< logicalInstance <<"\";"<< endl;
	output << "		section \"State Coverage\" {"<< endl;
	output << "			verification_scope : \"default\";"<< endl;
	output << "			logical_instances : \""<< logicalInstance <<"\";"<< endl;
	output << "			coverage \"State coverage\" {"<< endl;
	output << "				ep_CovChkTc : \"COV\";"<< endl;
	output << "				items_pattern : \"(HDL,instance)"<< logicalInstance <<".state_cov_I\";"<< endl;
	output << "			}"<< endl;
	output << "		}"<< endl;

	// Transitions coverage
	//----------------------------
	output << " section \"Transition Coverage\" {" << endl;
	output << "			verification_scope : \"default\";" << endl;
	output << "			logical_instances : "<< logicalInstance <<";" << endl;


	//-- Transitions loop
	int transitionCount = 0;
	map<string,int> transitionsNames;// Map to solve multiple identical transition names
	FOREACH_TRANSITIONS(fsm)


        //-- Get target state and source state
        string sstate = this->cleanString(transition->getStartState()->getName());
        string tstate = this->cleanString(transition->getEndState()->getName());

        // Prepare Name
        //-------------------------
        stringstream transName;
        transName << "tc_" << this->cleanString(transition->getName());
        //transName << "tc_" << fsm->getCurrentTrans()->tname;

        //-- Unnamed get a dummy name
        if (transition->getName().size() == 0) {
            transName << this->cleanString(sstate) << "_to_" << this->cleanString(tstate);
        }


        //-- Check buildup name doesn_t already exists, if yes, then increment the counter on it

        // Increment value in map
        if (transitionsNames.count(transName.str())==0)
            transitionsNames.insert(pair<string,int>(transName.str(),0));
        transitionsNames[transName.str()]++;

        // Suffix count value
        if (transitionsNames[transName.str()]>1) {
            transName<<transitionsNames[transName.str()];
        }

        //-- Default transition (not condition on input to be checked)
        //------------------------------
        if (transition->isDefault()) {


            output << "         coverage \""<< transName.str().c_str() << "_default" <<"\" {"<< endl;
            output << "             ep_CovChkTc : \"COV\";"<< endl;
            output << "             items_pattern : \"(HDL,instance)"<< logicalInstance <<"."<< transName.str().c_str() << "_default" <<"\";"<< endl;
            output << "         }"<< endl;


        }
        //-- Otherwise one cover property per condition
        //------------------------------
        else {
            int cCount = 0 ;
            FOREACH_TRANSITION_CONDITIONS(transition)

                //-- Property Name
                stringstream propertyName;
                propertyName << transName.str();

                //-- Add Condition index and condition name (if one)
                if (transition->getConditions().size()>1)
                    propertyName << "_c" << cCount;
                if (condition->getName().size()>0)
                    propertyName << "_" << this->cleanString(condition->getName());


                output << "         coverage \""<< propertyName.str().c_str() <<"\" {"<< endl;
                output << "             ep_CovChkTc : \"COV\";"<< endl;
                output << "             items_pattern : \"(HDL,instance)"<< logicalInstance <<"."<< propertyName.str().c_str() <<"\";"<< endl;
                output << "         }"<< endl;

                cCount++;

            END_FOREACH_TRANSITION_CONDITIONS

        }


    END_FOREACH_TRANSITIONS

	//-- Hypertransitions loop

	output << "		}"<< endl;
	output << "	}"<< endl;
	output << "	section \"automatic_top\" {"<< endl;
	output << "		instantiate \""<< sectionInstance <<"\" {"<< endl;
	output << "		}"<< endl;
	output << "	}"<< endl;
	output << "	perspective \"[automatic top]\" {"<< endl;
    output << "		top_section : \"automatic_top\";"<< endl;
	output << "	}"<< endl;



}

VerificationPlanGenerator::~VerificationPlanGenerator() {
	// TODO Auto-generated destructor stub
}


string VerificationPlanGenerator::cleanString(string input) {

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

