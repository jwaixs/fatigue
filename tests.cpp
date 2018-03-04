#define BOOST_TEST_MODULE Fatigue_Test_Suite
#include <boost/test/included/unit_test.hpp>
#include <boost/range/combine.hpp>

#include <string>

#include "speed_problem.h"
#include "memory_problem.h"
#include "stats.h"
#include "tools.h"
#include "ks_test.h"

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

    ProblemStats psd(problem, answer);

    std::vector<std::string> dates{"01-01-01", "02-02-02", "03-03-03",
                                   "04-04-04", "05-05-05"};

    std::string t3;
    for (auto const &try_time_date : boost::combine(tries, times, dates)) {
        boost::tie(t1, t2, t3) = try_time_date;
        psd.addTry(t1, t2, t3);
    }

    BOOST_CHECK_MESSAGE(psd.getNumberOfTries() == number_of_tries,
            "getNumberOfTries should be equal to "
            + std::to_string(number_of_tries));
    BOOST_CHECK_MESSAGE(psd.getTimePerTry().size() == tries.size(),
            "getTimePerTry size should be equal to "
            + std::to_string(tries.size()));
    BOOST_CHECK_MESSAGE(psd.getNumberOfCorrectTries() == tries.size(),
            "getNumberOfCorrectTries should be equal to "
            + std::to_string(tries.size()));

    auto const psd_dates = psd.getDatePerTry();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        psd_dates.begin(), psd_dates.end(), dates.begin(), dates.end()
    );

    BOOST_CHECK_MESSAGE(std::abs(psd.getMean() - 0.5) < tolerance,
            "Mean should be equal to 0.5.");
    BOOST_CHECK_MESSAGE(std::abs(psd.getMedian() - 0.3) < tolerance,
            "Median should be equal to 0.3.");
    BOOST_CHECK_MESSAGE(std::abs(psd.getStd()*psd.getStd() - 0.26) < tolerance,
            "Std squared should be equal to 0.26.");
}

BOOST_AUTO_TEST_CASE(time_tools) {
    std::string const time_str = "2018-02-25 11:09:01";
    auto const current_ptime = ptimeFromString(time_str);

    auto const current_date = current_ptime.date();
    BOOST_CHECK_MESSAGE(current_date.year() == 2018,
            "Current year should be 2018.");
    BOOST_CHECK_MESSAGE(
        std::strcmp(current_date.month().as_short_string(), "Feb") == 0,
        "Current month should be Feb.");
    BOOST_CHECK_MESSAGE(
        std::strcmp(current_date.day_of_week().as_short_string(), "Sun") == 0,
        "Current day of the week should be Sun.");

    BOOST_CHECK_MESSAGE(getDayOfWeek(current_ptime) == "Sun",
            "getDayOfWeek should be Sun.");
    BOOST_CHECK_MESSAGE(getMonth(current_ptime) == "Feb",
            "getMonth should be Feb.");
    BOOST_CHECK_MESSAGE(getHourOfDay(current_ptime) == 11,
            "getHourOfDay should be 11.");

    auto const current_time = current_ptime.time_of_day();
    BOOST_CHECK_MESSAGE(current_time.hours() == 11,
            "Current hour should be 11.");
    BOOST_CHECK_MESSAGE(current_time.minutes() == 9,
            "Current minutes should be 9.");
    BOOST_CHECK_MESSAGE(current_time.seconds() == 1,
            "Current seconds should be 1.");
}

BOOST_AUTO_TEST_CASE(cumulative_function) {
    std::vector<float> const data0;
    auto cf0 = CumulativeFunction<float>(data0);

    for (unsigned int i = 0; i <= 9; i++) {
        BOOST_CHECK_MESSAGE(std::abs(cf0(i) - 0) < tolerance,
            "Cumulative function with no data should always be zero.");
    }

    std::vector<float> const data1{1.0};
    auto cf1 = CumulativeFunction<float>(data1);

    BOOST_CHECK_MESSAGE(std::abs(cf1(0) - 0) < tolerance,
            "Cumulative function below data should be zero.");
    BOOST_CHECK_MESSAGE(std::abs(cf1(1) - 1) < tolerance,
            "Cumulative function equal or above data should be one.");

    std::vector<float> const data2{4.0, 5.0, 2.0, 2.0, 1.0,
                                  6.0, 6.0, 6.0, 7.0, 8.0};

    auto cf2 = CumulativeFunction<float>(data2);

    std::vector<float> const result{0.0, 0.1, 0.3, 0.3, 0.4, 0.5,
                                    0.8, 0.9, 1.0, 1.0};
    for (unsigned int i = 0; i <= 9; i++) {
        BOOST_CHECK_MESSAGE(std::abs(cf2(i) - result.at(i)) < tolerance,
            "Cumulative function should output correct results.");
    }


    auto const cf2_step_positions = cf2.getStepPositions();
    auto const correct_step_positions = std::vector<float>{
                                    1.0, 2.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    for (unsigned int i = 0; i < cf2_step_positions.size(); i++) {
        BOOST_CHECK_MESSAGE(
            std::abs(cf2_step_positions.at(i) - correct_step_positions.at(i)) < tolerance,
            "Cumulative function step positions should be correct.");
    }
}

BOOST_AUTO_TEST_CASE(two_sample_Kolmogorov_Smirnov_test) {
    auto const d1 = std::vector<float>{0.1, 0.2, 0.3, 0.4, 0.5};
    auto const d2 = std::vector<float>{-2, -1, 0, 1};
    TwoSampleKSTest<float> ks_test(d1, d2);

    auto const twodigit = 0.01;

    // Taken from the wiki:
    //   https://en.wikipedia.org/wiki/Kolmogorov%E2%80%93Smirnov_test
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.1) - 1.22) < twodigit,
        "c(0.1) should be 1.22.");
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.05) - 1.36) < twodigit,
        "c(0.05) should be 1.36.");
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.025) - 1.48) < twodigit,
        "c(0.025) should be 1.48.");
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.01) - 1.63) < twodigit,
        "c(0.01) should be 1.63.");
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.005) - 1.73) < twodigit,
        "c(0.005) should be 1.73.");
    BOOST_CHECK_MESSAGE(std::abs(ks_test.c(0.001) - 1.95) < twodigit,
        "c(0.001) should be 1.95.");
}
