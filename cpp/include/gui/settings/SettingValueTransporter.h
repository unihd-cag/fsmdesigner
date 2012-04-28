/**
 * SettingValueTransporter.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef SETTINGVALUETRANSPORTER_H_
#define SETTINGVALUETRANSPORTER_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Settings
#include <gui/settings/GuiSettings.h>

/**
 * This class sets value in the GUI settings for a certain path depending on what kind of data has been passed to it
 */
class SettingValueTransporter : public QObject {

    Q_OBJECT

    protected:
        QString path;

        QVariant data;

    public:
        SettingValueTransporter(QString str) {
            this->path = str;
        }

        ~SettingValueTransporter() {

        }

        QString getPath() {
            return this->path;
        }

        QVariant getData() {
            return this->data;
        }

    public Q_SLOTS:

        /**
         * Sets a QString data
         */
        void setData(QString str) {
               this->data = str;

               GuiSettings::setValue(path,data);

        }

        /**
         * Sets an integer data
         */
        void setData(int i) {
            this->data = i;

            GuiSettings::setValue(path,data);

        }

        /**
         * Sets a color data
         */
        void setData(QColor color) {
            this->data = color.value();

            GuiSettings::setValue(path,data);

        }
};

#endif /* SETTINGVALUETRANSPORTER_H_ */
