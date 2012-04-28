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
 \file link.h
 \brief Link

 \author Frank Lemke
 \date 2006-02-27
 */

#ifndef LINK_H_
#define LINK_H_

// Includes
//------------------------

//-- Std
#include <utility>
#include <string>
#include <cstdlib>

using namespace std;

//-- Core
#include <core/UniqueIDObject.h>
class State;

/*!
 \brief Link-object

 Link is a shortcut to a State. A Trackpoint can directly be connected
 to it.
 */

class Link : public UniqueIDObject {

    protected:

        State *                 targetState;
        pair<double, double>    position;
        int                     color;


    public:

        Link(State * targetState, double posx, double posy);
        virtual ~Link();

        /** \defgroup Properties Management */
        /** @{ */

        State * getTargetState();
        void setTargetState(State *);

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

        void setPosition(pair<double, double> position);

        /**
         * @warning Not a reference! Modifying the result will not modify the source
         * @return Position pair (first = x, second = y)
         */
        pair<double, double> getPosition();

        /** @} */

};

#endif
