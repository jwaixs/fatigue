#define BOOST_TEST_MODULE Fatigue_Test_Suite
#include <boost/test/included/unit_test.hpp>
#include <boost/range/combine.hpp>

#include <string>

#include "speed_problem.h"
#include "memory_problem.h"
#include "stats.h"

// Floating point tolerance. I don't know how to get unit_test::tolerance
// working. Hence I test it myself.
auto const tolerance = 0.00001;

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

BOOST_AUTO_TEST_CASE(statistic) {
    std::string const problem = "problem";
    std::string const answer = "answer";

    ProblemStats ps(problem, answer);

    BOOST_CHECK_MESSAGE(ps.getProblem() == problem,
            "getProblem() should return problem.");
    BOOST_CHECK_MESSAGE(ps.getAnswer() == answer,
            "getAnswer() should return answer.");

    BOOST_CHECK_MESSAGE(ps.getNumberOfTries() == 0,
            "Initial getNumberOfTries() should be zero.");
    BOOST_CHECK_MESSAGE(ps.getTimePerTry().size() == 0,
            "Initial getTimePerTry() should be zero.");
    BOOST_CHECK_MESSAGE(ps.getNumberOfCorrectTries() == 0,
            "Initial getNumberOfCorrectTries should be zero.");

    std::vector<unsigned int> tries{1, 2, 3, 4, 5};
    std::vector<double> times{0.1, 0.2, 0.3, 0.4, 1.5};

    unsigned int t1;
    double t2;
    for (auto const &try_time : boost::combine(tries, times)) {
        boost::tie(t1, t2) = try_time;
        ps.addTry(t1, t2);
    }

    unsigned int number_of_tries = std::accumulate(tries.begin(), tries.end(), 0);

    BOOST_CHECK_MESSAGE(ps.getNumberOfTries() == number_of_tries,
            "getNumberOfTries should be equal to "
            + std::to_string(number_of_tries));
    BOOST_CHECK_MESSAGE(ps.getTimePerTry().size() == tries.size(),
            "getTimePerTry size should be equal to "
            + std::to_string(tries.size()));
    BOOST_CHECK_MESSAGE(ps.getNumberOfCorrectTries() == tries.size(),
            "getNumberOfCorrectTries should be equal to "
            + std::to_string(tries.size()));

    BOOST_CHECK_MESSAGE(std::abs(ps.getMean() - 0.5) < tolerance,
            "Mean should be equal to 0.5.");
    BOOST_CHECK_MESSAGE(std::abs(ps.getMedian() - 0.3) < tolerance,
            "Median should be equal to 0.3.");
    BOOST_CHECK_MESSAGE(std::abs(ps.getStd()*ps.getStd() - 0.26) < tolerance,
            "Std squared should be equal to 0.26.");
}
