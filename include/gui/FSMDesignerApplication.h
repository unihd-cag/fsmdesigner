

#ifndef FSMDesignerApplication_H
#define FSMDesignerApplication_H


// Includes
//-------------------

//-- Qt
#include <QtCore>
#include <QtGui>



class FSMDesignerApplication : public QApplication {



    public:

        /**
         *
         * @param argc
         * @param argv
         */
        FSMDesignerApplication ( int & argc, char ** argv );
        virtual ~FSMDesignerApplication();

        /**
         * Reimplemented to catch std::exception from core
         * @param receiver
         * @param e
         * @return
         */
        virtual bool    notify ( QObject * receiver, QEvent * e );

};


#endif
