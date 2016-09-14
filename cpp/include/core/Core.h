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
 \file core.h
 \brief Core

 \author Frank Lemke
 \date 2006-03-02
 */

#ifndef CORE_H_
#define CORE_H_

// Includes
//-------------------

//-- Std
#include <iterator>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

//-- Core
class Fsm;
class Project;

/*!
 \brief Core

 The core is the entry point to access the opened project
 If is a singleton
 */

class Core {

    private:

        /// The current Opened Project
        Project * project;

        /// Core singleton
        static Core  core;

        /// Default constructor
        /// Private for Sinleton's sake
        Core();

    public:

        /// Default destructor
        virtual ~Core();

        /**
         * Static Call for the singleton
         *
         * @return The pointer to the Core singleton instance
         * @warning Don't delete the core instance, it is part of the static memory space
         */
        static Core * getInstance();

        /**
         * Delete the Project in the Core
         */
        void closeProject();

        /**
         * The curently loaded project Model
         *
         * @return The Project - NULL if none loaded
         */
        Project * getProject();

        /**
         * Define where the project is stored
         * @param project The Project that is loaded
         */
        void setProject(Project *);

        /**
         * Says if there is at least one FSM available
         *
         *
         * @return true if there is a FSM, false if not
         */
        bool hasFsm();

        /**
         *
         * @return true if there is a project loaded, false if not
         */
        bool hasProject();

        /*!
         \brief reset

         Function to reset Core.
         */

        void reset();
};

#endif
