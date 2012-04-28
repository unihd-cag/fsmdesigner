/*
 * User.cpp
 *
 *  Created on: Mar 10, 2011
 *      Author: rleys
 */

// Includes
//------------------

//-- Stdlib
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <sstream>
using namespace std;

#include "User.h"
User::User() {
	// TODO Auto-generated constructor stub

}

User::~User() {
	// TODO Auto-generated destructor stub
}


string User::getUserID() {

	//-- Get UID
	uid_t userID = getuid();

	//-- Build string with system
	//string userIDString;
	stringstream userIDString;
	userIDString << User::getOS() <<":";
	userIDString << userID;
	//userIDString.append("linux:");
	//userIDString.append()

	return userIDString.str();

}

string User::getOS() {
	return string("linux");
}
