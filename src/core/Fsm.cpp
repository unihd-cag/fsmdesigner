/***********************************************************************
 *                                                                      *
 * (C) 2006, Frank Lemke, Computer Architecture Group,                  *
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
//--------------
//-- Std
#include <cstdlib>
#include <vector>
#include <map>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

//-- Core
#include <core/Fsm.h>
#include <core/Condition.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Trackpoint.h>
#include <core/Hypertrans.h>
#include <core/Link.h>
#include <core/User.h>
#include <core/Join.h>

//-- Utils
#include <core/Utils.h>

#include "Fsm.h"

Fsm::Fsm(int f) {
    fid = f;
    fname = "fsm_";
    fname = fname + Utils::itos(fid);
    resetstate = 0;
    globals.first = "clk";
    globals.second = "res_n";

    //-- Init
    //this->statesMap.clear();
    this->joinsMap.clear();

}

Fsm::~Fsm() {

}

void Fsm::setId(int id) {
    this->fid = id;
}

void Fsm::setName(string str) {
    this->fname = str;
}


void Fsm::setParameter(string key,string value) {

    // Get User id
    //----------
    string userID = User::getUserID();

    // Set Parameter value
    //----------------
    this->setParameter(userID,key,value);


}

void Fsm::setParameter(string userID,string key,string value) {

    // Get the parameters map for this user / create it
    //------------
    map<string,string> * parametersMap = NULL;
    if (this->toolsParameters.count(userID) > 0) {
        parametersMap = this->toolsParameters[userID];
    } else {
        this->toolsParameters[userID] =  new map<string,string>();
        parametersMap = this->toolsParameters[userID];
    }


    //---- Set the parameter value
    //---------------
    if (value.size() == 0 && parametersMap->count(key)>0) {
        parametersMap->erase(parametersMap->find(key));
    } else if (value.size() > 0){
        (*parametersMap)[key] = value;
    }

}

string Fsm::getParameter(string key, string defaultValue) {

    string defaultReturn = defaultValue;

    // Get User id
    //----------
    string userID = User::getUserID();

    // Get the parameters map for this user
    //------------
    if (this->toolsParameters.count(userID) > 0) {

        map<string,string>* parametersMap = this->toolsParameters[userID];

        //---- Get the parameter value
        //---------------
        if (parametersMap->count(key)>0) {
            defaultReturn = (*parametersMap)[key];
        }
    }

    return defaultReturn;

}


void Fsm::setResetState(unsigned int id) {

    //-- Find the actual reset state and set it to not reset
    if (this->getStatebyID(resetstate) != NULL)
        this->getStatebyID(resetstate)->setReset(false);

    //-- Record new reset state
    this->resetstate = id;

    //-- Set new reset state to reset
    if (this->getStatebyID(id) != NULL)
        this->getStatebyID(id)->setReset(true);

}

map<string , map<string,string>* >&  Fsm::getParametersMap() {
    return this->toolsParameters;
}

void Fsm::addInput(string name,char defaultValue) {

    // Add input to list
    //-----------------
    inputnames.push_back(name);


    // Update All conditions
    //-----------------------------

    //-- parameters
    /*for (ci = parameter.begin(); ci != parameter.end(); ci++) {
        for (int a = 0; a < b; a++) {
            if (defaultValue == 0) {
                ci->input.append(1, '0');
            } else if (defaultValue == 1) {
                ci->input.append(1, '1');
            } else if (defaultValue == -1000) {
                ci->input.append(1, '-');
            }
        }
    }*/

    //-- Hypertransitions
    FOREACH_HYPERTRANSITIONS(this)
        FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

            condition->addInputBit(defaultValue);

        END_FOREACH_HYPERTRANSITION_CONDITIONS
    END_FOREACH_HYPERTRANSITIONS

    //-- Transitions
    FOREACH_TRANSITIONS(this)
       FOREACH_TRANSITION_CONDITIONS(transition)

           condition->addInputBit(defaultValue);

            END_FOREACH_TRANSITION_CONDITIONS
    END_FOREACH_TRANSITIONS


}

void Fsm::addOutput(string name,int defaultValue) {

    // Add Output
    //---------------------
    outputnames.push_back(name);

    // Update all States
    //--------------------------
    FOREACH_STATE(this)

        //-- Add output bit
        state->addOutput(defaultValue);

    END_FOREACH_STATE

}

void Fsm::setInputName(unsigned int i, string name) {
    if (i < inputnames.size()) {
        inputnames[i] = name;
    } else
        throw invalid_argument("Cannot change input name because the index is out of bound");
}

void Fsm::setOutputName(unsigned int i, string name) {
    if (i < outputnames.size()) {
        outputnames[i] = name;
    } else
        throw invalid_argument("Cannot change output name because the index is out of bound");
}

void Fsm::deleteInput(int i) {


    if (i < inputnames.size()) {


        //-- Update Parameters
        vector<Condition>::iterator ci;

        for (ci = parameter.begin(); ci != parameter.end(); ci++) {

            //-- Delete input bit at i in condition
            ci->deleteInputAt(i);
        }

        //-- Update Hypertransitions
        FOREACH_HYPERTRANSITIONS(this)
            FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

                condition->deleteInputAt(i);

            END_FOREACH_HYPERTRANSITION_CONDITIONS
        END_FOREACH_HYPERTRANSITIONS

        //-- Update transitions
        FOREACH_TRANSITIONS(this)
            FOREACH_TRANSITION_CONDITIONS(transition)

                condition->deleteInputAt(i);

            END_FOREACH_TRANSITION_CONDITIONS
        END_FOREACH_TRANSITIONS

        //-- Juste remove from vector
        this->inputnames.erase(this->inputnames.begin()+i);

    }
}

void Fsm::deleteOutput(int i) {

    if (i < this->outputnames.size()) {

        //-- Remove corresponding output bit in state
        FOREACH_STATE(this)

            string::iterator help;

            int countfordelete = 0;

            string output = state->getOutput();
            for (help = output.begin(); help != output.end(); help++) {
                if (countfordelete == i) {
                    output.erase(help);
                    help = output.end();
                    help--;
                    state->setOutput(output);
                }
                countfordelete++;
            }
        END_FOREACH_STATE

        //-- Remove output from fsm
        this->outputnames.erase(this->outputnames.begin()+i);

    }
}

void Fsm::moveupInput(int i) {

    if (i < this->inputnames.size() && i > 0) {


        //-- Update parameters
        vector<Condition>::iterator ci;
        for (ci = parameter.begin(); ci != parameter.end(); ci++) {
            char hvar = ci->getInput()[i - 1];
            ci->changeInputAt(i-1,ci->getInput()[i]);
            ci->changeInputAt(i,hvar);
        }

        //-- Update Hypertrans
        FOREACH_HYPERTRANSITIONS(this)
            FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

                char hvar = condition->getInput()[i-1];
                condition->changeInputAt(i - 1, condition->getInput()[i]);
                condition->changeInputAt(i, hvar);

            END_FOREACH_HYPERTRANSITION_CONDITIONS
        END_FOREACH_HYPERTRANSITIONS

        //-- Update transitions
        FOREACH_TRANSITIONS(this)
            FOREACH_TRANSITION_CONDITIONS(transition)

                char hvar = condition->getInput()[i-1];
                condition->changeInputAt(i - 1, condition->getInput()[i]);
                condition->changeInputAt(i, hvar);

            END_FOREACH_TRANSITION_CONDITIONS
        END_FOREACH_TRANSITIONS

        //-- Switch names
        string tmpname = this->getInputName(i - 1);
        this->setInputName(i-1,this->getInputName(i));
        this->setInputName(i,tmpname);

    }
}

void Fsm::moveupOutput(int i) {
    if (i < this->outputnames.size() && i > 0) {

        FOREACH_STATE(this)

            char hvar;

            hvar = state->getOutputAt(i - 1);
            state->setOutputAt(i - 1, state->getOutputAt(i));
            state->setOutputAt(i, hvar);

        END_FOREACH_STATE

        //-- Switch names
        string tmpname = this->getOutputName(i - 1);
        this->setOutputName(i-1,this->getOutputName(i));
        this->setOutputName(i,tmpname);

    }
}

void Fsm::movedownInput(int i) {
    if (i < this->inputnames.size() - 1) {

        //-- Update parameters
        vector<Condition>::iterator ci;
        for (ci = parameter.begin(); ci != parameter.end(); ci++) {

            char hvar = ci->getInput()[i+1];
            ci->changeInputAt(i + 1, ci->getInput()[i]);
            ci->changeInputAt(i, hvar);

        }

        //-- Update Hypertrans
        FOREACH_HYPERTRANSITIONS(this)
            FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition)

                char hvar = condition->getInput()[i+1];
                condition->changeInputAt(i + 1, condition->getInput()[i]);
                condition->changeInputAt(i, hvar);

            END_FOREACH_HYPERTRANSITION_CONDITIONS
        END_FOREACH_HYPERTRANSITIONS

        //-- Update transitions
        FOREACH_TRANSITIONS(this)
            FOREACH_TRANSITION_CONDITIONS(transition)

                char hvar = condition->getInput()[i+1];
                condition->changeInputAt(i + 1, condition->getInput()[i]);
                condition->changeInputAt(i, hvar);

            END_FOREACH_TRANSITION_CONDITIONS
        END_FOREACH_TRANSITIONS

        //-- Switch names
        string tmpname = this->getInputName(i + 1);
        this->setInputName(i+1,this->getInputName(i));
        this->setInputName(i,tmpname);

    }
}

void Fsm::movedownOutput(int i) {
    if (i < this->outputnames.size() - 1) {

        FOREACH_STATE(this)

            char hvar;

            hvar = state->getOutputAt(i + 1);
            state->setOutputAt(i + 1, state->getOutputAt(i));
            state->setOutputAt(i, hvar);

        END_FOREACH_STATE

        //-- Switch names
        string tmpname = this->getOutputName(i + 1);
        this->setOutputName(i+1,this->getOutputName(i));
        this->setOutputName(i,tmpname);

    }
}


string Fsm::getOutputName(int pos) {
    if (pos >= 0 && pos < this->outputnames.size()) {
        return outputnames[pos];
    } else {
        cerr << "Error OutPutNamePosition " << pos << "does not exist" << endl;
        return "not found";
    }
}

string Fsm::getInputName(int pos) {
    if (pos < this->inputnames.size()) {
        return inputnames[pos];
    } else {
        cerr << "Error InputNamePosition does not exist" << endl;
        return "not found";
    }
}

int Fsm::getFsmID() {
    return fid;
}

int Fsm::getNumberOfInputs() {
    return this->inputnames.size();
}

int Fsm::getNumberOfOutputs() {
    return this->outputnames.size();
}

int Fsm::getResetState() {
    return this->resetstate;
}

string Fsm::getFsmName() {
    return this->fname;
}



int Fsm::getStateCount() {
    return this->statesMap.size();
}

void Fsm::clockName(string s) {
    this->globals.first = s;
}

void Fsm::resetName(string s) {
    this->globals.second = s;
}

string Fsm::getClockName() {
    return this->globals.first;
}

string Fsm::getResetName() {
    return this->globals.second;
}


State* Fsm::getStatebyID(int id) {
    if (this->statesMap.count(id) == 0)
        return NULL;
    return this->statesMap[id];
}


map<unsigned int, State*>& Fsm::getStates() {

    return this->statesMap;
}

State** Fsm::getStatesArray() {

    // Create array
    //-------------
    State ** array = (State**) malloc(this->getStateCount()*sizeof(State*));
    //list<State*> statesList;

    // Add all States
    //----------------------
    int i=0;
    FOREACH_STATE(this)
        array[i] = state;
    i++;
        //statesList.push_back(state);
    END_FOREACH_STATE

    // Return list
    //----------------
    return array;
}


State* Fsm::addState() {


    //-- Create state (ID assigned here)
    State * newState = this->addState(new State(this->outputnames.size()));

    //-- Set a default name
    newState->setName("State_" + Utils::itos(this->statesMap.size()));


    return newState;

}

State * Fsm::addState(State * state) {

    //-- If already in the FSM, don't add
    if (state->isIdSet() && this->statesMap.count(state->getId())>0)
        return state;


    //-- Register to ID manager
   this->idManager.assignID(state);


    //-- Should be reset state ?
    bool isReset = false;
    if (this->statesMap.size() == 0)
        isReset = true;
    state->setReset(isReset);
    if (isReset)
        this->setResetState(state->getId());

    //-- Add
    this->statesMap[state->getId()] = state;

    return state;
}

void Fsm::deleteState(State * state) {

    //-- Verify state is present in this FSM
    if (this->statesMap.count(state->getId()) == 0) {
        stringstream message;
        message << "Trying to delete state with id " << state->getId()
                << "not possible because state does not belong to FSM";
        throw new invalid_argument(message.str());
    }

    //-- Remove
    this->statesMap.erase(state->getId());

    //-- Deassign from ID manager
    this->idManager.derefenceObject(state);

}




Trans * Fsm::addTrans(unsigned int startId, unsigned int endId) {

    // Get Start State
    //--------------------
    State * start = this->getStatebyID(startId);
    State * end = this->getStatebyID(endId);

    if (start == NULL || end == NULL) {
        stringstream ss;
        ss
                << "Start or End state id does not match to a State in addTrans(uint,uint) (id/start: "
                << startId << "/" << start << ", id/end: " << endId << "/"
                << end << ")";
        throw invalid_argument(ss.str());
    }

    // Call addTrans with pointers
    //----------------
    return this->addTrans(start, end);

}

Trans * Fsm::addTrans(State * start, State * end) {

    //-- Create transition
    Trans * newTrans = new Trans(start, end);

    //-- Add (ID assigned in there)
    newTrans = this->addTrans(newTrans);

    //-- Auto Adjust name
    newTrans->setName(string("trans_") + Utils::itos(this->transitionsMap.size()));


    return newTrans;
}

Trans * Fsm::addTrans(Trans * transition) {

    //-- If already in the FSM, don't add
    if (transition->isIdSet() && this->transitionsMap.count(transition->getId())>0)
        return transition;

    //-- Set ID
    this->idManager.assignID(transition);

    //-- Add
    this->transitionsMap[transition->getId()] = transition;

    //-- Register to start state
    transition->getStartState()->addStartingTransition(transition);

    return transition;

}

Trans * Fsm::deleteTrans(unsigned int id) {

    //-- Check transition is in the map
    if (this->transitionsMap.count(id)==0)
        throw invalid_argument("Transition with id "+Utils::itos(id)+" is not in the FSM, cannot be deleted");

    Trans * removed = this->transitionsMap[id];

    //-- Remove
    return this->deleteTrans(removed);




    return removed;
}

Trans * Fsm::deleteTrans(Trans * trans) {

    //-- Verify ID is set and allright
    if (!trans->isIdSet())
            throw invalid_argument("Requesting deletion of a transition without an ID. This is not possible");

    //-- Remove from map
    this->transitionsMap.erase(trans->getId());

    //-- Remove from ID manager
    this->idManager.derefenceObject(trans);

    return trans;

}


Hypertrans * Fsm::addHypertrans() {

    //-- Create a new hypertrans
    Hypertrans * hypertransition = new Hypertrans();

    //-- Add and return
    return this->addHypertrans(hypertransition);

}

Hypertrans * Fsm::addHypertrans(Hypertrans * hypertrans) {

    //-- If already in the FSM, don't add
    if (hypertrans->isIdSet() && this->hyperTransMap.count(hypertrans->getId())>0)
        return hypertrans;

    //-- Assign ID
    this->idManager.assignID(hypertrans);

    //-- Add to map
    this->hyperTransMap[hypertrans->getId()] = hypertrans;

    return hypertrans;
}

Hypertrans* Fsm::deleteHypertrans(Hypertrans* hypertransition) {

    //-- Check exists
    if (hypertransition==NULL || !hypertransition->isIdSet()) {
        throw invalid_argument("Cannot delete non existing hypertransition (NULL or not registered in FSM)");
    }

    //-- Remove from Map
    this->hyperTransMap.erase(hypertransition->getId());

    //-- Deassign ID
    this->idManager.derefenceObject(hypertransition);

    return hypertransition;

}
Hypertrans* Fsm::deleteHypertrans(unsigned int id) {

    //-- Call Pointer version of function
    return this->deleteHypertrans(this->getHypertransbyID(id));


}


Link * Fsm::getLinkbyID(unsigned int id) {
    return this->linksMap.count(id) > 0 ? this->linksMap[id] : NULL;
}

Link * Fsm::getLinkbyGoal(State * state) {

    //-- Foreach Links
    Link * result = NULL;
    FOREACH_LINKS(this)
        if (link->getTargetState()==state)
            return link;
    END_FOREACH_LINKS

    return NULL;
}

Link * Fsm::addLink(State * targetState,double posx, double posy) {



    //-- Create link
    Link * newLink = new Link(targetState,posx,posy);

    //-- Add (ID is assigned here)
    this->addLink(newLink);


    return newLink;

}

Link * Fsm::addLink(Link * link) {


    //-- If already in the FSM, don't add
    if (link->isIdSet() && this->linksMap.count(link->getId())>0)
        return link;

    //-- Register to ID manager
    this->idManager.assignID(link);

    //-- Add to map
    this->linksMap[link->getId()] = link;

    return link;

}

Link * Fsm::deleteLink(Link * link) {

    //-- Check exists
    if (!link->isIdSet()) {
        throw invalid_argument("Cannot delete link with id "+Utils::itos(link->getId())+" because it is not registered in the FSM");
    }

    //-- if the Link is used by a transition => Remove the transition and clear memory
    FOREACH_TRANSITIONS(this)
        FOREACH_TRANSITION_TRACKPOINTS(transition)
            if ((trackpoint->getTargetLink() == link)) {
                // Delete transition
                Trans * deletedTransition = this->deleteTrans(transition);
                delete deletedTransition;
                break;
            }
        END_FOREACH_TRANSITION_TRACKPOINTS
    END_FOREACH_TRANSITIONS


    //-- Remove from map
    this->linksMap.erase(link->getId());

    //-- Remove from ID manager
    this->idManager.derefenceObject(link);



    return link;

}


map<unsigned int, Link*>& Fsm::getLinks() {
    return this->linksMap;
}



map<unsigned int, Trans*>& Fsm::getTransitions() {
    return this->transitionsMap;
}

Trans** Fsm::getTransitionsArray() {

    // Create array
    //-------------
    Trans ** array = (Trans**) malloc(this->getTransitions().size()*sizeof(Trans*));
    //list<State*> statesList;

    // Add all States
    //----------------------
    int i=0;
    FOREACH_TRANSITIONS(this)
        array[i] = transition;
    i++;
        //statesList.push_back(state);
    END_FOREACH_TRANSITIONS

    // Return list
    //----------------
    return array;
}


Trans * Fsm::getTransbyID(unsigned int id) {

    return this->transitionsMap.count(id) > 0 ? this->transitionsMap[id] : NULL;

}



map<unsigned int, Hypertrans*>& Fsm::getHypertransitions() {
    return this->hyperTransMap;
}

Hypertrans * Fsm::getHypertransbyID(unsigned int id) {

    return this->hyperTransMap.count(id) > 0 ? this->hyperTransMap[id] : NULL;

}




Join * Fsm::addJoin() {



    //-- Create Join (ID assigned here and all)
    Join * join = this->addJoin(new Join());

    return join;

}

Join * Fsm::addJoin(Join * join) {


    //-- If already in the FSM, don't add
    if (join->isIdSet() && this->joinsMap.count(join->getId())>0)
        return join;

    //-- Assign an ID
    this->idManager.assignID(join);

    //-- Add to map
    this->joinsMap[join->getId()] = join;

    return join;

}

Join * Fsm::deleteJoin(Join * join) {


    //-- Verify state is present in this FSM
    if (this->joinsMap.count(join->getId()) == 0) {
        stringstream message;
        message << "Trying to delete join with id " << join->getId()
                << "not possible because join does not belong to FSM";
        throw new invalid_argument(message.str());
    }

    //-- Remove from map
    this->joinsMap.erase(join->getId());

    //-- Deassign ID
    this->idManager.derefenceObject(join);

    return join;

}

map<unsigned int, Join*>& Fsm::getJoins() {
    return this->joinsMap;
}

Join * Fsm::getJoin(unsigned int id) {
    if (this->joinsMap.count(id) == 0)
            return NULL;
    return this->joinsMap[id];
}
/*
/// Path to last generated verilog source
void Fsm::setLastGeneratedVerilogFile(string path) {

    //-- Get User ID
    string userID = User::getUserID();

    //-- Put into map
    this->lastGeneratedVerilogFile[userID] = path;
}

string Fsm::getLastGeneratedVerilogFile() {

    //-- Get User ID
    string userID = User::getUserID();

    //-- Return
    return this->lastGeneratedVerilogFile[userID];
}

map<string, string> Fsm::getLastGeneratedVerilogFileMap() {
    return this->lastGeneratedVerilogFile;
}

void Fsm::addLastGeneratedVerilogFile(string user, string path) {

    this->lastGeneratedVerilogFile[user] = path;
}*/
