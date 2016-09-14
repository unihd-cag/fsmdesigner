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
#include <core/Fsm.h>
#include <core/Core.h>
#include <core-qxml/QXMLLoad.h>



// Tests
//-------------------

BOOST_FIXTURE_TEST_SUITE( core_fsm, Fsm)

    BOOST_AUTO_TEST_CASE( tools_parameters_set )
    {

        //-- Set a parameter for two user ids
        this->setParameter("user1","parameter1","value1");
        this->setParameter("user2","parameter1","value2");

        //-- Verify parameters presence and values
        BOOST_CHECK_EQUAL(this->toolsParameters.count("user1"),1);
        BOOST_CHECK_EQUAL((*(this->toolsParameters["user1"]))["parameter1"],"value1");

        BOOST_CHECK_EQUAL(this->toolsParameters.count("user2"),1);
        BOOST_CHECK_EQUAL((*(this->toolsParameters["user2"]))["parameter1"],"value2");



    }

    BOOST_AUTO_TEST_CASE( tools_parameters_remove )
    {

        //-- Set a parameter for two user ids
        this->setParameter("user1","parameter1","value1");
        this->setParameter("user2","parameter1","value2");

        //-- Remove
        this->setParameter("user1","parameter1","");
        this->setParameter("user2","parameter1","");

        //-- Verify maps are empty
        BOOST_CHECK_EQUAL(this->toolsParameters.count("user1"),1);
        BOOST_CHECK_EQUAL(this->toolsParameters["user1"]->size(),0);
        BOOST_CHECK_EQUAL(this->toolsParameters.count("user2"),1);
        BOOST_CHECK_EQUAL(this->toolsParameters["user2"]->size(),0);

    }

BOOST_AUTO_TEST_SUITE_END()


