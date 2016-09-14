/**
 * TransitionBase.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef TRANSITIONBASE_H_
#define TRANSITIONBASE_H_

// Includes
//-----------------

//-- Std
#include <vector>
#include <string>
using namespace std;

//-- Qt


//-- Core
#include <core/UniqueIDObject.h>
class Condition;
class Trackpoint;

/**
 * This class is a base class for objects that are like transitions.
 * It offers the methods for:
 *
 * - Name
 * - Conditions
 * - Trackpoint
 *
 */
class TransitionBase : public UniqueIDObject {

    protected:

        /// Name
        string name;

        /// Conditions
        vector<Condition*> conditions;

        /// The list of trackpoints
        vector<Trackpoint*>      trackpoints;

    public:
        TransitionBase();
        virtual ~TransitionBase();

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

        /** @} */

        /** \defgroup Conditions Conditions management */
        /** @{ */

        #define FOREACH_CONDITIONS(transitionBase) \
             for (vector<Condition*>::iterator it = transitionBase->getConditions().begin();it!=transitionBase->getConditions().end();it++) { \
                 ::Condition * condition = (*it);

        #define END_FOREACH_CONDITIONS }

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
        unsigned int deleteCondition(Condition * conditionToDelete);



        /** @} */

        /** \defgroup Trackpoints Trackpoints Utilities */
        /** @{ */

        #define FOREACH_TRACKPOINTS(transitionBase) \
             for (vector<Trackpoint*>::iterator it = transitionBase->getTrackpoints().begin();it!=transitionBase->getTrackpoints().end();it++) { \
                 ::Trackpoint* trackpoint = *it;

        #define END_FOREACH_TRACKPOINTS }


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

#endif /* TRANSITIONBASE_H_ */
