/*
 * QXMLSave.cpp
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */


// Includes
//--------------

//-- Std
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <sstream>
using namespace std;

//-- Qt
#include <QtCore>
#include <QFileInfo>

//-- Core
#include <core/Utils.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/SaveFactory.h>
#include <core/Project.h>
#include <core/Trackpoint.h>
#include <core/Join.h>
#include <core/State.h>
#include <core/Trans.h>


#include "QXMLSave.h"

// Register the saver in factory
Save_I * SaveFactory::saver = new QXMLSave();



QXMLSave::QXMLSave() {
	// TODO Auto-generated constructor stub

}

QXMLSave::~QXMLSave() {
	// TODO Auto-generated destructor stub
}

void QXMLSave::createDTD(string dirname) {

}
void QXMLSave::createProjectDTD(string dirname) {

}

void QXMLSave::save(string filename, string fsmname) {



    // Write the FSM to a string buffer before writing out to a file
    // Otherwise any SEGFAULT happenning while saving the FSM will mangle the file
    //---------------

    // Determing platform codec
   // QString::from
    ;

    // Prepare Output String
    //-------------
    stringstream out;
    out << "<?xml version='1.0' encoding='UTF-8' ?>" << endl;

    // Call Write FSM on the right FSM
    //-------------
    Core& core = *(Core::getInstance());
    Fsm * fsm = core.getProject()->findFSM(fsmname);
    if (fsm!=NULL) {
        this->writeFSM(fsm, out);
    }




	// Write to file at the end
	//-------------
	ofstream fout(filename.c_str(), ios_base::out | ios_base::trunc);
	fout << out.str();
	fout << flush;
	fout.close();

}

void QXMLSave::saveProject(string filename) {

    Core& core = *(Core::getInstance());

	// Set back to project
	//---------------
	QFileInfo fileInfo = QFileInfo(filename.c_str());
	core.getProject()->setFileInfo(fileInfo);

	// Write the FSM to a string buffer before writing out to a file
    // Otherwise any SEGFAULT happenning while saving the FSM will mangle the file
    //---------------

	// Prepare Output String
    //-------------
    stringstream out;
    out << "<?xml version='1.0' encoding='UTF-8' ?>" << endl;
	out << "<project pname='" << core.getProject()->getName().toStdString() << "' version=\""<<Utils::getMajorVersion() <<"." << Utils::getMinorVersion()<<"\">" <<endl;

	// Foreach FSMs and call Write FSM
	//-------------
	QList<Fsm*> fsms = core.getProject()->getFSMs();
	for (QList<Fsm*>::iterator it = fsms.begin() ; it!=fsms.end();it++) {

		// Get FSM and write it
		Fsm * currentFsm = *it;
		this->writeFSM(currentFsm, out);
	}


	// Close project and write to file
	//----------------
	out << "</project>" << endl;

	ofstream fout(filename.c_str(), ios_base::out | ios_base::trunc);
	fout << out.str();
	fout << flush;
	fout.close();

}

void QXMLSave::writeFSM(Fsm * f, stringstream& out) {

	out << "  <fsm fname='" << f->getFsmName() << "' resetstate='"
			<< f->getResetState();
	out << "' numberofinputs='" << f->getNumberOfInputs()
			<< "' numberofoutputs='";
	out << f->getNumberOfOutputs();
	out	<< "'>" << endl;

	//-- Tools parameters
	//--  * Only write out for a user if the parameters map is not empty
	//---------------------------------
	out << "    " << "<ToolsParameters>" << endl;

	    map<string , map<string,string>* >& usersParametersMap = f->getParametersMap();
	    //map<string , map<string,string>& >::iterator parametersId = parametersMap.begin()
	    for (map<string , map<string,string>* >::iterator it = usersParametersMap.begin() ; it != usersParametersMap.end() ; it++  ) {

	        string user = (*it).first;
            map<string,string>* parametersMap = (*it).second;
            if (parametersMap->size()>0)
                out << "    " << "    <Parameters userid=\""<< (*it).first <<"\">" << endl;

	        // Write out Map for user
	        //-----------------
	        for (map<string,string>::iterator pit = parametersMap->begin() ; pit != parametersMap->end() ; pit++  ) {
	            out << "    " << "         <Parameter key=\""<< (*pit).first <<"\">"<< (*pit).second << "</Parameter>"<<endl ;
	        }

	        if (parametersMap->size()>0)
	            out << "    " << "    </Parameters>" << endl;

	    }

	out << "    " << "</ToolsParameters>"<<endl;

	//---------------------------------//

	out << "    <globals>" << endl;
	out << "        <name>" << f->getClockName() << "</name>" << endl;
	out << "        <name>" << f->getResetName() << "</name>" << endl;
	out << "    </globals>" << endl;
	out << "    <inputnames>" << endl;
	for (int i = 0; i < f->getNumberOfInputs(); i++) {
		out << "        <name><![CDATA[" << f->getInputName(i) << "]]></name>" << endl;
	}
	out << "    </inputnames>" << endl;
	out << "    <outputnames>" << endl;
	for (int i = 0; i < f->getNumberOfOutputs(); i++) {
		out << "        <name><![CDATA[" << f->getOutputName(i) << "]]></name>" << endl;
	}
	out << "    </outputnames>" << endl;

//	if (f->getFirstParameter()) {
//		out << "    <parameter>" << endl;
//		do {
//			out << "        <condition>" << endl;
//			out << "            <cname>" << f->getParameterName() << "</cname>"
//					<< endl;
//			out << "            <input>" << f->getParameterValue()
//					<< "</input>" << endl;
//			out << "        </condition>" << endl;
//		} while (f->getNextParameter());
//		out << "    </parameter>" << endl;
//	}

	// States
	//-------------------------
	FOREACH_STATE(f);

	    out << "    <state id='"<< state->getId() <<"' posx='" << state->getPosition().first << "' posy='"
	                        << state->getPosition().second << "' color='";
        out << state->getColor() << "' >" << endl;
        out << "        <sname><![CDATA[" << state->getName() << "]]></sname>"     << endl;
        out << "        <output>" << state->getOutput() << "</output>" << endl;
        out << "    </state>" << endl;
	END_FOREACH_STATE


	// Transitions
	//-------------------------------------
	FOREACH_TRANSITIONS(f)

	    //-- Properties
        out << "    <trans id='"<< transition->getId() <<"' color='" << transition->getColor() << "' textposx='"
                << transition->getTextPosition().first << "' textposy='"
                << transition->getTextPosition().second << "'>" << endl;
        out << "        <name><![CDATA[" << transition->getName() << "]]></name>" << endl;
        out << "        <default>" << transition->isDefault() << "</default>"
                << endl;

        out << "        <start>" << transition->getStartState()->getId() << "</start>"<< endl;
        out << "        <end>" << transition->getEndState()->getId() << "</end>"<< endl;

        //-- Conditions
        FOREACH_TRANSITION_CONDITIONS(transition)
            out << "        <condition>" << endl;
            out << "            <cname><![CDATA[" << condition->getName()
                    << "]]></cname>" << endl;
            out << "            <input>" << condition->getInput()
                    << "</input>" << endl;
            out << "        </condition>" << endl;
        END_FOREACH_TRANSITION_CONDITIONS

        //-- Trackpoints
        FOREACH_TRANSITION_TRACKPOINTS(transition)

            out << "        <trackpoint posx='"
                    << trackpoint->getPosition().first << "' posy='"
                    << trackpoint->getPosition().second;

            out << "' link='" << trackpoint->isLink() << "'";
            out << " linkid='" << (trackpoint->getTargetLink()!=NULL?trackpoint->getTargetLink()->getId():0) << "'";

            //qDebug() << "Transaction trackpoint join is: " << f->getTransTrackpointJoin();

            out << " join='" << (trackpoint->getJoin()!=NULL ? trackpoint->getJoin()->getId() : 0)
                    << "' color='" << trackpoint->getColor();
            out << "' />" << endl;

        END_FOREACH_TRANSITION_TRACKPOINTS

        // Close transition
        out << "    </trans>" << endl;

	END_FOREACH_TRANSITIONS

	// Hypertransitions
	//-----------------------------
	FOREACH_HYPERTRANSITIONS(f)

			out << "    <hypertrans id='"<< hypertransition->getId() <<"' posx='" << hypertransition->getPosition().first
					<< "' posy='" << hypertransition->getPosition().second << "' targetState='"<< hypertransition->getTargetState()->getId() <<"' color='";
			out << hypertransition->getColor() << "' >" << endl;
			out << "        <type>" << hypertransition->getType() << "</type>"
					<< endl;

			//-- Write name
			out << "        <name><![CDATA[" << hypertransition->getName() << "]]></name>"<< endl;

			//-- Write Conditions
			FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)
                out << "        <condition>" << endl;
                out << "            <cname><![CDATA["
                        << condition->getName() << "]]></cname>"
                        << endl;
                out << "            <input>"
                        << condition->getInput() << "</input>"
                        << endl;
                out << "        </condition>" << endl;
			END_FOREACH_HYPERTRANSITION_CONDITIONS

			//-- Trackpoints
            FOREACH_HYPERTRANSITION_TRACKPOINTS(hypertransition)

                out << "        <trackpoint posx='"
                        << trackpoint->getPosition().first << "' posy='"
                        << trackpoint->getPosition().second;

                out << "' link='" << trackpoint->isLink()
                        << "' linkid='" << (trackpoint->getTargetLink()!=NULL?trackpoint->getTargetLink()->getId():0);

                //qDebug() << "Transaction trackpoint join is: " << f->getTransTrackpointJoin();

                out << "' join='" << trackpoint->getJoin()->getId()
                        << "' color='" << trackpoint->getColor();
                out << "' />" << endl;
                END_FOREACH_HYPERTRANSITION_TRACKPOINTS

			// Close hypertrans
			out << "    </hypertrans>" << endl;

	END_FOREACH_HYPERTRANSITIONS

	// Links
	//----------------------
	FOREACH_LINKS(f)
        out << "    <link id='"<< link->getId() <<"' posx='" << link->getPosition().first << "' posy='"
                << link->getPosition().second << "' color='";
        out << link->getColor() << "' goal='" << link->getTargetState()->getId();
        out << "' />" << endl;
	END_FOREACH_LINKS


	// Joins
	//-------------
	FOREACH_JOIN(f)

		//-- Join Start
		out << "	<Join";
		out << " id=\""				<< join->getId() <<"\"";
		out << " posx=\""			<< join->getPosx() <<"\"";
		out << " posy=\""			<< join->getPosy() <<"\"";
		out << " targetState=\""	<< join->getTargetState()->getId() <<"\"";
		out << ">" << endl;

		//-- Trackpoints
		list<Trackpoint*> trackpoints = join->getTrackpoints();
		for (list<Trackpoint*>::iterator tit = trackpoints.begin();tit!=trackpoints.end();tit++) {

			out << "		<trackpoint";

			out << " posx=\""			<< (*tit)->getPosition().first <<"\"";
			out << " posy=\""			<< (*tit)->getPosition().second <<"\"";
			out << " color=\""			<< (*tit)->getColor()<<"\"";
			out << " link=\""			<< (*tit)->isLink()<<"\"";
			out << " linkid=\""			<< ((*tit)->getTargetLink()!=NULL?(*tit)->getTargetLink()->getId():0)<<"\"";

			out << "/>" << endl;


		}


		//-- Join End
		out << "	</Join>" << endl;
	END_FOREACH_JOIN


	out << "  </fsm>" << endl;
	;

}
