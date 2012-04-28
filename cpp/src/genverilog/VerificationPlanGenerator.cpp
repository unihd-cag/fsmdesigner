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

#include "VerificationPlanGenerator.h"

VerificationPlanGenerator::VerificationPlanGenerator(Fsm * fsm ) {
	this->fsm = fsm;

}

void VerificationPlanGenerator::generate(ostream& output) {


	string sectionInstance("");
	string logicalInstance("FSM_");
	logicalInstance+=this->fsm->getFsmName();
	sectionInstance+=this->fsm->getFsmName();
	string fileName(this->fsm->getFsmName());
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
	FOREACH_TRANSITIONS(this->fsm)


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
        /*if ( fsm->getCurrentTrans()->tname.size()==0) {
            transName<<sstate<<"_to_" << tstate;
        }
        //-- Replace  White spaces with _
        else {
            for (int ci=0;ci< fsm->getCurrentTrans()->tname.size();ci++) {
                if ( fsm->getCurrentTrans()->tname.at(ci)==' ')
                    transName<<"_";
                else
                    transName<< fsm->getCurrentTrans()->tname.at(ci);
            }
        }*/

        //-- Check buildup name doesn_t already exists, if yes, then increment the counter on it

        // Increment value in map
        if (transitionsNames.count(transName.str())==0)
            transitionsNames.insert(pair<string,int>(transName.str(),0));
        transitionsNames[transName.str()]++;

        // Suffix count value
        if (transitionsNames[transName.str()]>1) {
            transName<<transitionsNames[transName.str()];
        }

        output << "			coverage \""<< transName.str() <<"\" {"<< endl;
        output << "				ep_CovChkTc : \"COV\";"<< endl;
        output << "				items_pattern : \"(HDL,instance)"<< logicalInstance <<"."<< transName.str() <<"\";"<< endl;
        output << "			}"<< endl;

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
	inputQString = inputQString.replace(QChar(' '),QChar('_'));


	//-- replaces then non \\W characters by nothing
	//inputQString = inputQString.replace(QRegExp("\\W"),"");

	//-- Only Allow letters and numbers
	inputQString = inputQString.replace(QRegExp("[^A-Za-z0-9_]*"),"");


	return inputQString.toStdString();

}

