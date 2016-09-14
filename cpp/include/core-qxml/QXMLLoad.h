/*
 * DOMLoad.h
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */

#ifndef QXMLLOAD_H_
#define QXMLLOAD_H_

// Includes
//------------

//-- STL
#include <list>
using namespace std;

//-- Qt
#include <QtXml>
#include <QtCore>

//-- Core
#include <core/Load_I.h>

class QXMLLoad : public Load_I {



public:
	QXMLLoad();
	virtual ~QXMLLoad();


	/**
	 * @deprecated Calls directly this#load
	 * @param filename
	 */
	virtual void loadProject(string filepath);

	/**
	 * Loads the provided file, and creates a new Project or add the provided single-fsm file to the project.
	 * The core will be reseted (Current Project deleted) before opening the new file
	 * @param filepath Path to the file to load
	 */
	virtual void load(string filepath);

	/**
	 * Utility method
	 * @param attribute Attribute name to get
	 * @param element Element on which to lookup
	 * @return the string value or empty string if nothing
	 */
	static const char * getAttributeValue(const char * attribute,QDomElement  element);
        string getAttributeValueString(const char * attribute,QDomElement element);

	static const  QDomElement getFirstMatchingChild(const char * childName,QDomElement  element);

	static QList<QDomElement> getChildElements(const char * childName,QDomElement  element);

	/**
	 * does element/childName/text()
	 * @param childName
	 * @param element
	 * @return
	 */
	static string getChildText(const char * childName,QDomElement  element);

protected:

	/**
	 * Parses an XML file into a Xerces DOM Document
	 * @param filename File to parse
	 * @return DOMDocument instance of NULL if something got wrong
	 */
	QDomDocument * parseFile(string filename);

	/**
	 * Parses an FSM from XML
	 * @param fsm The fsm DOm element
	 */
	void parseFSM(QDomElement  fsm) ;

	/**
	 * Converts the DOM document to the newest version
	 * Conversion provided at the moment:
	 *
	 * -> Link
	 *
	 * @param source
	 */
	void convert(QDomDocument * source);

};

#endif /* DOMLOAD_H_ */
