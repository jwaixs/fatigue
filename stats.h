#include <string>
#include <vector>
#include <map>
using namespace std;

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
using namespace boost::accumulators;

#include "stats_type.h"

#ifndef STATS_H
#define STATS_H

class ProblemStats {
public:
    ProblemStats(string, string);
    string getProblem();
    string getAnswer();
    void addTry(unsigned int, double);
    void addTry(unsigned int, double, string);
    unsigned int getNumberOfTries();
    unsigned int getNumberOfCorrectTries();
    vector<double> getTimePerTry();
    vector<string> getDatePerTry();
    double getMean();
    double getMedian();
    double getStd();
private:
    string problem;
    string answer;
    unsigned int number_of_tries;
    unsigned int number_of_correct_tries;
    vector<double> time_per_try;
    vector<string> date_per_try;
    accumulator_set<double, features<tag::mean, tag::median, tag::variance>> acc;
};

class MemoryStats {
public:
    MemoryStats(string const &, string const &, string const &, unsigned int);
    string const getProblem();
    string const getAnswer();
    string const getDate();
    unsigned int getCorrect();
private:
    string problem;
    string answer;
    string date;
    unsigned int correct;
};

class Statistics {
public:
    void readSpeedCSV(string const &);
    void readMemoryCSV(string const &);
    void printProblemMeanHistogram();
    void printMemoryHistogram();
    void printSpeedProblemPerHour();
    void printSpeedProblemPerHour(StatsType const &);
    void printSpeedProblemPerDay();
    void printSpeedProblemPerDay(StatsType const &);
    void printSpeedProblemPerProblem();
    void printSpeedProblemPerWeek(unsigned int const);
    void printSpeedProblemPerWeek(unsigned int const, StatsType const &);
    std::vector<float> getSpeedData();
private:
    map<string, ProblemStats *> problem_statistics;
    std::vector<MemoryStats> memory_statistics;
    unsigned short const bar_width = 60;
};

#endif /* STATS_H */
