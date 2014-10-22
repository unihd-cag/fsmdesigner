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
 \file state.h
 \brief State

 \author Frank Lemke
 \date 2006-02-28
 */

#ifndef STATE_H_
#define STATE_H_

// Includes
//-------------

//-- Std
#include <utility>
#include <string>
#include <cstdlib>
#include <vector>
#include <exception>
#include <stdexcept>
#include <list>
using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
class Trans;

/*!
 \brief State-object

 The State-object stores all informations about a FSM-state.
 Consists of internal ID, statename, hierarchylevel, position, color
 and outputvalues.
 */

class State: public UniqueIDObject {

    protected:

        /// Name of the state
        string name;

        /// Position
        pair<double, double> position;

        /// Hierarchical level
        int hlevel;

        /// Color
        unsigned int color;

        /// The output string
        string output;

        /// Is this state the reset sate? (default is false)
        bool reset;

        /// Transitions starting from this state
        list<Trans*> startingTransitions;

    public:

        State(int numberOfOutputs);
        virtual ~State();


        /** \defgroup Properties Set/Getters */
        /** @{ */

        /**
         * Set Name of the state
         * @param name
         */
        void setName(string name);

        /**
         * Returns the name of the state
         * @return
         */
        string getName();

        /**
         * Sets the position of the graphical element
         * @param position
         */
        void setPosition(pair<double, double> position);

        /**
         * Returns the Graphical position of this element
         * @return
         */
        pair<double, double> getPosition();

        /**
         * Is the current state defined as reset ?
         * @warning FSM verification checks there is only one reset state in the FSM
         * @return
         */
        bool isReset();

        /**
         * Defines this state as reset state
         * @warning FSM verification checks there is only one reset state in the FSM
         * @param reset
         */
        void setReset(bool reset);

        /**
         * String representing the output of this state
         * @return
         */
        string getOutput();

        /**
         *
         * @param output
         */
        void setOutput(string output);

        /**
         * Add a new output bit at the end of the vector
         *
         * @param value 0 or 1
         */
        void addOutput(unsigned int value);


        unsigned int getColor();

        void setColor(unsigned int newColor);

        /** @} */

        /**
         * Modify the output string to set at a specified position
         * @param outputIndex
         * @param val
         */
        void setOutputAt(int outputIndex, char val);

        /**
         * Returns the output character at a specified position
         * @param outputIndex
         * @return
         */
        char getOutputAt(int outputIndex);

#define FOREACH_STATE_STARTING_TRANSITIONS(state) \
     for (list<Trans*>::iterator it = state->getStartingTransitions().begin();it!=state->getStartingTransitions().end();it++) { \
         ::Trans* transition = *it;

#define END_FOREACH_STATE_STARTING_TRANSITIONS }


        /**
         *
         * @return
         */
        list<Trans*>& getStartingTransitions();

        /**
         * Add the provided transition to the list of starting transitions
         * @param transition
         */
        void addStartingTransition(Trans * transition);

        /**
         * Remove the provided transition from the list of starting transitions
         * @param transition
         */
        void removeStartingTransition(Trans * transition);

        /** 
         * @return the default transition from this state, or NULL if none has been defined 
         */
        Trans* getDefaultTransition();

};

#endif
