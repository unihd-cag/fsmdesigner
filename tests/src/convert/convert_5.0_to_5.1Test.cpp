/**
 * convert_5.0_to_5.1.cpp
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
#include <core-qxml/QXMLLoad.h>



// Utilities
//----------------
string readFile(const char * path) {

    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;

}

void writeFile(const char * path,const char * data) {

    std::ofstream t(path);
    t << data;

    t.close();



}

// Tests
//-----------------
BOOST_AUTO_TEST_SUITE( convert_5_0_to_5_1 )

    BOOST_FIXTURE_TEST_CASE( load_and_convert ,QXMLLoad )
    {
        //-- Open 5.0 DomDocument
        QDomDocument * document_5_0 = this->parseFile("datasets/xmlconvert/pflashXL_prog_5.0.pro.xml");


        //-- Convert FSM
        this->convert(document_5_0);

        //-- Create String
        QString converted_result = document_5_0->toString(0);
        converted_result = converted_result.replace(QRegExp("\\s*"),"");

        //-- Compare output to converted one
        QString source_result = QString::fromStdString(readFile("datasets/xmlconvert/pflashXL_prog_5.1_converted.pro.xml"));
        source_result =  source_result.replace(QRegExp("\\s*"),"");

        //-- Write Out results
        writeFile("output/xmlconvert/pflashXL_prog_5.1_converted.pro.xml",document_5_0->toString(0).toStdString().c_str());

        BOOST_CHECK_EQUAL(converted_result.toStdString(),source_result.toStdString());
    }

    BOOST_AUTO_TEST_CASE( load_convert_and_save )
    {

    }

BOOST_AUTO_TEST_SUITE_END()

