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
  \file loadfactory.h
  \brief LoadFactory
  
  \author Frank Lemke
  \date 2006-04-26
*/

#ifndef LOADFACTORY_H_
#define LOADFACTORY_H_

#include <core/Load_I.h>
//#include <core-sax/xmlload.h>

/*!
  \brief LoadFactory
  
  FactoryClass to make the static Load-object available.
*/

class LoadFactory {
  //static Xmlload l;


 public:
  
	static Load_I * loader;

  /*!
    \brief getLoad_I

    Function to return Load_Interface for working with static Load-object.
  \return Load_I& Load_I reference.  
  */ 
  static Load_I& getLoad_I();
};

#endif
