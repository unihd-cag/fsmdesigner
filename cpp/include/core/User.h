/*
 * User.h
 *
 * User stores informations and provides methods to access  the current user of FSM designer informations
 *
 *  Created on: Mar 10, 2011
 *      Author: rleys
 */

#ifndef USER_H_
#define USER_H_

// Includes
//-------------

//-- Std
#include <cstdlib>
#include <string>
using namespace std;


class User {
public:
	User();
	virtual ~User();

	/**
	 * Get the user ID
	 */
	static string getUserID();

	/**
	 * Get the OS the user is on
	 * @return string with OS name e.g linux,windows etc...
	 */
	static string getOS();

};



#endif /* USER_H_ */
