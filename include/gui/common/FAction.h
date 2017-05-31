/**
 * \file FAction.h
 *
 * 
 *
 */

#ifndef FACTION_H_
#define FACTION_H_

// Includes
//--------------------

//-- Qt
#include <QtCore>
#include <QtGui>



/** Description Here
 *
 */
class FAction : public QAction {

    Q_OBJECT

    public:

        /**
         * Default constructor
         * @param text Will be set as tooltip if icon is provided
         */
        FAction(QIcon icon,QString text = "");

        /**
         * Text will be set as button text
         * @param text
         */
        FAction(QString text = "");

        /**
         * Default Destructor
         */
        virtual ~FAction();

    protected:

        /**
         * Signal hoverLeaver
         * @param e
         * @return
         */
        virtual bool event ( QEvent * e );

    public slots:

        /**
         * Propagate to hover with reference to self
         */
        void fetchHover();

    Q_SIGNALS:

        /**
         *
         */
        void hovered(FAction *);

        /**
         * Send when leaving the component
         * @param
         */
        void hoveredLeave(FAction *);




};

#endif /* UNDODELETELINKJOIN_H_ */
