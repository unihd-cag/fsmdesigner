/**
 * TableVerificationListener.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef TABLEVERIFICATIONLISTENER_H_
#define TABLEVERIFICATIONLISTENER_H_

// Includes
//-----------------

//-- Qt
#include <QtCore>
#include <QtGui>

//-- Verification
#include <verification/VerificationListener.h>

/**
 * This Widget is a Table showing verification results
 */
class TableVerificationListener : public QTreeView , public VerificationListener {

    protected:

        QStandardItemModel model;

    public:
        TableVerificationListener(QWidget* parent = NULL);

        virtual ~TableVerificationListener();


        /** \defgroup Verification VerificationListener Methods */
        /** @{ */

        virtual void enteredRule(VerificatorRule * rule);

        virtual void ruleSuccessful(VerificatorRule * rule);

        virtual void ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors);

        /** @} */
};

#endif /* TABLEVERIFICATIONLISTENER_H_ */
