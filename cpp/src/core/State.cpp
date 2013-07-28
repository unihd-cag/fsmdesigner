/***********************************************************************
*                                                                      *
* (C) 2006, Frank Lemke, Computer Architecture Group,                  *
* University of Mannheim, Germany                                      *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                            *
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
//------------------

//-- Std
#include <cstdlib>
#include <sstream>
#include <map>
#include <iostream>
using namespace std;

//-- Core
#include <core/Utils.h>
#include <math.h>

#include "State.h"

State::State(int numberOfOutputs)  : UniqueIDObject() {
  this->position.first = 10;
  this->position.second = 10;
  this->hlevel = 0;
  this->name = "State";
  this->output = "";
  this->output.assign(numberOfOutputs, '0');
  this->color = 4294901760;
  this->reset = false;
}

State::~State() {

}


void State::setName(string name) {

    this->name.clear();
    this->name.append(name);

}

string State::getName() {
    return this->name;
}

void State::setPosition(pair<double, double> position) {
    this->position = position;
}


pair<double, double> State::getPosition() {
    return this->position;
}

bool State::isReset() {
        return this->reset;
}

void State::setReset(bool reset) {
    this->reset = reset;
}

string State::getOutput() {
    return this->output;
}

string State::getOutputHamming() {
    return this->outputHamming;
}

void State::setHammingOutput() {

	string output = this->output;

	int i,j,l,index;
	int n, k;
	int code[1024];
	int red[1024], info[1024];
	int m; // Number of parity bits
	int parity[10];
	int test, result;
	int zeropadding;

	if (output.size() == 1) {
		m = 2;
		zeropadding = 0;
	} else if (output.size() < 4) {
		m = 3;
		zeropadding = 4 - output.size();
	} else if (output.size() < 11) {
		m = 4;
		zeropadding = 11 - output.size();
	} else if (output.size() < 26) {
		m = 5;
		zeropadding = 26 - output.size();
	} else if (output.size() <= 57) {
		m = 6;
		zeropadding = 57 - output.size();
	} else {
    	cout << "Error: No possible count of bits";
    	this->outputHamming = "";
    	exit(0);
	}

	for(int z=1; z <= zeropadding; z++) {
		output.append("0");
	}

    n = pow(2,m)-1;
    k = n - m;

    // Compute parity positions
    parity[1] = 1;
    for (i=2; i<=m; i++)
      parity[i] = (parity[i-1]<<1) & 0xfffffffe;

    cout << "parity positions: ";
    for (i=1; i<=m; i++)
    	cout << parity[i] << " ";
    cout << endl;

    for (i = 1; i <= output.size(); i++)
    {
    	info[i] = output[i - 1] - '0';
    }

    cout << "information bits = ";

    for (j=1; j<=k; j++)
    	cout << info[j];

    cout << endl;

    // Compute parity bits
    for (j=1; j<=m; j++)
    {
      red[j] = 0;
      l = 0;
      for (i=1; i<=n; i++)
      {
        // Check that "i" is not a parity position = not a power of 2
        result = 0;
        test = 1;
        for (index=1; index<=m; index++)
        {
          if (i==test) result = 1;
          test *= 2;
        }
        if (!result)
        {
          l++;
          if ( (i>>(j-1)) & 0x01 )
            red[j] ^= info[l];
        }
      }
    }

    cout << "parity bits = ";
    for (j=1; j<=m; j++)
    	cout << red[j];

    cout << endl;

    // Transmit codeword
    i = 1;
    l = 1;
    for (j=1; j<=n; j++)
      if (j==parity[l] && l<=m)
      {
        code[j] = red[l]; l++;
      }
      else
      {
        code[j] = info[i]; i++;
      }


    ostringstream oss("");

    cout << "codeword = ";
    for (j=1; j<=n; j++) {
    	cout << code[j];
    	oss << code[j];
    }

    this->outputHamming = oss.str();

    cout << endl;
}

void State::setOutput(string output) {
    this->output = output;
}

void State::addOutput(unsigned int value) {

    if (value > 1) {
        throw invalid_argument(string("Trying to add and output to a signal with a value different than 0 or 1 :")+Utils::itos(value));
    }

    this->output = this->output.append(Utils::itos(value));
}

unsigned int State::getColor() {
    return this->color;
}

void State::setColor(unsigned int newColor) {
    this->color = newColor;
}



void State::setOutputAt(int outputIndex, char val) {

    //-- Check argument
    if (outputIndex >= this->output.length()) {
        throw new invalid_argument(
                "Provided output index exceeds output length in setOutputAt");
    }

    //-- Set
    this->output[outputIndex] = val;

}

char State::getOutputAt(int outputIndex) {

    //-- Check argument
    if (outputIndex >= this->output.length()) {
        throw new invalid_argument(
                "Provided output index exceeds output length in getOutputAt ");
    }

    //-- Return
    return this->output[outputIndex];

}

list<Trans*>& State::getStartingTransitions() {
    return this->startingTransitions;
}


void State::addStartingTransition(Trans * transition) {
    this->startingTransitions.push_back(transition);
}

void State::removeStartingTransition(Trans * transition) {
    this->startingTransitions.remove(transition);
}
