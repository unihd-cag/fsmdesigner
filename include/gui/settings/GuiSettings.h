/*
 * GuiSettings.h
 *
 *  Created on: Sep 30, 2010
 *      Author: rleys
 */

#ifndef GUISETTINGS_H_
#define GUISETTINGS_H_

// Includes
//-------------

//-- QT Includes
#include <QtGui>
#include <QtCore>

// Properties paths and default values
//----------------
#define SETTINGS_STATE_TEXT_BG "ui/StateTextBackgroundColor"
#define SETTINGS_STATE_TEXT_BG_DEFAULT Qt::yellow
#define SETTINGS_STATE_FUTURE	"ui/StateFuture"
#define SETTINGS_STATE_FUTURE_DEFAULT false


#define SETTINGS_GENERAL_RECENT_FILES "General.recentFiles"

#define SETTINGS_UI_TABPOS "UI.TabPosition"



class GuiSettings : public QSettings {

	Q_OBJECT

private:
	/// Settings singleton
	static GuiSettings  settings;

public:
	GuiSettings();
	virtual ~GuiSettings();

	static QObject * getAsQObject();

	/** \defgroup Static access */
	/**@{*/

	/// Static accessor to singleton value()
	static QVariant 	value( const QString & key, const QVariant & defaultValue = QVariant() ) ;
	static void 		setValue ( const QString & key, const QVariant & value );

	/// Get Recent files
	static QStringList 	getRecentFiles();

	/// Add a recent file, and deletes the older one if limit is overwhelmed
	static void 		addRecentFile(QString path);

	/**
	 * Clear the rencent Files list
	 */
	static void			clearRecentFiles();

	/**
	 * Clean The recent files list:
	 * 	- Remove duplicates
	 * 	- Remove files that don't exist anymore
	 */
	static void			cleanRecentFiles();

	/**@}*/

Q_SIGNALS:

	/// Triggered whenever a setting has been changed
	void settingsChanged();

public Q_SLOTS:

	/** \defgroup Settings Slots to change settings */
	/**@{*/


	void setTabTop();
	void setTabRight();
	void setTabBottom();
	void setTabLeft();

	void changeValue(QString path,QVariant& value);


	/**@}*/

};

#endif /* GUISETTINGS_H_ */
