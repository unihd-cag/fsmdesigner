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
 \file trans.h
 \brief Trans

 \author Frank Lemke
 \date 2006-02-28
 */

#ifndef TRANS_H_
#define TRANS_H_

// Includes
//------------------

//-- Std
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <cstdlib>
#include <utility>
using namespace std;

//--Core
#include <core/UniqueIDObject.h>
#include <core/Condition.h>
class Trackpoint;
class State;

/*!
 \brief Trans-Object

 Object to store Transitions with all needed parameters.
 */

class Trans : public UniqueIDObject {

protected:



	/// The start state of this transition
	State *                 start;

	/// the end state of this transition
	State *                 end;

	/// The name of the transition
	string                  name;

	/// Is this transition marked default?
	bool                    defaultTransition;

	/// The color
    int                     color;

    /// The position of the text
    pair<double, double>    textpos;


	/// The conditions vector
	vector<Condition*>       cv;

	/// The list of trackpoints
	vector<Trackpoint*>      tl;


public:
	Trans(State * start, State * end);
	virtual ~Trans();


	/** \defgroup Properties Get/Setters */
	/** @{ */

	/**
	 *
	 * @return A reference to be able to modify
	 */
	pair<double,double>& getTextPosition();

	void setTextPosition(double x,double y);

	int getColor() ;
	void setColor(int color);

    string getName() ;
    void setName(string name);

    bool isDefault() ;
    void setDefault(bool d);


    /// Define the start State of this transition
    void setStartState(State*);

    /// Get the start state of this transition
    State *getStartState();

    /// Define the end State of this transition
    void setEndState(State*);

    /// Get the end state of this transition
    State *getEndState();


    /** @} */



#define FOREACH_TRANSITION_CONDITIONS(transition) \
     for (vector<Condition*>::iterator it = transition->getConditions().begin();it!=transition->getConditions().end();it++) { \
         ::Condition * condition = (*it);

#define END_FOREACH_TRANSITION_CONDITIONS }

    /**
     * Returns all the conditions that are set on this transition
     * @return A reference to the internal vector, to allow modification
     */
    vector<Condition*>& getConditions();

    /**
     * Get the Condition at the id position
     * @param id
     * @return
     */
    Condition * getConditionByID(unsigned int id);

    /*!
	 \brief addCondition

	 Function to add Condition to Trans.
	 \param numberOfInputBits Number of inputbits.

	 @return The added condition
	 */
    Condition * addCondition(int numberOfInputBits);

    /**
     * Add an existing condition object at the specified position
     * @param condition
     * @param position
     * @return The pointer provided as input
     * @throw invalid_argument if the condition pointer was NULL
     */
    Condition* addCondition(Condition * condition,unsigned int position);

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
    unsigned int deleteCondition(Condition * condition);


    /** \defgroup Trackpoints Trackpoints Utilities */
    /** @{ */

#define FOREACH_TRANSITION_TRACKPOINTS(transition) \
     for (vector<Trackpoint*>::iterator it = transition->getTrackpoints().begin();it!=transition->getTrackpoints().end();it++) { \
         ::Trackpoint* trackpoint = *it;

#define END_FOREACH_TRANSITION_TRACKPOINTS }

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
