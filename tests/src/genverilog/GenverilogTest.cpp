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

//-- Core
#include <core/Project.h>
#include <core/Core.h>
#include <core-qxml/QXMLLoad.h>

//-- Gen Verilog
#include <genverilog/generationofverilog.h>




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

BOOST_FIXTURE_TEST_SUITE( genverilog, PCIeXC6V)

    BOOST_AUTO_TEST_CASE( simple_generate )
    {

        //-- Parse FSM
        QXMLLoad load;
        load.load("./datasets/genverilog/pcie_bridge_xc6v_128.pro.xml");


        //-- Generate Verilog
        GenerationOfVerilog vlogGenerator(Core::getInstance()->getProject()->getFSMs().first(),
                    NULL,true,true,"output/pcie_bridge_xc6v_128.rx.v","output");

        vlogGenerator.generateVerilog("output/pcie_bridge_xc6v_128.rx.v");


    }

BOOST_AUTO_TEST_SUITE_END()


