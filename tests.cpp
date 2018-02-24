#define BOOST_TEST_MODULE Fatigue_Test_Suite
#include <boost/test/included/unit_test.hpp>

#include <string>

#include "speed_problem.h"
#include "speed_quiz.h"

BOOST_AUTO_TEST_CASE(sanity_check) {
    auto const i = 1;

    BOOST_CHECK_EQUAL(i, 1);
}

BOOST_AUTO_TEST_CASE(speed_problem) {
    std::string const problem = "problem";
    std::string const solution = "solution";

    SpeedProblem sp(problem, solution);

    BOOST_CHECK_MESSAGE(sp.getProblem() == problem,
            "Speed problem should have getProblem().");
    BOOST_CHECK_MESSAGE(sp.getSolution() == solution,
            "Speed problem should have getSolution().");
    BOOST_CHECK_MESSAGE(sp.getNumberOfTries() == 1,
            "Speed problem initial number of tries should be one.");
    BOOST_CHECK_MESSAGE(sp.getTimeToSolve() == 0.0,
            "Speed problem initial time to solve is zero.");

    BOOST_CHECK_MESSAGE(!sp.isCorrect(problem),
            "Speed problem should not have correct solution " + problem + ".");
    auto const t1 = 37.42;
    BOOST_CHECK_NO_THROW(sp.addTime(t1));

    BOOST_CHECK_MESSAGE(sp.isCorrect(solution),
            "Speed problem should have correct solution " + solution + ".");
    auto const t2 = 0.141592;
    BOOST_CHECK_NO_THROW(sp.addTime(t2));

    BOOST_CHECK_MESSAGE(sp.getNumberOfTries() == 2,
            "Speed problem has been tried twice.");

    BOOST_CHECK_SMALL(sp.getTimeToSolve() - t1 - t2, 0.00001);
}
