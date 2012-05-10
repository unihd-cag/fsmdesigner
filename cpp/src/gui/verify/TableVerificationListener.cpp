/**
 * TableVerificationListener.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//------------------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Verification
#include <verification/VerificationListener.h>

#include "TableVerificationListener.h"

TableVerificationListener::TableVerificationListener(QWidget* parent) : QTreeView(parent) {

    // Prepare model
    //-----------------------
    this->model.setHorizontalHeaderItem(0,new  QStandardItem("Name"));
    this->model.setHorizontalHeaderItem(1,new  QStandardItem("Success"));
    this->model.setHorizontalHeaderItem(2,new  QStandardItem("Details"));
    this->setModel(&(this->model));

}

TableVerificationListener::~TableVerificationListener() {

}

void TableVerificationListener::enteredRule(VerificatorRule * rule) {

   // qDebug() << "Entered rule";

}

void TableVerificationListener::ruleSuccessful(VerificatorRule * rule) {

    //-- Create Columns
    QList<QStandardItem*> columns;

    columns.append(new QStandardItem(rule->getName()));
    columns.append(new QStandardItem("Yes"));
    columns.append(new QStandardItem(""));

    //-- Add Result Line
    this->model.appendRow(columns);

    //-- Resize Name
    this->resizeColumnToContents(0);


}

void TableVerificationListener::ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors) {

    // Add a new line
    //--------------------------
    for (int i = 0 ; i<errors.size() ; i++) {

        //--  Get Error
        RuleError * error = errors.at(i);

        //-- Create Columns
        QList<QStandardItem*> columns;

        columns.append(new QStandardItem(rule->getName()));
        columns.append(new QStandardItem("No"));
        columns.append(new QStandardItem(error->getMessage()));

        //-- Add Result Line
        this->model.appendRow(columns);

    }


    //-- Resize Name
    this->resizeColumnToContents(0);


}
