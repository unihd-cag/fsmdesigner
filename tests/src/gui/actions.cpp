/**
 * actions.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */
// Includes
//---------------

//-- Core
#include <core/State.h>

//-- Gui
#include <gui/items/StateItem.h>

//-- Boost Test
#include <boost/test/unit_test.hpp>


// State Fixture
//----------------------
struct StateFixture {


        State state;
        StateItem stateItem;

        StateFixture() {

            stateItem->setModel(state);
        }
        ~StateFixture() {

        }

};


// Tests
//---------------------------
BOOST_FIXTURE_TEST_SUITE( state_actions, StateFixture)


BOOST_AUTO_TEST_SUITE_END()


