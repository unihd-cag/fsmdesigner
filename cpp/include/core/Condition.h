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
 \file condition.h
 \brief Condition

 \author Frank Lemke
 \date 2006-02-27
 */

#ifndef CONDITION_H_
#define CONDITION_H_

// Includes
//---------------

//-- Std
#include <string>
#include <cstdlib>
using namespace std;


/*!
 \brief Condition-object

 Condition-object with automatically created id to store name and input sequence of a condition. The input sequence consists of a string of 0,1 or x (don't care) values with length equal to the number of inputs of the Fsm.

 */

class Condition {

    protected:
        string name;
        string input;

    public:
        Condition(int numberOfInputs);
        virtual ~Condition();

        /** \defgroup Overlapping Overlapping check */
        /** @{ */

        /**
         * This method checks if the current condition is overlapping with the provided one
         *
         * The algorithm is following:
         *
         *  - Compare both inputs characters one to one (this[i] with comparing[i] )
         *  - As soon as a position shows a decisive difference (like this[i] == && and comparing[i]==0), it is not overlapping
         *  - If one of both input is 'x' or '-' or '?', the bit is not decisive
         *
         * @param comparingInput
         * @return true if overlapping, false if not
         */
        bool isOverlappingWith(string comparingInput);

        /** @} */

        /** \defgroup Properties Properties */
        /** @{ */

        string getName();
        void setName(string name);

        /**
         * Returns the Input string this condition matches
         * @return
         */
        string getInput();

        /**
         * Set the Input string this condition will be matching
         * @param input
         */
        void setInput(string input);


        /** @} */

        /** \defgroup Input manipulation */
        /** @{ */


        /**
         * Add an input bit at the end of the input vector
         * @param value '1' , '0' , '-'
         */
        void addInputBit(char value);

        /**
         * Return this condition's input bit at the given position
         * @param position
         * @return The input bit character ('0','1','-')
         * @throws invalid_argument if the given position is out of bound
         */
        char getInputBit(unsigned int position);

        /**
         * Modifies the input bit at the given position
         * @param position
         * @param value
         */
        void changeInputAt(unsigned int position , char value);

        /**
         * Deletes the input character at position
         * @param position
         */
        void deleteInputAt(unsigned int position);

        /** @} */

};

#endif
