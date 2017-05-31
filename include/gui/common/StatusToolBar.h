/**
 * StatusToolBar.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef STATUSTOOLBAR_H_
#define STATUSTOOLBAR_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Common
class FAction;


/**
 * A toolbar showing the action names in a Label under the toolbar
 */
class StatusToolBar: public QWidget {

    Q_OBJECT

    protected:

        QToolBar * toolbar ;
        QLabel * statusLabel;

    public:
        StatusToolBar(QWidget * parent = NULL);
        virtual ~StatusToolBar();

        /**
         * Returns the toolbar to add actions
         * @return
         */
        QToolBar * getToolBar();


        /**
         * Adds an action to the toolbar, with the given text to display on hover
         * @param icon
         * @param statusLabel
         * @return
         */
        FAction * addAction(QIcon icon,QString statusLabel);


        /**
         * Adds the action using #addAction(QIcon,QString), and returns the underlying QToolButton, to allow creation of submenus and so on
         * @param icon
         * @param statusLabel
         * @return
         */
        QToolButton * addActionButton(QIcon icon,QString statusLabel);

    public slots:

        /**
         * Display toolbar hovered action in status label
         */
        void toolbarActionHovered(FAction *);

    protected:

        /**
         * The leave event is used to clear the status label
         * @param event
         */
        virtual void leaveEvent ( QEvent * event );


};

#endif /* STATUSTOOLBAR_H_ */
