/**
 * SettingsWidget.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <sstream>
#include <exception>
#include <error.h>
#include <stdexcept>
using namespace std;

//-- QT
#include <QtCore>
#include <QtGui>
#include <QtXml>

//-- Core XML
#include <QXMLLoad.h>

//-- Common
#include <gui/common/ColorButton.h>

//-- Settings
#include <gui/settings/GuiSettings.h>
#include <gui/settings/SettingValueTransporter.h>

#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QString resourcePath,QWidget * parent) : QWidget(parent) {


    // Readin' XML
    //----------------------
    QDomDocument * document = new QDomDocument();

    //-- Read file
    QFile xmlFile(resourcePath);
    if (xmlFile.open(QIODevice::ReadOnly)) {

        //-- Parsing
        int errorLine;
        int errorColumn;
        QString errorMessage;
        if (!document->setContent(&xmlFile,true,&errorMessage,&errorLine,&errorColumn)) {
            // Prepare error
            stringstream buildedError;
            buildedError << "Document Parsing Failed at Line" << errorLine << "," << errorColumn << ":" << errorMessage.toStdString();
            throw runtime_error(buildedError.str());

        }

        //-- Build GUI
        this->init(document);
    }

}
SettingsWidget::~SettingsWidget() {



}

void SettingsWidget::init(QDomDocument * document) {

    // FIXME Clear
    //-----------

    //-- Layout per default using VBox
    QVBoxLayout * layout = new QVBoxLayout(this);
    //this->setLayout(layout);

    //-- Get Root
    QDomElement settings = document->documentElement();


    // Groups
    //------------------------
    QList<QDomElement> groups = QXMLLoad::getChildElements("Group",settings);
    while (!groups.isEmpty()) {

        // Get group
        QDomElement group = groups.takeFirst();

        //---- Add a Group Panel
        //-------------------
        QGroupBox *groupBox = new QGroupBox(tr(QXMLLoad::getAttributeValue("name",group)));
        layout->addWidget(groupBox);

        //---- Set Layout to VBox Too
        QVBoxLayout * groupLayout = new QVBoxLayout(groupBox);
        //groupBox->setLayout(layout);

        //---- Add Subgroups to process List
        //-------------------
        groups.append(QXMLLoad::getChildElements("Group",group));

        //---- Add Settings
        //-----------------------
        QList<QDomElement> settings = QXMLLoad::getChildElements("Setting",group);
        while (!settings.isEmpty()) {

            //  Get Setting
            QDomElement setting = settings.takeFirst();
            groupLayout->addWidget(this->placeSetting(setting,groupBox));
        }

    }


}



QWidget * SettingsWidget::placeSetting(QDomElement& setting,QWidget * parent) {

    //-- Prepare result
    QWidget * result = new QWidget(parent);
    QHBoxLayout * layout = new QHBoxLayout(result);

    //-- Determine settings path (ex: my.settings.path.for.a.setting.color)
    QString settingPath = setting.hasAttribute("base") ? setting.attribute("base") : "" ;
    QDomElement  parentElement = setting.parentNode().toElement();
    while (!parentElement.isNull()) {

        //-- Prepend parent/@base attribute if present
        QString base = parentElement.attribute("base","");
        settingPath = settingPath.prepend(base.size()>0 ? base+".":"");

        //-- Go to parent
        parentElement = parentElement.parentNode().toElement();
    }

    // Add Label
    //-----------------------
    QLabel * label = new QLabel(tr(QXMLLoad::getAttributeValue("name",setting)));
    layout->addWidget(label);
    label->setToolTip("Path: "+settingPath);

    // Add Field depending on type
    //------------------------
    QString type = QString(QXMLLoad::getAttributeValue("type",setting));
    QString hint = QString(QXMLLoad::getAttributeValue("hint",setting));

    //---- String
    //---------------------
    if (type == "string") {

        //-- Add Text Field
        QLineEdit * line = new QLineEdit(parent);
        layout->addWidget(line);

        //-- init
        line->setText(GuiSettings::value(settingPath).toString());

        //-- Connect to value setter
        SettingValueTransporter * transporter = new SettingValueTransporter(settingPath);
        this->connect(line,SIGNAL(textChanged(QString)),transporter,SLOT(setData(QString)));

    }
    //---- Boolean
    //-----------------------
    else  if (type == "bool" ) {

        //-- Add CheckBox
        QCheckBox * checkBox = new QCheckBox(parent);
        layout->addWidget(checkBox);

        //-- Init
        checkBox->setChecked(GuiSettings::value(settingPath,true).toBool());

        //-- Connect to value setter
        SettingValueTransporter * transporter = new SettingValueTransporter(settingPath);
        this->connect(checkBox,SIGNAL(stateChanged(int)),transporter,SLOT(setCheckedData(int)));

    }
    //---- Slider Integer
    //-----------------------------
    else  if (type == "integer" && hint.contains("slider") ) {

        //-- Add Slider
        QSlider * slider = new QSlider(Qt::Horizontal,parent);
        layout->addWidget(slider);

        //-- Init
        slider->setValue(GuiSettings::value(settingPath).toInt());

        //-- Set min value?
        QString min = QString(QXMLLoad::getAttributeValue("min",setting));
        if (min.size()>0) {
            slider->setMinimum(atoi(min.toStdString().c_str()));
        }

        //-- Set max value ?
        QString max = QString(QXMLLoad::getAttributeValue("max",setting));
        if (max.size()>0) {
            slider->setMaximum(atoi(max.toStdString().c_str()));
        }

        //-- Connect to value setter
        SettingValueTransporter * transporter = new SettingValueTransporter(settingPath);
        this->connect(slider,SIGNAL(valueChanged(int)),transporter,SLOT(setData(int)));

    }
    //---- Color
    //--------------------
    else  if (type == "color") {

        //-- Add Color Button
        ColorButton * colorButton = new ColorButton(QColor::fromRgb(GuiSettings::value(settingPath).toInt()),parent);

        layout->addWidget(colorButton);

        //-- Connect to value setter
        SettingValueTransporter * transporter = new SettingValueTransporter(settingPath);
        this->connect(colorButton,SIGNAL(colorChanged(QColor)),transporter,SLOT(setData(QColor)));
    }


    //-- Return
    return result;

}



