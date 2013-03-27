/**
 * VerilogGeneratorChecks.cpp
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
using namespace std;

//-- Qt
#include <QtCore>

//-- FSM
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>

//-- Genverilog stuff
#include <verification/verify.h>
#include <verification/logicmin.h>
#include <verification/invertDNF.h>

#include "VerilogGeneratorChecks.h"

VerilogGeneratorChecks::VerilogGeneratorChecks() {
    // TODO Auto-generated constructor stub

}

VerilogGeneratorChecks::~VerilogGeneratorChecks() {
    // TODO Auto-generated destructor stub
}

QString VerilogGeneratorChecks::getName() {

    return "Verilog Generator v1, Generated Default transitions' conditions not overlapping with transitions' conditions";
}

/**
 * 
 */
QList<RuleError*> VerilogGeneratorChecks::applyRule(Fsm * fsm) {

    QList<RuleError*> resultErrors;

    // Verify Generated Default transition's case outputs overlapping
    //----------------------------------


    // Foreach States' transitions and conditions
    //-----------------------------
    FOREACH_STATE(fsm)

        //-- Record all inputs of the non

        //-- Foreach State, gather all conditions of non default transition
        //------------------
        vector<string> inputs;
        FOREACH_STATE_STARTING_TRANSITIONS(state)

            //-- If we are in the default transition -> skip
            if (!transition->isDefault()) {

                //-- Foreach Conditions
                FOREACH_TRANSITION_CONDITIONS(transition)

                    // Normalize string: xX? -> -
                    QString result = QString::fromStdString(condition->getInput());
                    result = result.replace(QChar('x'),QChar('-'),Qt::CaseInsensitive);
                    result = result.replace(QChar('?'),QChar('-'),Qt::CaseInsensitive);
                    inputs.push_back(result.toStdString());
                END_FOREACH_TRANSITION_CONDITIONS
            }
        END_FOREACH_STATE_STARTING_TRANSITIONS


        //-- Calculate invert of gathered inputs, which are the default conditions, then check overlapping with non default transitions's conditions
        //----------
        if (inputs.size() > 0) {

            //---- Calculate invert
            InvertDNF INV(false);
            QString defaultvalue = QString::fromStdString(INV.invert(inputs));

            //---- Break default value to '+'
            defaultvalue = defaultvalue.replace(QChar('-'),QChar('x'));
            QStringList defaultValues = defaultvalue.split('+',QString::SkipEmptyParts);

            //---- Now Compare with all non default transitions
            //-----------------------
            FOREACH_STATE_STARTING_TRANSITIONS(state)

                // Skip default transitino
                if (transition->isDefault())
                        continue;


                //-- For Each of this transition's conditions, compare with all calculated default values
                //--
                int sourceConditionsCount = 0;
                FOREACH_TRANSITION_CONDITIONS(transition)

                    // Loop on calculated default values
                    for (QStringList::iterator it = defaultValues.begin();it!=defaultValues.end();it++) {

                          if(condition->isOverlappingWith((*it).toStdString())) {

                                //-- Create Error
                                RuleError * error = new RuleError();
                                resultErrors+=error;

                                //-- Target objects are 1st source condition, then target condition
                                error->addConcernedObject(condition,FSMDesigner::CONDITION);

                                //-- Message
                                stringstream message;
                                message << "State: "<< state->getName() <<", Transition: "<< transition->getName() <<" , Condition: "<< condition->getName() <<" ,with input: "<< condition->getInput() <<" overlaps with Calculated default transition's case input: "<< (*it).toStdString();
                                error->setMessage(QString::fromStdString(message.str()));


                            }  


                    }
                    
                END_FOREACH_TRANSITION_CONDITIONS

            END_FOREACH_STATE_STARTING_TRANSITIONS


        }

        

    END_FOREACH_STATE

    return resultErrors;

}
