/*
 * SGC.h
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

#ifndef SGC_H_
#define SGC_H_

// Includes
//--------------

//-- Qt
#include <QtGui>
#include <QtCore>

/**
 * SGC is a Synced Gargabe Collector
 *
 * Concept is easy:
 * 	- Request a delete on a pointer
 * 	- If we are locked in a secured  code area, wait until unlock is reached to perform delete
 * 	- If not, delete imediately
 *
 * 	SGC is only accessible as a Singleton
 *
 *
 */
class SGC {

    private:

        /// Singleton Reference
        static SGC * singleton;

        /// Singleton Mutex for init
        static QMutex singletonMutex;

        /// The list of pending deletes
        QList<QGraphicsItem*> pendingDeletes;

        /// Mutex assuring secure mode management
        QMutex secureAreaMutex;

        /// Private construtor
        SGC();

        /// Private destructor
        virtual ~SGC();

    public:

        /// Activate the GC, thus letting all delete requests be pending
        void activate();

        /**
         * Deactivates the GC by first performing all pending deletes
         * The GC is deactivated at the end, this means that all requestDelete calls coming during deleting operation will be
         * pended, and GC will try to avoid multiple deletes of the same object
         */
        void collect();

        /**
         * Requests to delete datas behind the given pointer
         * @param pDel
         */
        void requestDelete(QGraphicsItem * pDel);

        /**
         *  Public Singleton Accessor
         * @return
         */
        static SGC * getInstance() {

            //-- Sync
            SGC::singletonMutex.lock();

            //-- Test and create
            if (SGC::singleton == NULL) {
                SGC::singleton = new SGC();
            }

            //-- Unsync
            SGC::singletonMutex.unlock();

            //-- Return
            return SGC::singleton;

        }

};

#endif /* SGC_H_ */
