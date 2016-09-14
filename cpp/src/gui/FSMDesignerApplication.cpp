/**
 * FSMDesignerApplication.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */



// Includes
//-------------------

//-- Std
#include <exception>
using namespace std;


//-- Qt
#include <QtCore>
#include <QtGui>


#include "FSMDesignerApplication.h"


FSMDesignerApplication::FSMDesignerApplication ( int & argc, char ** argv ) : QApplication(argc,argv) {


}

FSMDesignerApplication::~FSMDesignerApplication() {


}

 bool FSMDesignerApplication::notify ( QObject * receiver, QEvent * e ) {

     // Call Parent and catch std exceptions
     //--------------------
     try {
         // Call Parent
         bool result = QApplication::notify(receiver,e);
         return result;

     } catch (exception& e) {

         qDebug() << "Caught exception: " << e.what();

         // Show an error
         //-------------------
         QErrorMessage errorDialog(this->activeWindow());
         //errorDialog.showMessage(QString::fromStdString(e.what()));
        // errorDialog.exec();

     } catch (exception * e) {

         qDebug() << "Caught exception: " << e->what();

         // Show an error
         //-------------------
         QErrorMessage errorDialog(this->activeWindow());
         //errorDialog.showMessage(QString::fromStdString(e->what()));
         //errorDialog.exec();
     }

     return false;


 }
