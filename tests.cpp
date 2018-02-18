#define BOOST_TEST_MODULE Fatigue_Test_Suite
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(sanity_check) {
    auto const i = 1;

    BOOST_CHECK_EQUAL(i, 1);
}
