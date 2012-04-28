/***********************************************************************
*                                                                      *
* (C) 2007, Frank Lemke, Computer Architecture Group,                  *
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
  \file load_I.h
  \brief Load_I
  
  \author Frank Lemke
  \date 2006-04-26
*/

#ifndef LOAD_I_H_
#define LOAD_I_H_

#include <string>
#include <cstdlib>

using namespace std;

/*!
  \brief Load_Interface

  Interface which provies all functions to load used FSMs or 
  projects.

*/

class Load_I {

public:
	/// Load a project
	virtual void loadProject(string filename) = 0;

	/// Load a single FSM
	virtual void load(string filename) = 0;

};

#endif
