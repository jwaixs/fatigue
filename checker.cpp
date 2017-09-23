#define BOOST_TEST_MODULE fatigue_test
#include <boost/test/included/unit_test.hpp>

#include "problem.h"

BOOST_AUTO_TEST_CASE(problem) {
    string question = "What's my name";
    string solution = "Noud";
    Problem p(question, solution);

    BOOST_CHECK_EQUAL(p.getProblem(), question);
    BOOST_CHECK_EQUAL(p.getSolution(), solution);
    BOOST_CHECK(p.isCorrect(solution));
    BOOST_CHECK(!p.isCorrect("Arnoud"));
}
