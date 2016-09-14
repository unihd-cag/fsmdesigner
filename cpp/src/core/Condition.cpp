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
//----------------

//-- Std
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <sstream>
using namespace std;

//-- Core
#include <core/Utils.h>

#include "Condition.h"

Condition::Condition(int numberOfInputs) {
  this->name = "";
  this->input.assign(numberOfInputs, '-');
  
}

Condition::~Condition() {

}


bool Condition::isOverlappingWith(string comparingInput) {


    string dontcare = "x-?"; // Posssible Don't care characters

    // Verify both inputs are of the same length
    //----------------
    if (this->input.length() != comparingInput.length()) {

        stringstream ss;
        ss << "Condition::isOverlappingWith needs inputs of the same length (local: " << this->input.length() << ", comparing: " << comparingInput.length();
        throw runtime_error(ss.str().c_str());

    }

    // Proceed
    //---------------
    for (int i = 0 ; i < this->input.length() ; i++) {

        //-- Get Characters
        char base = this->input[i];
        char compare = comparingInput[i];

        //-- If one is "Don't care" -> not decisive
        //-------------------
        if (dontcare.find(base)!=dontcare.npos || dontcare.find(compare,0)!=dontcare.npos ) {
            continue;
        }
        //-- If not don't care, and different -> It is not overlapping
        //------------
        else if (base!=compare) {
            return false;
        }

    }

    //-- If we come up to here, not decisive bit has been found -> overlapping
    return true;


}


string Condition::getName() {
    return this->name;
}
void Condition::setName(string name) {
    this->name = name;
}

/**
 * Returns the Input string this condition matches
 * @return
 */
string Condition::getInput() {
    return this->input;
}

/**
 * Set the Input string this condition will be matching
 * @param input
 */
void Condition::setInput(string input) {
    this->input = input;
}


void Condition::addInputBit(char value) {

    //-- Check value
    if (value!='1' and value != '0' and value != '-') {
        throw invalid_argument(string("Cannot add and input bit with default value: ")+value+" (allowed: '1', '0', '-')");
    }

    //-- Add it
    this->input += value;

    cout << "Input vector is now of size: " << this->input.size() << endl;

}

char Condition::getInputBit(unsigned int position) {

    //-- Check validity
    if (position >= this->input.size())
        throw invalid_argument(string("Cannot modify condition bit: Out of bounds(size: ")+Utils::itos((this->input.size())));

    return this->input[position];
}

void Condition::changeInputAt(unsigned int position , char value) {

    cout << "Change Input vector of size: " << this->input.size() << endl;

    //-- Check validity
    if (position >= this->input.size())
        throw invalid_argument(string("Cannot modify condition bit: Out of bounds(size: ")+Utils::itos((this->input.size())));

    //-- Change
    this->input[position] = value;
}


void Condition::deleteInputAt(unsigned int position) {

    //-- Check validity
    if (position >= this->input.size())
        throw invalid_argument("Cannot delete condition bit: Out of bounds");

    //-- Erase
    this->input.erase(position,1);
}
