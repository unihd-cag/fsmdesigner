/**
 * SimvisionMmapGenerator.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//------------------

//-- Std
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

//-- Qt
#include <QtCore>

//-- Core
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Hypertrans.h>
#include <core/Condition.h>

//-- Verification
#include <verification/verify.h>
#include <verification/logicmin.h>
#include <verification/invertDNF.h>

#include "SimvisionMmapGenerator.h"

SimvisionMmapGenerator::SimvisionMmapGenerator() {
    // TODO Auto-generated constructor stub

}

SimvisionMmapGenerator::~SimvisionMmapGenerator() {
    // TODO Auto-generated destructor stub
}

Generator * SimvisionMmapGenerator::newInstance() {
    return new SimvisionMmapGenerator();
}

QString SimvisionMmapGenerator::getName()  {

    return "Verilog Generator";
}


QString SimvisionMmapGenerator::getDescription()  {

    return "Verilog Generator from CAG";

}

void SimvisionMmapGenerator::generate(Fsm * fsm, QDataStream * dataStream) {

    //-- Open Target file
    QTextStream out (dataStream->device());

    //-- Prepare output and datas
    int outputsCount = fsm->getNumberOfOutputs();

    // Write Map out
    //---------------------

    //-- Map name
    out << "mmap new -name " << fsm->getFsmName().c_str() << " -contents {"
            << endl;

    //-- States
    FOREACH_STATE(fsm)

    //-- Write state
        QColor stateColor(state->getColor());
        out << "    { 'b" << state->getOutput().c_str() << " -label \""
                << state->getName().c_str() << "\"" << " -linecolor "
                << ((QVariant) stateColor).toString()
                << " -bgcolor "
                << ((QVariant) stateColor).toString() << "}"
                << endl;

    END_FOREACH_STATE

    //-- Close
    out << "}" << endl;

    //------------------//

}

