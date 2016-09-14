/***********************************************************************
 *                                                                      *
 * (C) 2008, Frank Lemke, Computer Architecture Group,                  *
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
 *                 office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/

/*!
 \file invertDNF.h
 \brief InvertDNF

 \author Frank Lemke
 \date 2008-01-30
 */

#ifndef INVERTDNF_H_
#define INVERTDNF_H_

// Includes
//-----------------

//-- Std
#include <cstdlib>
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//-- Logic min
#include "logicmin.h"

class InvertDNF {

    private:

        vector<string> restTerms;
        vector<string> complementDNF;
        vector<string> currentResult;
        string mainCTerm;
        string currentCALCTerm;
        int numberOfInputterms;
        int currentPosition;
        int len;
        int currentBitsSet;
        int cnt;
        int currentMaxNumber;
        string result;
        string curres;
        char help;
        char helpvalue;
        bool intersection;
        bool overlap;
        bool overlapall;

        /// true: try to remove all intersections, false: don't
        bool removeIntersections;

        void minimizeComplementDNF();

    public:

        /**
         * Default constructor : Don't try to remove minimized vector intersections (because it's very time expensive)
         * @param doFullIntersection
         */
        InvertDNF(bool removeIntersections = false);
        virtual ~InvertDNF();

        /*!
         \brief invert

         Function to invert a Disjunctiv Normal Form(DNF).

         \param inputDNF Conjunction terms of a DNF.
         \return string  The compliment DNF of the input DNF,
         conjunction terms are separated with +.
         */

        string invert(vector<string> inputDNF);

};

#endif
