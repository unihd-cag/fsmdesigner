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


#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/Core.h>
#include <core/Project.h>
#include <core/LoadFactory.h>

//-- Generate
#include <generate/GeneratorFactory.h>
#include <genverilog/VerilogGenerator.h>
#include <genverilog/SimvisionMmapGenerator.h>

//-- Gui
#include <gui/FSMDesignerApplication.h>
#include <gui/mergedGUI/MergedMainWindow.h>

//-- Backtrace
#ifdef FDEBUG
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <ctime>

void handler(int sig) {
  void *buffer[20];
  size_t size;
  char **c_strings = NULL;
  // get void*'s for all entries on the stack
  size = backtrace(buffer, 10);
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(buffer, size, STDERR_FILENO);
  // try to print the stacktrace to a trace file
  c_strings = backtrace_symbols(buffer, size);
  if (c_strings != NULL) {
    //TODO: make the time code portable?
    time_t t = time(0);
    struct tm * now = localtime ( &t );
    int year  = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day   = now->tm_mday;
    int hour  = now->tm_hour + 1;
    int min   = now->tm_min + 1;
    std::string path = "/tmp/fsm_stack_dump_" + std::to_string(year) + "_" +
      std::to_string(month) + "_" + std::to_string(day) + "_h" +
      std::to_string(hour) + "_m" + std::to_string(min);
    std::ofstream dump_file;
    dump_file.open(path.c_str() );
    for (int i = 0; i < size; ++i)
      dump_file << c_strings[i] << std::endl;
    dump_file.close();
    fprintf(stderr, "Trace dump stored: %s\n", path.c_str() );
    free(c_strings);
  }
  exit(1);
}
#endif
//-- END Backtrace


int main( int argc, char ** argv )
{
    // -- Backtrace: install signal handler
#ifdef FDEBUG
    signal(SIGSEGV, handler);
#endif
    // -- END Backtrace
    // Prepare Application
    //--------------------------

    FSMDesignerApplication a( argc, argv );

    if (QApplication::style() && QApplication::style()->objectName() == "gtk+" && qApp->styleSheet().isEmpty())
        qApp->setStyleSheet("QLineEdit { background-color: white }");

    //-- Locale test
    QLocale lc = QLocale::system();


    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF8" ) );
    QTextCodec::setCodecForTr ( QTextCodec::codecForName ( "UTF8" ) );
    QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF8" ) );
    qDebug() << "System Locale: " << lc.name();
    qDebug() << "System encoding: "<< QTextCodec::codecForLocale()->name();

    //-- Application Icon
    Q_INIT_RESOURCE(resources);
    QPixmap applicationIcon(":/icons/FSMDesignerlogosmall.png", 0, 0);
    QApplication::setWindowIcon(QIcon(applicationIcon));

    //-- Splash screen
    QPixmap logo(":/icons/FSMDesignerlogo.png");
    QSplashScreen* splashScreen = new QSplashScreen(logo);
    splashScreen->show();


    // Init Generators
    //--------------------------
    GeneratorFactory::getInstance()->registerGenerator("Verilog",new VerilogGenerator());
    GeneratorFactory::getInstance()->registerGenerator("Simvision_Mmap",new SimvisionMmapGenerator());

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
