/**
 * IDManager.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//---------------

//-- Std
#include <fstream>

//-- Boost Test
#include <boost/test/unit_test.hpp>

//-- Qt
#include <QtCore>

//-- Core
#include <core/Project.h>
#include <core/Core.h>
#include <core-qxml/QXMLLoad.h>

//-- Gen Verilog
#include <genverilog/VerificationPlanGenerator.h>



// Structs for test
//-------------------------


struct PCIeXC6V {

    string path;

    PCIeXC6V() {}
    ~PCIeXC6V(){ }

    // Utilities
    //----------------
    string readFile(const char * path) {

        std::ifstream t(path);
        std::string str((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());
        return str;

    }

};




// Tests
//-------------------

BOOST_FIXTURE_TEST_SUITE( genvplan, PCIeXC6V)


    BOOST_AUTO_TEST_CASE( rma_responder )
    {

        //-- Parse FSM
        QXMLLoad load;
        load.load("./datasets/genverilog/rma_responder_fsms.pro.xml");

        //-- Prepare output path
        QFile verilogFile("output/responder_stage4_128_fsm.vplan");
        verilogFile.open(QFile::Text | QFile::WriteOnly | QIODevice::Truncate);

        //-- Generate
        QDataStream outputStream(&verilogFile);

        //-- Generate Verilog
        VerificationPlanGenerator * generator = new VerificationPlanGenerator();
        generator->generate(Core::getInstance()->getProject()->getFSMs().at(4),&outputStream);


        verilogFile.close();

    }


BOOST_AUTO_TEST_SUITE_END()


