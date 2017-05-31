/**
 * VPlanGenerator.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//------------------

//-- Qt
#include <QtCore>

//-- Core
#include <core/Fsm.h>
#include <core/State.h>
#include <core/Trans.h>
#include <core/Hypertrans.h>
#include <core/Condition.h>

#include "VPlanGenerator.h"

VPlanGenerator::VPlanGenerator() {
    // TODO Auto-generated constructor stub

}

VPlanGenerator::~VPlanGenerator() {
    // TODO Auto-generated destructor stub
}

VerilogGenerator * VerilogGenerator::newInstance() {

    return new VerilogGenerator();

}


void VerilogGenerator::generate(Fsm *,QDataStream * dataStream) {






}
