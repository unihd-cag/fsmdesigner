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

/*!
 \file fsm.h
 \brief Fsm

 \author Frank Lemke
 \date 2006-02-28
 */

#ifndef FSM_H_
#define FSM_H_

// Includes
//-----------------

//-- Std
#include <string>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <map>
#include <list>
using namespace std;

//-- Core
#include <core/Condition.h>
#include <core/Hypertrans.h>
#include <core/Link.h>
#include <core/IDManager.h>
class State;
class Trans;
class Join;
class Project;

/*!
 \brief Fsm-Object

 Object to store a complete FSM with all needed abilities..
 */

class Fsm {

    protected:

        /// The IDManager for this FSM
        IDManager idManager;

        /// ID of the FSM (useless now, but anyway there for name setting)
        int fid;

        /// The Name of the FSM
        string fname;

        /// The ID of the reset state
        unsigned int resetstate;

        /// Link to the parent Project
        Project * project;

        /// States Map
        map<unsigned int, State *>      statesMap;

        /// Transitions Map
        map<unsigned int, Trans *>      transitionsMap;

        /// Links Map
        map<unsigned int, Link *>       linksMap;

        /// Hypertransition Map
        map<unsigned int, Hypertrans*>  hyperTransMap;

        /// Joins map
        map<unsigned int, Join*>        joinsMap;

        /// Path to last generated verilog source depending on userID
        /*map<string, string> lastGeneratedVerilogFile;*/

        /** A map to store parameters for tools, based on user id (to avoid conflicts)
         *
         * user id <-> (key >-> value )
         *
         * \addtogroup ToolsParameters
         */
        map<string, map<string,string>* > toolsParameters;

        /// COMMENT
        pair<string, string> globals;

        /// COMMENT
        vector<Condition> parameter;

        /// COMMENT
        vector<string> inputnames;

        /// COMMENT
        vector<string> outputnames;

    public:

        Fsm(int f = 0);
        virtual ~Fsm();

        /** \defgroup FSMProperties FSM Properties */
        /** @{ */

        /**
         * Returns the project the FSM belongs to
         * @return A pointer to the project or NULL if the FSM does not belong to any Project
         */
        Project * getProject() {
            return this->project;
        }

        /**
         * Set the project the FSM belongs to
         * @param project
         */
        void setProject(Project * project) {
            this->project = project;
        }

        /**
         * Set the Id of the Fsm (internal usage at runtime)
         * @param i
         */
        void setId(int id);

        /*!
         \brief clockName

         To change clock-name..
         */

        void clockName(string s);

        /*!
         \brief resetName

         To change reset-name..
         */

        void resetName(string s);



        /**
         * Adds a new input at the end of inputvector, and updates all the transitions
         * @param name
         * @param defaultValue
         */
        void addInput(string name, char defaultValue = '-');


        /**
         * Add a new output to the FSM and update existing states
         * @param name
         * @param defaultValue '0' for 0, '1' for 1
         */
        void addOutput(string name, int defaultValue = 0);

        /*!
         \brief inputName

         Changes the inputname of a input.
         \param i Position of chosen input.
         \param name New name of input.
         */

        void setInputName(unsigned int i, string name);

        /*!
         \brief outputName

         Changes the outputname of a output.
         \param i Position of chosen output.
         \param name New name of output.
         */

        void setOutputName(unsigned int i, string name);


        /*!
         \brief deleteInput

         Deletes input i.
         \param i Position of input for deleting.
         */

        void deleteInput(int i);

        /*!
         \brief deleteOutput

         Deletes output i.
         \param i Position of output for deleting.
         */

        void deleteOutput(int i);

        /*!
         \brief moveupInput

         Moves output i one step forward in inputvector.
         \param i Position of input for repositioning.
         */

        void moveupInput(int i);

        /*!
         \brief moveupOutput

         Moves output i one step forward in outputvector.
         \param i Position of output for repositioning.
         */

        void moveupOutput(int i);

        /*!
         \brief movedowninput

         Moves output i one step back in inputvector.
         \param i Position of input for repositioning.
         */

        void movedownInput(int i);

        /*!
         \brief movedownOutput

         Moves output i one step back in outputvector.
         \param i Position of output for repositioning.
         */

        void movedownOutput(int i);

        /*!
         \brief resetState.

         Sets new resetstate.
         \param id ID of new resetstate.
         */

        void setResetState(unsigned int id);

        /*!
         \brief fsmName

         Changes name os Fsm.
         \param str New name for Fsm.
         */
        void setName(string str);


        /** @} */


        /** \defgroup ToolsParameters Userid mapped tools parameters */
        /** @{ */

        /** Sets the value of the parameter at the given key, mapped to the user id. An empty string removes the parameter
         *
         * @param key
         * @param value Removes the parameter is empty string
         */
        void setParameter(string key,string value);

        /**  Sets the value of the parameter at the given key, mapped to the provided user id. An empty string removes the parameter
         *
         * @param userid
         * @param key
         * @param value
         */
        void setParameter(string userid,string key,string value);

        /** Get the value of the parameter at the given key, mapped from the user id
         *
         * @param key
         * @param defaultValue
         */
        string getParameter(string key, string defaultValue = "");

        /**
         * Returns the parameters map
         *
         * @return
         */
        map<string , map<string,string>* >& getParametersMap();


        /** @} */

        /// State FOREACH Macro:
        ///    - Map<unsigned long int,State*> iterator is accessible under the it variable
        ///    - State is accessible under the state variable
        /// Don't forget to #include <map> !!
#define FOREACH_STATE(fsm) \
     for (map<unsigned int,::State*>::iterator it = fsm->getStates().begin();it!=fsm->getStates().end();it++) { \
         ::State * state = it->second;

#define END_FOREACH_STATE }

        /**
         * Returns a copy of the states map (to avoid unwished concurrent modifications)
         * Example:
         *
         *    FOREACH_STATE(fsm) {
         *        ...
         *    }
         *
         * @return A reference to the map ! Modifications allowed then!
         */
        map<unsigned int, State*>& getStates();

        /**
         * Returns an array with pointers to all the states
         * This is useful behaviours needing direct offset access
         * @return
         */
        State** getStatesArray();

        /*!
         \brief addState

         Adds new State at the end of statevector of Fsm.
         */
        State * addState();

        /**
         * Add a provided state object to the model
         * The state id will be adapted
         * @param state The state to add
         */
        State * addState(State * state);


        /**  Delete State of Fsm by state-ID
         *
         * - Does not remove transitions in any way
         *
         * @warning Does not delete the memory
         * @param state
         */
        void deleteState(State * state);


        /**
         * Adds new Transition to Fsm, using state ids
         * @param startId Id of the start State
         * @param endId   Id of the beginning state
         * @return
         */
        Trans * addTrans(unsigned int startId, unsigned int endId);

        /*!
         \brief addTrans

         Adds new Transition to Fsm.
         \param start Startpoint of transition.
         \param end Endpoint of transition.
         */

        Trans * addTrans(State * start, State * end);

        /**
         * Add The already created transition
         * @param
         * @return The provided transition, with id adapted
         */
        Trans * addTrans(Trans *);

        /*!
         \brief deleteTrans


         \param id ID of a Transition.
         \return The deleted transition
         */

        /** Delete Trans by ID.
         *
         * @warning Does not delete the memory
         * @param id ID of a Transition.
         * @return Pointer to the deleted model
         */
        Trans * deleteTrans(unsigned int id);

        /** Delete Trans by pointer
         *
         * @warning Does not delete the memory
         * @param trans Pointer to transition to delete
         * @return
         */
        Trans * deleteTrans(Trans * trans);

        /**
         * Adds an existing Hypertrans object to the FSM
         *
         * @warning Returning immediately if already in the FSM
         * @param link
         * @return A pointer to the added Hypertrans (looping pointer)
         */
        Hypertrans * addHypertrans(Hypertrans * hypertrans);

        /*!
         \brief deleteHypertrans

         Delete Hypertrans by ID.
         \param id ID of Hypertransition.

         @return A pointer to the deleted Hyper transition
         */

        Hypertrans* deleteHypertrans(unsigned int id);


        /*!
         \brief addLink

         Adds Link to Fsm.
         \param targetState Target of %Link.
         \param posx X-position.
         \param posy Y-position.

         @see #addLink(Link*)
         @return A pointer to the added link
         */

        Link * addLink(State * targetState,double posx, double posy);

        /**
         * Adds an existing Link object to the FSM
         *
         * @warning Returning immediately if already in the FSM
         * @param link
         * @return A pointer to the added link (looping pointer)
         */
        Link * addLink(Link * link);

        /**
         * Delete Link by pointer
         *
         * @warning This method will hard delete any transition using this link.
         *          If the user wants to keep track, he should take care of manually
         *          removing the transitions to keep the pointers alive
         *
         * @warning User is responsible for managing memory
         * @param link POinter ot the link
         * @return A pointer to the deleted link
         */
        Link * deleteLink(Link * link);



        /*!
         \brief getOutputName

         To get a outputname.
         \param pos position of output.
         \return string Name of output.
         */

        string getOutputName(int pos);

        /*!
         \brief getInputName

         To get a inputname.
         \param pos position of input.
         \return string Name of input.
         */

        string getInputName(int pos);

        /*!
         \brief getFsmID

         To get ID of Fsm.
         \return int ID of %Fsm.
         */

        int getFsmID();

        /*!
         \brief getNumberOfInputs

         Get number of inputbits.
         \return int Number of inputbits.
         */

        int getNumberOfInputs();

        /*!
         \brief getNumberOfOutputs

         Get number of outputbits.
         \return int Number of outputbits.
         */

        int getNumberOfOutputs();

        /*!
         \brief getResetState

         To get resetstate
         \return int ID of resetstate
         */

        int getResetState();

        /*!
         \brief getFsmName

         Get name of Fsm.
         \return string Name of Fsm.
         */

        string getFsmName();



        /**
         * Get the number of states in the fsm
         * @return
         */
        int getStateCount();

        /*!
         \brief getStatebyID

         If there is a State in the statevector with chosen ID it becomes current
         selected State.
         \param id ID of wanted %State
         \return The State or NULL if not found
         */

        State* getStatebyID(int id);



#define FOREACH_TRANSITIONS(fsm) \
     for (map<unsigned int,::Trans*>::iterator it = fsm->getTransitions().begin();it!=fsm->getTransitions().end();it++) { \
         ::Trans * transition = it->second;

#define END_FOREACH_TRANSITIONS }

        /**
         *
         * @return A reference to the transitions map
         */
        map<unsigned int,Trans*>& getTransitions();


        /**
         * Returns an array with pointers to all the transitions
         * This is useful for behaviours needing direct offset access
         * @return
         */
        Trans** getTransitionsArray();

        /*!
         \brief getTransbyID

         If there is a Transition in the transition-vector with chosen ID
         it becomes current selected Transition.
         \param id ID of wanted Transition.
         \return Trans * The transition if it exists, NULL otherwise
         */

        Trans * getTransbyID(unsigned int id);



#define FOREACH_LINKS(fsm) \
     for (map<unsigned int,::Link*>::iterator it = fsm->getLinks().begin();it!=fsm->getLinks().end();it++) { \
         ::Link * link = it->second;

#define END_FOREACH_LINKS }

        /**
         *
         * @return
         */
        map<unsigned int,Link*>& getLinks();

        /*!
         \brief getLinkbyID

         If there is a Link in the link-vector with chosen ID
         it becomes current selected Link.
         \param id ID of wanted Link.
         \return The searched lin, or NULL if does not exist
         */

        Link * getLinkbyID(unsigned int id);

        /**
         *
         * @param goal The id of the goal state
         * @return A pointer to the Link or NULL if not found
         */
        Link * getLinkbyGoal(State * state);




#define FOREACH_HYPERTRANSITIONS(fsm) \
     for (map<unsigned int,::Hypertrans*>::iterator it = fsm->getHypertransitions().begin();it!=fsm->getHypertransitions().end();it++) { \
         ::Hypertrans * hypertransition = it->second;

#define END_FOREACH_HYPERTRANSITIONS }

        /**
         *
         * @return
         */
        map<unsigned int,Hypertrans*>& getHypertransitions();


        /*!
         \brief getHypertransbyID

         If there is a Hypertransition in the hypertransition-vector with chosen ID
         it becomes current selected Hypertransition.
         \param id ID of wanted Hypertransition.
         \return The hyper trans or NULL if not found
         */
        Hypertrans * getHypertransbyID(unsigned int id);

        /*!
         \brief clockName

         To change clock-name..
         */

        string getClockName();

        /*!
         \brief resetName

         To change reset-name..
         */

        string getResetName();

        /**
         * Adds a new Join to the FSM
         * @return a new Join, with preseted id
         */
        Join * addJoin();

        /**
         * Adds an existing Join to the FSM
         * @return The provided join, with setted id if it is -1 (not defined)
         */
        Join * addJoin(Join *);

        /// Joins FOREACH Macro:
        ///    - Map<unsigned long int,Join*> iterator is accessible under the it variable
        ///    - Join is accessible under the join variable
        /// Don't forget to #include <map> !!
#define FOREACH_JOIN(fsm) \
       for (map<unsigned int,::Join*>::iterator it = fsm->getJoins().begin();it!=fsm->getJoins().end();it++) { \
           ::Join * join = it->second;

#define END_FOREACH_JOIN }

        /**
         *
         * @return The Joins map
         */
        map<unsigned int, Join*>& getJoins();

        /**
         * Get the join matching provided Id
         * @param id ID of the join to look for
         * @return The join or NULL if not found
         */
        Join * getJoin(unsigned int id);


        /** \defgroup Generator Parameters */
        /** @{ */

        /**
         * Set Path to last generated verilog source
         * @param path
         */
       /* void setLastGeneratedVerilogFile(string path);*/

        /**
         *
         * @return The path to last generated Verilog file
         */
        /*string getLastGeneratedVerilogFile();*/

        /**
         * Get a copy of the complete map of last generated verilog files
         * @return map of userId <-> Path of file
         */
        /*map<string, string> getLastGeneratedVerilogFileMap();*/

        /**
         * Add a path to the last generated files map
         *
         */
        /*void addLastGeneratedVerilogFile(string user, string path);*/


        /** @} */



};

#endif

