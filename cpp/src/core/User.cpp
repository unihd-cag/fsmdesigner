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
#include <pwd.h>
using namespace std;

//-- Qt
#include <QtCore>

#ifdef WIN32
#include <lmcons.h>
#include <windows.h>
//#include <LMCONS.h>
#else

#endif

#include "User.h"
User::User() {
	// TODO Auto-generated constructor stub

}

User::~User() {
	// TODO Auto-generated destructor stub
}


string User::getUserID() {


    string username;

    //-- Get User name depending on system
    #ifdef WIN32


       //-- Get username under windows, not the uid
        #if defined(UNICODE)
            if ( qWinVersion() & Qt::WV_NT_based )
            {
                TCHAR winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
                DWORD winUserNameSize = sizeof(winUserName);
                GetUserName( winUserName, &winUserNameSize );
                username = qt_winQString( winUserName ).toStdString();
            } else
        #endif
            {
                char winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
                DWORD winUserNameSize = sizeof(winUserName);
                GetUserNameA( winUserName, &winUserNameSize );
                username = QString::fromLocal8Bit( winUserName ).toStdString();
            }

   #else

            //-- Get login name
            //std::string login = getlogin();
            //username.append(getlogin());

            struct passwd *pw;
            pw = getpwuid(getuid());
            username.append(pw->pw_name);

   #endif


	//-- Build string with system
	//string userIDString;
	stringstream userIDString;
	userIDString << User::getOS() <<":";
	userIDString << username;
	//userIDString.append("linux:");
	//userIDString.append()

	return userIDString.str();

}

string User::getOS() {

    #ifdef WIN32
        return string("windows");
    #else
        return string("linux");
    #endif

}
