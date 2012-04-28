/*
 * GuiSettings.cpp
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */

// Includes
//------------------

//-- Std
#include <iostream>
using namespace std;

//-- QT
#include <QtCore>

#include "GuiSettings.h"


// Singleton initialisation
//--------------------------------
GuiSettings GuiSettings::settings;



GuiSettings::GuiSettings() : QSettings("FSMDesigner","LSRA") {
	// TODO Auto-generated constructor stub
	//cout << "Construcing GUI Settings" << endl;
}

GuiSettings::~GuiSettings() {
	// TODO Auto-generated destructor stub
}

QObject * GuiSettings::getAsQObject() {

	//cout << "Returning instance from settings" << endl;
	//static GuiSettings instance;
	//return instance;

	return dynamic_cast<QObject*>(&settings);
}

/// Static
QVariant GuiSettings::value( const QString & key, const QVariant & defaultValue ) {
	return dynamic_cast<QSettings*>(&settings)->value(key,defaultValue);
}

void GuiSettings::setValue ( const QString & key, const QVariant & value ) {
	dynamic_cast<QSettings*>(&settings)->setValue (key,value);
}

/// Get Recent files
QStringList GuiSettings::getRecentFiles() {


	// Get Recent files
	QVariant recentFiles = GuiSettings::value(SETTINGS_GENERAL_RECENT_FILES,QVariant());

	// If nothing, return an empty list
	if (!recentFiles.isValid()) {
		QStringList defaultValue;
		return defaultValue;
	} else {

		// Convert list of variant to normal list
		return recentFiles.toStringList();

	}

}

/// Add a recent file, and deletes the older one if limit is overwhelmed
void GuiSettings::addRecentFile(QString path) {

	//-- Get Value
	QStringList recentFiles = GuiSettings::getRecentFiles();

	//-- Add
	if (!recentFiles.contains(path)) {
		recentFiles.append(path);
		GuiSettings::cleanRecentFiles();
	}


	//-- Resave
	GuiSettings::setValue(SETTINGS_GENERAL_RECENT_FILES,QVariant(QStringList(recentFiles)));
	return;


}

void GuiSettings::clearRecentFiles() {

	//-- Resave
	GuiSettings::setValue(SETTINGS_GENERAL_RECENT_FILES,QVariant(QStringList()));
	return;


}


void GuiSettings::cleanRecentFiles() {

	// FIXME have a "modified" marker to not update the list if nothing has been cleaned

	//-- Get List
	QStringList recentFiles = GuiSettings::getRecentFiles();

	//-- Remove duplicates
	recentFiles.removeDuplicates();

	//-- Remove all files that don'T exist anymore

	//-- Resave
	GuiSettings::setValue(SETTINGS_GENERAL_RECENT_FILES,QVariant(QStringList(recentFiles)));
	return;


}


void GuiSettings::setTabTop() {
	GuiSettings::setValue(SETTINGS_UI_TABPOS,QVariant("Top"));
	this->settingsChanged();
}
void GuiSettings::setTabRight() {
	GuiSettings::setValue(SETTINGS_UI_TABPOS,QVariant("Right"));
	this->settingsChanged();
}

void GuiSettings::setTabBottom() {
	qDebug("Tab positin changed");
	GuiSettings::setValue(SETTINGS_UI_TABPOS,QVariant("Bottom"));
	this->settingsChanged();
}
void GuiSettings::setTabLeft() {
	GuiSettings::setValue(SETTINGS_UI_TABPOS,QVariant("Left"));
	this->settingsChanged();
}

void GuiSettings::changeValue(QString path,QVariant& value) {

    GuiSettings::setValue(path,value);

}


