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
 \file hypertrans.h
 \brief Hypertrans

 \author Frank Lemke
 \date 2006-02-28
 */

#ifndef HYPERTRANS_H_
#define HYPERTRANS_H_

// Includes
//-----------------

//-- Std
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
#include <core/TransitionBase.h>
class Condition;
class State;
class Trackpoint;



/**
 * @memory Managed by the owner FSM
 */
class Hypertrans : public TransitionBase {

    public:

        /** Type of the Hypertransition
         *
         */
        enum TYPE {

            FromReset,   //!< Can be triggered only from the reset state
            FromAllStates//!< Can be triggered only from any state

        };


    protected:

        /// Name
        string name;

        /// Type
        Hypertrans::TYPE type;
        //bool c;

        /// Color
        int color;

        // Position
        pair<double, double> position;

        // State we are targeting at
        State * targetState;



    public:
        Hypertrans();
        virtual ~Hypertrans();

        /** \defgroup Properties Properties management */
        /** @{ */

        /**
         * Define color
         * @param color
         */
        void setColor(int color);

        /**
         * Return the actual set color
         * @return
         */
        int getColor();

        /**
         * @warning Not a reference! Modifying the result will not modify the source
         * @return Position pair (first = x, second = y)
         */
        pair<double, double> getPosition();

        void setPosition(pair<double, double> position);

        /** Get the state this hypertransition is pointing at
         *
         * @return
         */
        State * getTargetState();

        /** Set the state this hypertransition is pointing at
         *
         * @param
         */
        void setTargetState(State *);

        /**
         *
         * @return all_states is true, rest_states is false
         */
        Hypertrans::TYPE getType();

        /**
         *
         * @param type all_states is true, rest_states is false
         */
        void setType(Hypertrans::TYPE type);

        /** @} */

        /** \defgroup Conditions Conditions management */
        /** @{ */

#define FOREACH_HYPERTRANSITION_CONDITIONS(hypertransition) \
     for (vector<Condition*>::iterator it = hypertransition->getConditions().begin();it!=hypertransition->getConditions().end();it++) { \
         ::Condition* condition = *it;

#define END_FOREACH_HYPERTRANSITION_CONDITIONS }


        /** @} */

        /** \defgroup Trackpoints Trackpoints Utilities */
        /** @{ */

        #define FOREACH_HYPERTRANSITION_TRACKPOINTS(hyperTransition) \
         for (vector<Trackpoint*>::iterator it = hyperTransition->getTrackpoints().begin();it!=hyperTransition->getTrackpoints().end();it++) { \
             ::Trackpoint* trackpoint = *it;

        #define END_FOREACH_HYPERTRANSITION_TRACKPOINTS }


        /** @} */


};

#endif
