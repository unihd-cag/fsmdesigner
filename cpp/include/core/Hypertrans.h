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
class Condition;
class State;
class Trackpoint;



/**
 * @memory Managed by the owner FSM
 */
class Hypertrans : public UniqueIDObject {

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

        /// Conditions
        vector<Condition*> conditions;

        /// The list of trackpoints
        vector<Trackpoint*>      tl;

    public:
        Hypertrans();
        virtual ~Hypertrans();

        /** \defgroup Properties Properties management */
        /** @{ */


        /**
         * Return this name of this Hypertrans
         * @return
         */
        string getName();

        /**
         * Set the name of this hypertrans
         * @param name
         */
        void setName(string name);

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


        /** Returns a conditions vector
         *
         *
         */
        vector<Condition*>& getConditions();

        /**
         * Get the Condition at the id position
         * @param id
         * @return
         */
        Condition* getConditionByID(unsigned int id);

        /*!
         \brief addCondition

         Function to add Condition to Hypertrans.
         \param number Number of inputbits.

         @return The added condition
         */
        Condition * addCondition(int numberOfInputBits);

        /*!
         \brief deleteCondition

         Delete Condition by ID.
         \param id ID to delete associated %Condition.
         */

        void deleteCondition(unsigned int id);

        /**
         * Delete provided condition at pointer
         * @param condition
         * @return The index of the deleted condition
         * @throw invalid_argument if the condition pointer was not found in conditions list
         */
        unsigned int deleteCondition(Condition * conditionToDelete);



        /** @} */

        /** \defgroup Trackpoints Trackpoints Utilities */
        /** @{ */

        #define FOREACH_HYPERTRANSITION_TRACKPOINTS(hyperTransition) \
         for (vector<Trackpoint*>::iterator it = hyperTransition->getTrackpoints().begin();it!=hyperTransition->getTrackpoints().end();it++) { \
             ::Trackpoint* trackpoint = *it;

        #define END_FOREACH_HYPERTRANSITION_TRACKPOINTS }

        /**
         * Returns trackpoints list
         * @return A reference to the list of trackpoint, to be able to modify
         */
        vector<Trackpoint*>& getTrackpoints();


        /**
         * Add a trackpoint at the given position, first in the list
         * @param posx
         * @param posy
         */
        Trackpoint *addFirstTrackpoint(double posx, double posy);

        /**
         * Add a trackpoint as last in the list
         * @param trackpoint
         * @return the newly added trackpoint
         */
        Trackpoint *appendTrackpoint(double posx, double posy);

        /**
         * Add an existing trackpoint as last in the list
         * @param The trackpoint
         * @return the newly added trackpoint
         */
        Trackpoint *appendTrackpoint(Trackpoint*);

        /**
         * Add a trackpoint in the list after the specified
         * @param posx
         * @param posy
         * @param
         * @return the newly added trackpoint
         */
        Trackpoint *addTrackpointBefore(Trackpoint*, double posx, double posy);

        /**
         * Add a trackpoint in the list after the specified
         *
         * @param baseTrackpoint The Trackpoint to insert before
         * @param newTrackpoint The new Trackpoint
         * @return
         */
        Trackpoint *addTrackpointBefore(Trackpoint *baseTrackpoint, Trackpoint *newTrackpoint);

        /*!
         \brief deleteTrackpoint

         Delete Trackpoint by ID.
         \param id ID to delete associated %Trackpoint.
         @return The Trackpoint right after the deleted one, or NULL if none
         */
        Trackpoint *deleteTrackpoint(unsigned int id);
        /**
         * Removes the provided Trackpoint from this Trans.
         * @warning Does not delete the object!!
         * @param trackpoint
         * @return The Trackpoint right after the deleted one, or NULL if none
         */
        Trackpoint *deleteTrackpoint(Trackpoint *trackpoint);

        /** @} */


};

#endif
