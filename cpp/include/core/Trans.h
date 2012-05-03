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
#include <core/TransitionBase.h>
class Trackpoint;
class State;

/*!
 \brief Trans-Object

 Object to store Transitions with all needed parameters.
 */

class Trans :  public TransitionBase {

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

    /** \defgroup Conditions Conditions management */
    /** @{ */

#define FOREACH_TRANSITION_CONDITIONS(transition) \
     for (vector<Condition*>::iterator it = transition->getConditions().begin();it!=transition->getConditions().end();it++) { \
         ::Condition * condition = (*it);

#define END_FOREACH_TRANSITION_CONDITIONS }


    /** @} */

    /** \defgroup Trackpoints Trackpoints Utilities */
    /** @{ */

#define FOREACH_TRANSITION_TRACKPOINTS(transition) \
     for (vector<Trackpoint*>::iterator it = transition->getTrackpoints().begin();it!=transition->getTrackpoints().end();it++) { \
         ::Trackpoint* trackpoint = *it;

#define END_FOREACH_TRANSITION_TRACKPOINTS }


    /** @} */

};

#endif
