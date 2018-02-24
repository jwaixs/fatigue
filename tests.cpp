#define BOOST_TEST_MODULE Fatigue_Test_Suite
#include <boost/test/included/unit_test.hpp>

#include <string>

#include "speed_problem.h"
#include "memory_problem.h"

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

BOOST_AUTO_TEST_CASE(memory_problem) {
    std::string const problem = "12345";
    std::string const wrong_guess = "abcde";

    MemoryProblem mp(problem);

    BOOST_CHECK_MESSAGE(mp.getMemoryProblem() == problem,
            "Memory problem should properly working getMemoryProblem()");
    BOOST_CHECK_MESSAGE(mp.correctNumber("") == 0,
            "Empty string should give zero correct numbers");
    BOOST_CHECK_MESSAGE(mp.correctNumber(problem) == problem.size(),
            "The correct number of the solution should be equal to the size "
            "of the problem.");
    BOOST_CHECK_MESSAGE(mp.correctNumber(problem + "abcde") == problem.size(),
            "correctNumber() should support longer guesses.");

    for (unsigned long i = 0; i < problem.size(); i++) {
        auto const guess = problem.substr(0, i);
        BOOST_CHECK_MESSAGE(mp.correctNumber(guess) == i,
            "First " + std::to_string(i) + " correct characters should give "
            + std::to_string(i) + " correct numbers.");
    }

    for (auto i = 0; i < std::pow(2, problem.size()); i++) {
        std::string guess = wrong_guess;
        unsigned int correct = 0;
        for (unsigned int j = 0; j < problem.size(); j++) {
            bool const bit_switch = (1 << j) & i;
            if (bit_switch) {
                guess[j] = problem[j];
                correct++;
            }
        }

        BOOST_CHECK_MESSAGE(mp.correctNumber(guess) == correct,
            "Guess " + guess + " should have " + std::to_string(correct)
            + " correct characters in " + problem + ".");
    }
}
