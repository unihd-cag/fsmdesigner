/*
 * QXMLLoad.cpp
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */




// Includes
//-----------------

//-- STL
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <error.h>
#include <sstream>
using namespace std;

//-- Qt
#include <QtXml>
#include <QtCore>
#include <QFileInfo>

//-- Core
#include <core/Utils.h>
#include <core/LoadFactory.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Trackpoint.h>
#include <core/Join.h>

#include "QXMLLoad.h"

// Register the saver in factory
Load_I * LoadFactory::loader = new QXMLLoad();



QXMLLoad::QXMLLoad() {


}

QXMLLoad::~QXMLLoad() {
}

const char * QXMLLoad::getAttributeValue(const char * attribute,QDomElement element) {
	return element.attribute(QString(attribute)).toStdString().c_str();

}

 const QDomElement  QXMLLoad::getFirstMatchingChild(const char * childName,QDomElement element) {

	return element.elementsByTagName(QString(childName)).item(0).toElement();
}

 QList<QDomElement> QXMLLoad::getChildElements(const char * childName,QDomElement element) {

	QDomNodeList children = element.elementsByTagName(QString(childName));
	QList<QDomElement> childrenList;
	for (int i = 0;i<children.length();i++ )
		childrenList.push_back(children.item(i).toElement());

	return childrenList;
}

string QXMLLoad::getChildText(const char * childName,QDomElement element) {
	return QXMLLoad::getFirstMatchingChild(childName,element).text().toStdString();
}


QDomDocument * QXMLLoad::parseFile(string file) {

	// Create Document builder
	//-------------
	QDomDocument * document = new QDomDocument();



	// Parse
	//--------
	try {
		QFile xmlFile(file.c_str());
		 if (!xmlFile.open(QIODevice::ReadOnly))
		     throw runtime_error(string("File does not exist: ")+file);

		//-- Parsing
		int errorLine;
		int errorColumn;
		QString errorMessage;
		if (!document->setContent(&xmlFile,true,&errorMessage,&errorLine,&errorColumn)) {
			// Prepare error
			stringstream buildedError;
			buildedError << "Document Parsing Failed at Line" << errorLine << "," << errorColumn << ":" << errorMessage.toStdString();
			throw runtime_error(buildedError.str());

		}
		return document;
	}
	catch (exception& e) {
		cerr << "[QXMLLoad] Unexpected Exception: " << e.what() << endl;
		throw e;
		return NULL;
	}

	return document;


}

/// Load a project
void QXMLLoad::loadProject(string file) {

	this->load(file);
}

/// Load a single FSM
void QXMLLoad::load(string file) {

	// Parse
	//-----------
	QDomDocument * document = this->parseFile(file);
	if (document==NULL)
		return;

	// Clear Available FSms from core
	Core& c = *(Core::getInstance());
	c.reset();



	// Convert
	//--------------------
	this->convert(document);


	// Analyse
	//-------------

	// Set project dir
	QFileInfo fileInfo = QFileInfo(file.c_str());

	c.getProject()->setFileInfo(fileInfo);


	// Available fsms
	QList<QDomElement> fsmList;


	//-- Get Document Element local name
	//-- Get FSMS if it is a project
	if (document->documentElement().nodeName()==QString("project")) {

		// Set name
		c.getProject()->setName(string(QXMLLoad::getAttributeValue("pname",document->documentElement())));


		// List FSMS
		QDomNodeList fsmNodes = document->documentElement().elementsByTagName(QString("fsm"));
		for (int i=0;i<fsmNodes.length();i++) {
			fsmList.push_back(fsmNodes.item(i).toElement());
		}
	} else if(document->documentElement().nodeName()==QString("fsm")) {
		// Document Element is the only FSM
		fsmList.push_back(document->documentElement());
	}

	//-- Process all the FSMS
	for (QList<QDomElement>::iterator it = fsmList.begin();it!=fsmList.end();it++) {
		this->parseFSM(*it);
	}

	// Clear memory
	delete document;

}

void QXMLLoad::parseFSM(QDomElement fsmElement) {

	// Preparation
	int loopcounter =0;

	// Get Core
	Core& c = *(Core::getInstance());

	// Add FSM to core
	//-----------------
	Fsm * fsm = c.getProject()->addFSM();

	//-- FSM attributes
	fsm->setName(QXMLLoad::getAttributeValue("fname",fsmElement));
	fsm->setResetState(atoi(QXMLLoad::getAttributeValue("resetstate",fsmElement)));

	//-- Generators parameters
	//--------------------------------------
	QList<QDomElement> toolsParameters = QXMLLoad::getChildElements("ToolsParameters",fsmElement);
	if (toolsParameters.size()>0) {

		//-- Get parameters for a user
		QList<QDomElement> userParameters = QXMLLoad::getChildElements("Parameters",toolsParameters.front());
		for (QList<QDomElement>::iterator it=userParameters.begin();it!=userParameters.end();it++) {

		    //---- Get user id
		    string userid = QXMLLoad::getAttributeValue("userid",*it);

		    //---- Get All parameters
		    QList<QDomElement> parameters = QXMLLoad::getChildElements("Parameter",*it);
		    for (QList<QDomElement>::iterator pit=parameters.begin();pit!=parameters.end();pit++) {

		        // Record
		        fsm->setParameter(userid,QXMLLoad::getAttributeValue("key",*pit),(*pit).text().toStdString());
		    }
		}
	}

	//-- Globals


	//-- Inputs
	QList<QDomElement> inputnames = QXMLLoad::getChildElements("name",QXMLLoad::getFirstMatchingChild("inputnames",fsmElement));
	loopcounter =0;
	for (QList<QDomElement>::iterator it=inputnames.begin();it!=inputnames.end();it++,loopcounter++) {
		fsm->addInput((*it).text().toStdString().c_str());
	}

	//-- Outputs
	QList<QDomElement> outputnames = QXMLLoad::getChildElements("name",QXMLLoad::getFirstMatchingChild("outputnames",fsmElement));
	loopcounter =0;
	for (QList<QDomElement>::iterator it=outputnames.begin();it!=outputnames.end();it++,loopcounter++) {
		fsm->addOutput((*it).text().toStdString().c_str());
	}

	//-- States
	QList<QDomElement> states = QXMLLoad::getChildElements("state",fsmElement);
	loopcounter =0;
	for (QList<QDomElement>::iterator it=states.begin();it!=states.end();it++,loopcounter++) {

		//-- Create State and set parameters
		State * currentState = new ::State(fsm->getNumberOfOutputs());
		currentState->setId(atoi(QXMLLoad::getAttributeValue("id",*it)));
		currentState->setName(QXMLLoad::getChildText("sname",*it));
		currentState->setOutput(QXMLLoad::getChildText("output",*it));
		currentState->setPosition(pair<double,double>(atof(QXMLLoad::getAttributeValue("posx",*it)),atof(QXMLLoad::getAttributeValue("posy",*it))));
		currentState->setColor(atoi(QXMLLoad::getAttributeValue("color",*it)));
		currentState->setReset(true);

		//string name = QXMLLoad::getChildText("sname",*it);

		//qDebug() << "State name: "<< QString::fromStdString(name) << "//" << QString::fromStdString(currentState->getName());

		fsm->addState(currentState);


	} // EO States --//

	//---- Transitions
	//--------------------
	QList<QDomElement> transitions = QXMLLoad::getChildElements("trans",fsmElement);
	loopcounter =0;
	for (QList<QDomElement>::iterator it=transitions.begin();it!=transitions.end();it++,loopcounter++) {

		//-- Create trans and set params
		//cout << "Trans: " <<atoi(QXMLLoad::getChildText("start",*it)) <<"->" <<atoi(QXMLLoad::getChildText("end",*it)) << ", Default: "<< (atoi(QXMLLoad::getChildText("default",*it))==1?true:false)  <<endl;

	    //-- Get start and end
	    State * start = fsm->getStatebyID(atoi(QXMLLoad::getChildText("start",*it).c_str()));
	    State * end   = fsm->getStatebyID(atoi(QXMLLoad::getChildText("end",*it).c_str()));


	    //-- Create
	    Trans * addedTransition = new Trans(start,end);
	    addedTransition->setId(atoi(QXMLLoad::getAttributeValue("id",*it)));

		//-- Text
	    addedTransition->setName(string(QXMLLoad::getChildText("name",*it)));
	    addedTransition->setTextPosition((double)atof(QXMLLoad::getAttributeValue("textposx",*it)),
                (double)atof(QXMLLoad::getAttributeValue("textposy",*it)));

        //-- Default
        addedTransition->setDefault((atoi(QXMLLoad::getChildText("default",*it).c_str())==1?true:false));

        //-- Add to FSM
        fsm->addTrans(addedTransition);

		//---- Manage trackpoints
		//---------------------------
		QList<QDomElement> trackpoints = QXMLLoad::getChildElements("trackpoint",*it);
		int loopcounter2 =0;
		for (QList<QDomElement>::iterator it2=trackpoints.begin();it2!=trackpoints.end();it2++,loopcounter2++) {

			//-- Add
			Trackpoint * addedTrackpoint = addedTransition->appendTrackpoint(
					(double)atof(QXMLLoad::getAttributeValue("posx",*it2)),
							(double)atof(QXMLLoad::getAttributeValue("posy",*it2)));

			//-- Link ?
			bool link = atoi(QXMLLoad::getAttributeValue("link",*it2))==1?true:false;
			if (link) {
			    addedTrackpoint->setTargetLink(atoi(QXMLLoad::getAttributeValue("linkid",*it2)));
			}

			//-- Join
			int joinid = atoi(QXMLLoad::getAttributeValue("join",*it2));
			addedTrackpoint->setJoinID(joinid);

			//-- Color
			addedTrackpoint->setColor(atoi(QXMLLoad::getAttributeValue("color",*it2)));

		}


		//-- Conditions
		QList<QDomElement> conditions = QXMLLoad::getChildElements("condition",*it);
		loopcounter2 =0;
		for (QList<QDomElement>::iterator it2=conditions.begin();it2!=conditions.end();it2++,loopcounter2++) {

			// add
		    Condition * condition = addedTransition->addCondition(fsm->getNumberOfInputs());
		    condition->setName(QXMLLoad::getChildText("cname",*it2));
		    condition->setInput(QXMLLoad::getChildText("input",*it2));

		} // EO Conditions --//


	} // EO Transitions --//

	//-- Links
	//----------------
	QList<QDomElement> links = QXMLLoad::getChildElements("link",fsmElement);
	loopcounter =0;
	for (QList<QDomElement>::iterator it=links.begin();it!=links.end();it++,loopcounter++) {

	    //-- Create
	    Link * link = new Link(fsm->getStatebyID(atoi(QXMLLoad::getAttributeValue("goal",*it))),atof(QXMLLoad::getAttributeValue("posx",*it)),
                atof(QXMLLoad::getAttributeValue("posy",*it)));
	    link->setId(atoi(QXMLLoad::getAttributeValue("id",*it)));

	    //-- Color
        link->setColor(atoi(QXMLLoad::getAttributeValue("color",*it)));

		//-- Add
	    fsm->addLink(link);



	}

	//-- Hypertrans
	//----------------
	QList<QDomElement> hypertrans = QXMLLoad::getChildElements("hypertrans",fsmElement);
	loopcounter =0;
	for (QList<QDomElement>::iterator it=hypertrans.begin();it!=hypertrans.end();it++,loopcounter++) {

	    //-- Create
	    Hypertrans * hyperTransition = new Hypertrans();
	    hyperTransition->setType(atoi(QXMLLoad::getChildText("type",*it).c_str()) == 0 ? Hypertrans::FromReset : Hypertrans::FromAllStates);
	    hyperTransition->setTargetState(fsm->getStatebyID(atoi(QXMLLoad::getChildText("goal",*it).c_str())));
	    hyperTransition->setPosition(make_pair( atof(QXMLLoad::getAttributeValue("posx",*it)),atof(QXMLLoad::getAttributeValue("posy",*it))));
	    hyperTransition->setId(atoi(QXMLLoad::getAttributeValue("id",*it)));

		//-- Color
	    hyperTransition->setColor(atoi(QXMLLoad::getAttributeValue("color",*it)));

	    //-- Add
        Hypertrans * hypertrans =  fsm->addHypertrans(hyperTransition);

		//-- Conditions
		QList<QDomElement> conditions = QXMLLoad::getChildElements("condition",*it);
		int loopcounter2 =0;
		for (QList<QDomElement>::iterator it2=conditions.begin();it2!=conditions.end();it2++,loopcounter2++) {

			// add
		    Condition* condition = hypertrans->addCondition(fsm->getNumberOfInputs());
		    condition->setName(QXMLLoad::getChildText("cname",*it2));
		    condition->setInput(QXMLLoad::getChildText("input",*it2));

		} // EO Conditions --//

		//-- Trackpoints ?

	}


	//-- Joins
	//----------------
	QList<QDomElement> joins = QXMLLoad::getChildElements("Join",fsmElement);
	loopcounter =0;
	for (QList<QDomElement>::iterator it=joins.begin();it!=joins.end();it++,loopcounter++) {

		//-- Create
		Join * newJoin = new Join();

		//-- Id
		newJoin->setId(atoi(QXMLLoad::getAttributeValue("id",*it)));

		//-- Position
		newJoin->setPosx(atof(QXMLLoad::getAttributeValue("posx",*it)));
		newJoin->setPosy(atof(QXMLLoad::getAttributeValue("posy",*it)));

		//-- target State
		newJoin->setTargetState(fsm->getStatebyID(atoi(QXMLLoad::getAttributeValue("targetState",*it))));

		//-- Add join to FSM
        fsm->addJoin(newJoin);

		//---- Trackpoints
		//-----------------------
		QList<QDomElement> trackpoints = QXMLLoad::getChildElements("trackpoint",*it);
		int loopcounter2 =0;
		for (QList<QDomElement>::iterator it2=trackpoints.begin();it2!=trackpoints.end();it2++,loopcounter2++) {

			//-- Add
			Trackpoint * addedTrackpoint = newJoin->addTrackpoint();

			//-- Position
			addedTrackpoint->setPosition(make_pair((double)atof(QXMLLoad::getAttributeValue("posx",*it2)),(double)atof(QXMLLoad::getAttributeValue("posy",*it2))));

			//-- Link ?
			bool link = atoi(QXMLLoad::getAttributeValue("link",*it2))==1?true:false;
			addedTrackpoint->setTargetLink(atoi(QXMLLoad::getAttributeValue("linkid",*it2)));

			//-- Color
			addedTrackpoint->setColor(atoi(QXMLLoad::getAttributeValue("color",*it2)));

			//-- Hierarchy

		}




	}

}

void QXMLLoad::convert(QDomDocument * document) {

	// Try to find the version of the FSM/project
	//------------------------
	uint majorVersion = 0;
	uint minorVersion = 0;
	if (document->documentElement().hasAttribute("version")) {

		//-- Get string
		QString version = document->documentElement().attribute("version");

		//-- Split in major and minor version
		majorVersion = version.split('.').at(0).toUInt();
		minorVersion = version.split('.').at(1).toUInt();
	}

	// If we are at the current version, don't do anything
	//----------------
	if (majorVersion==FSMDesigner_VERSION_MAJOR && minorVersion == FSMDesigner_VERSION_MAJOR) {
		return;
	}
	// Conversion Chain
	//--------------------------------
	else {

	    //-- List FSMs
	    QList<QDomElement> fsms;
        if (document->documentElement().nodeName()==QString("project")) {
            // List FSMS
            QDomNodeList fsmNodes = document->documentElement().elementsByTagName(QString("fsm"));
            for (int i=0;i<fsmNodes.length();i++) {
                fsms+=(fsmNodes.item(i).toElement());
            }
        } else if(document->documentElement().nodeName()==QString("fsm")) {
            // Document Element is the only FSM
            fsms+=(document->documentElement());
        }

	    //---- Conversion from pre-5 (version stays 0)
	    if (majorVersion==0) {

	        // Pre5 conversion to version 5.0
	        // Convert Joins
            //-----------------------

            QMap<int,QDomElement> joinsMap; /// target state <-> join


            //---- Go through FSMS
            //-------------------------

            for (QList<QDomElement>::iterator fsm = fsms.begin();fsm!=fsms.end();fsm++) {


                //----- Go through Transitions and trackpoints
                //--------------------
                QList<QDomElement> transitions = QXMLLoad::getChildElements("trans",(*fsm));

                for (QList<QDomElement>::iterator it=transitions.begin();it!=transitions.end();it++) {


                    //---- Go through trackpoints
                    //---------------------------
                    QList<QDomElement> trackpoints = QXMLLoad::getChildElements("trackpoint",*it);
                    for (QList<QDomElement>::iterator it2=trackpoints.begin();it2!=trackpoints.end();it2++) {


                        //-- Join ??
                        int joinid = atoi(QXMLLoad::getAttributeValue("join",*it2));
                        if (joinid==1) {

                            //-- Create or reuse a New XML Join for this target, and set the join to the produced id
                            int targetState = atoi(QXMLLoad::getChildText("end",*it).c_str());
                            QDomElement joinElement;
                            if (!joinsMap.contains(targetState)) {

                                //-- Create the join
                                joinElement = document->createElement("Join");
                                (*fsm).appendChild(joinElement);
                                joinElement.setAttribute("id",(joinsMap.size())+1);
                                joinElement.setAttribute("targetState",targetState);
                                joinElement.setAttribute("posx",QXMLLoad::getAttributeValue("posx",*it2));
                                joinElement.setAttribute("posy",QXMLLoad::getAttributeValue("posy",*it2));
                                joinsMap[targetState] = joinElement;
                            } else
                                joinElement = joinsMap[targetState];


                            //-- Set join id to the trackpoint
                            (*it2).setAttribute("join",joinElement.attribute("id","1"));

                        } // EOF convert join


                    } // EOF trackpoints
                } // EOF Transitions --//
            } // EOF FSMs


	        // We are now 5.0 compatible
            majorVersion = 5;
            minorVersion = 0;

	    } // EOF Pre-5

	    //---- Conversion from 5.0 to 5.1
	    //----   * All states / transitions / Links must have ids
	    if (majorVersion==5 && minorVersion==0) {

	        // Set Version
	        //---------------------
	        document->documentElement().setAttribute("version","5.1");

	        //---- Go through FSMS
            //-------------------------
            for (QList<QDomElement>::iterator fsm = fsms.begin();fsm!=fsms.end();fsm++) {

                unsigned int ids = 1;

                // Reset state must be incremented
                //-------------------------
                unsigned int resetstate = atoi(QXMLLoad::getAttributeValue("resetstate",(*fsm)))+1;
                (*fsm).setAttribute("resetstate",Utils::itos(resetstate).c_str());

                // States must have IDs
                // Set the id to the number in the list is sufficient
                //-----------------------------------
                QList<QDomElement> states = QXMLLoad::getChildElements("state",(*fsm));
                for (QList<QDomElement>::iterator it=states.begin();it!=states.end();it++,ids++) {
                    stringstream ss ;
                    ss << ids;
                    (*it).setAttribute("id",Utils::itos(ids).c_str());

                }

                // Links Must Have IDS
                // * Keep map with old IDs to new IDs to allow link resolution in transitions
                // * Update goals: state all get the same id as before but with +1
                //--------------
                map<unsigned int,unsigned int> linksOldTONewIDs;
                QList<QDomElement> links = QXMLLoad::getChildElements("link",(*fsm));
                unsigned int linksCount = 0; // The counter is the old ID
                for (QList<QDomElement>::iterator it=links.begin();it!=links.end();it++,ids++,linksCount++) {

                    // ID
                    (*it).setAttribute("id",Utils::itos(ids).c_str());

                    // Goal
                    unsigned int goal = atoi(QXMLLoad::getAttributeValue("goal",*it))+1;
                    (*it).setAttribute("goal",Utils::itos(goal).c_str());

                    // Map old to new id
                    linksOldTONewIDs[linksCount] = ids;

                }

                // Joins Must Have IDs
                // * Keep map with old IDs to new IDs to allow join resolution in transitions
                // * Update targets: state all get the same id as before but with +1
                //---------------------------
                map<unsigned int,unsigned int> joinsOldTONewIDs;
                QList<QDomElement> joins = QXMLLoad::getChildElements("Join",(*fsm));
                unsigned int joinsCount = 0; // The counter is the old ID
                for (QList<QDomElement>::iterator it=joins.begin();it!=joins.end();it++,ids++,joinsCount++) {

                    // ID
                    (*it).setAttribute("id",Utils::itos(ids).c_str());

                    // Target
                    unsigned int target = atoi(QXMLLoad::getAttributeValue("targetState",*it))+1;
                    (*it).setAttribute("targetState",Utils::itos(target).c_str());

                    // Map old to new id
                    joinsOldTONewIDs[joinsCount] = ids;
                }

                // Transitions must have IDs
                //  * Set the id to the number in the list is sufficient
                //  * Ids start at 1, make +1 on all state ids
                //  * Scan the trackpoints, and adapt the link and join ids
                //-----------------------------------
                QList<QDomElement> transitions = QXMLLoad::getChildElements("trans",(*fsm));
                for (QList<QDomElement>::iterator it=transitions.begin();it!=transitions.end();it++,ids++) {

                    //-- Id
                    stringstream ss ;
                    ss << ids;
                    (*it).setAttribute("id",ss.str().c_str());

                    //-- +1 on state
                    int start = atoi(QXMLLoad::getChildText("start",*it).c_str())+1;
                    int end = atoi(QXMLLoad::getChildText("end",*it).c_str())+1;

                    (*it).removeChild(QXMLLoad::getFirstMatchingChild("start",*it));
                    QDomNode startElt = (*it).appendChild(document->createElement("start"));
                    startElt.appendChild(document->createTextNode(QString().setNum(start)));

                    (*it).removeChild(QXMLLoad::getFirstMatchingChild("end",*it));
                    QDomNode endElt = (*it).appendChild(document->createElement("end"));
                    endElt.appendChild(document->createTextNode(QString().setNum(end)));

                    //-- Adapt Link and Join
                    QList<QDomElement> trackpoints = QXMLLoad::getChildElements("trackpoint",(*it));
                    for (QList<QDomElement>::iterator tit=trackpoints.begin();tit!=trackpoints.end();tit++) {

                        //-- If the trackpoint is a link, adapt linkid
                        bool link = atoi(QXMLLoad::getAttributeValue("link",*tit))==1?true:false;
                        if (link) {
                            // Get new id from map and set

                            unsigned int newid = linksOldTONewIDs[atoi(QXMLLoad::getAttributeValue("linkid",*tit))];
                            (*tit).setAttribute("linkid",Utils::itos(newid).c_str());
                        }

                        //-- If join > 0, adapt id
                        int joinid = atoi(QXMLLoad::getAttributeValue("join",*tit));
                        if (joinid>0) {
                            // Get new id from map and set
                            unsigned int newid = joinsOldTONewIDs[atoi(QXMLLoad::getAttributeValue("joinid",*tit))];
                            (*tit).setAttribute("join",Utils::itos(newid).c_str());
                        }

                    }

                }

                // Hypertransitions must have IDs
                //  * goal must be incremented and set to a targetState attribute
                //----------------------------------------
                QList<QDomElement> hypertransitions = QXMLLoad::getChildElements("hypertrans",(*fsm));
                for (QList<QDomElement>::iterator it=hypertransitions.begin();it!=hypertransitions.end();it++,ids++) {

                    //-- Id
                    stringstream ss ;
                    ss << ids;
                    (*it).setAttribute("id",ss.str().c_str());

                    //-- +1 on goal
                    int goal = atoi(QXMLLoad::getChildText("goal",*it).c_str())+1;
                    (*it).removeChild(QXMLLoad::getFirstMatchingChild("goal",*it));

                    //-- Set goal to targetState
                    (*it).setAttribute("targetState",QString().setNum(goal));



                } // EOF Hypertransitions

            } // EOF FSMs

            // We are now 5.1 compatible
            majorVersion = 5;
            minorVersion = 1;


	    } // EOF 5.0 to 5.1





	}// EOF Conversion chain

	// EOF Convert
	//---------------------
	//qDebug() << document->toString(0);
	//exit(0);



}

