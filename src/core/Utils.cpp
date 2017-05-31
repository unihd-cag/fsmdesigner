/*
 * Utils.cpp
 *
 *  Created on: May 17, 2011
 *      Author: rleys
 */


// Includes
//----------

//-- STD
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <list>
#include <iterator>

using namespace std;

#include "Utils.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}


string Utils::resolveEnvironmentVariables(string input,char ** environment) {

}

string Utils::insertEnvironementVariables(string input,char ** environment) {

	if (environment==NULL)
		return input;

	//-- Foreach variables
	string result = input;
	int resultLonger = 0; // The ENV variable that matches the biggest part of the string
	char ** env;

	for (env = environment;*env != NULL; env++) {

		//-- Get Env Variable Name
		char * envValue  = (char*) calloc(strlen(*env),sizeof(char));
		memcpy(envValue,*env,strlen(*env)*sizeof(char));
		char* envVariable = strtok(envValue,"=");

		//-- If in ignore, then continue
		list<string> ignores = Utils::getEnvironmentIgnores();
		list<string>::iterator it;
		bool ignore = false;
		for (it = ignores.begin();it!=ignores.end();it++) {
			if (*it == string(envVariable)) {
				ignore = true;
				break;
			}
		}

		if (ignore)
			continue;

		//-- Get Value of variable
	    char * value = getenv(envVariable);

	    //cout << "Env variable is: '" << envVariable << "' => '"<< value <<"'";
	    //cout << endl;

	    if (value!=NULL) {

	    	//cout << "ENV Var: "<< envVariable <<" of length " << strlen(value) << " for value: " << value<< endl;

	    	//-- Search value in input string
	    	size_t position = input.find(value);
	    	if (position!=input.npos) {

	    		//cout << "Trying with variable "<< envVariable <<" of length " << strlen(value) << endl;

	    	    // Only replace if replaced length is bigger than 4 chars
	    		if (strlen(value)> 4 && strlen(value)>resultLonger) {


					//-- Replace
					resultLonger = strlen(value);
					result = input;
					result.replace(position,resultLonger,string("${")+envVariable+string("}"));

					//cout << "\tReplaced with variable "<< envVariable <<" , input is now: " << input << endl;
	    		}

	    	}

	    }


	}

	return result;



}

list<string> Utils::getEnvironmentIgnores() {

	list<string> environmentInsertIgnores;
	environmentInsertIgnores.push_front(string("PWD"));
	environmentInsertIgnores.push_front(string("USER"));
	environmentInsertIgnores.push_front(string("USERNAME"));
	environmentInsertIgnores.push_front(string("LOGNAME"));
	environmentInsertIgnores.push_front(string("OLDPWD"));
	environmentInsertIgnores.push_front(string("HOME"));
	environmentInsertIgnores.push_front(string("SHLVL"));


	return environmentInsertIgnores;
}



string Utils::getMajorVersion() {
	return string(FSMDesigner_VERSION_MAJOR_STR);
}


string Utils::getMinorVersion() {
	return string(FSMDesigner_VERSION_MINOR_STR);
}

string Utils::itos(int i) {
    stringstream s;
    s << i;
    return s.str();
}
