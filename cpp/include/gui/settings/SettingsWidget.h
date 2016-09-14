/**
 * SettingsWidget.hpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef SETTINGSWIDGET_HPP_
#define SETTINGSWIDGET_HPP_

// Includes
//-----------------

//-- QT
#include <QtCore>
#include <QtGui>
#include <QtXml>

/**
 * The settings widget takes an XML setttings file as input and build automagically the Settings Setup UI
 */
class SettingsWidget : public QWidget {

    Q_OBJECT


    public:

        /**
         * Uses resourcePath as the path for a file to open and read XML from
         * @param resourcePath
         * @param parent
         */
        SettingsWidget(QString resourcePath,QWidget * parent = NULL);
        virtual ~SettingsWidget();

        /**
         * Init this Widget from a DOM Document contains a <Settings> description
         */
        void init(QDomDocument * document);

    protected:

        /**
         * Process a <Setting element> and creates a QWidget for it
         * @param setting
         * @param parent
         * @return The created QWidget
         */
        QWidget * placeSetting(QDomElement& setting,QWidget * parent);

    public Q_SLOTS:

        /** \defgroup ValueChange Setting Value Change methods */
        /** @{ */


        /** @} */



};



#endif /* SETTINGSWIDGET_HPP_ */
