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


//-- Core
#include <core/IDManager.h>
#include <core/UniqueIDObject.h>

//-- Boost Test
#include <boost/test/unit_test.hpp>



// Structs for test
//-------------------------

class FakeObject : public UniqueIDObject {

    public:
        FakeObject() : UniqueIDObject() {

        }

        ~FakeObject() {

        }

};

struct F {
    F() {}
    ~F()         { }

    FakeObject testObject;
    IDManager idManager;
};




// Tests
//-------------------

BOOST_FIXTURE_TEST_SUITE( unique_id_object_tests, F)

    BOOST_AUTO_TEST_CASE( id_is_set )
    {
        //-- Not set
        BOOST_CHECK_EQUAL(testObject.isIdSet(),false);

        //-- Set
        testObject.setId(1);

        //-- Should be set
        BOOST_CHECK_EQUAL(testObject.isIdSet(),true);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE( id_manager_tests, F)


    BOOST_AUTO_TEST_CASE( add )
    {
        //-- Add To ID manager
        idManager.assignID(&(testObject));
        BOOST_CHECK_EQUAL(testObject.getId(),1);

    }

    BOOST_AUTO_TEST_CASE( add_and_readd )
    {
        //-- Add To ID manager
        idManager.assignID(&(testObject));

        //-- Readd to ID Manager, there should be an exception
        BOOST_TEST_MESSAGE("Fixture test object has id: " << testObject.getId());
        BOOST_CHECK_THROW(idManager.assignID(&(testObject)), invalid_argument);
    }

    BOOST_AUTO_TEST_CASE( add_dereference_and_add )
    {

        //-- Add To ID manager
        idManager.assignID(&(testObject));

        //-- Dereference
        idManager.derefenceObject(&(testObject));
        BOOST_CHECK_EQUAL(testObject.isIdSet(),false);

        //-- Add (id should be 1 again)
        idManager.assignID(&(testObject));
        BOOST_CHECK_EQUAL(testObject.getId(),1);
    }


    BOOST_AUTO_TEST_CASE( add_with_custom_id )
    {

        //-- Set Custom ID
        testObject.setId(256);

        //-- Add (id should still be 256)
        idManager.assignID(&(testObject));
        BOOST_CHECK_EQUAL(testObject.getId(),256);

    }



BOOST_AUTO_TEST_SUITE_END()
