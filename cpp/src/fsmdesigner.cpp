/***********************************************************************
*                                                                      *
* (C) 2007, Frank Lemke, Computer Architecture Group,                  *
* University of Mannheim, Germany                                      *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the Free Software          *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
* USA                                                                  *
*                                                                      *
* For informations regarding this file contact                         *
*			      office@mufasa.informatik.uni-mannheim.de *
*                                                                      *
***********************************************************************/

// Includes
//--------------

//-- Qt
#include <QtGui>

//-- Core
#include <core/Core.h>
#include <core/Project.h>
#include <core/LoadFactory.h>

//-- Gui
#include <gui/FSMDesignerApplication.h>
#include <gui/mergedGUI/MergedMainWindow.h>


int main( int argc, char ** argv )
{
	// Prepare Application
	//--------------------------
    FSMDesignerApplication a( argc, argv );

    //-- Application Icon
    Q_INIT_RESOURCE(resources);
    QPixmap applicationIcon(":/icons/FSMDesignerlogosmall.png", 0, 0);
    QApplication::setWindowIcon(QIcon(applicationIcon));

    //-- Splash screen
    QPixmap logo(":/icons/FSMDesignerlogo.png");
    QSplashScreen* splashScreen = new QSplashScreen(logo);
    splashScreen->show();





    // Show GUI
    //------------------
    MergedMainWindow * window = new MergedMainWindow();
    window->show();

    //-- Close spashscreen
    splashScreen->finish(window);
    delete splashScreen;

    // Open Command line project or create default
    //-------------------------
    Core::getInstance()->reset();
    if (argc > 1) {

        QString projectFile = QString(argv[1]);
        LoadFactory::getLoad_I().load(projectFile.toStdString());

    } else {
        Core::getInstance()->getProject()->addFSM();
    }
    window->openProject();

    // Close application on last window closed
    //-----------------------------------
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();


}
