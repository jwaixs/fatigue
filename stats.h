#include <string>
#include <vector>
#include <map>
using namespace std;

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
using namespace boost::accumulators;

#ifndef STATS_H
#define STATS_H

class ProblemStats {
public:
    ProblemStats(string, string);
    string getProblem();
    string getAnswer();
    void addTry(unsigned int, double);
    unsigned int getNumberOfTries();
    unsigned int getNumberOfCorrectTries();
    vector<double> getTimePerTry();
    double getMean();
    double getMedian();
    double getStd();
private:
    string problem;
    string answer;
    unsigned int number_of_tries;
    unsigned int number_of_correct_tries;
    vector<double> time_per_try;
    accumulator_set<double, features<tag::mean, tag::median, tag::variance>> acc;
};

class Statistics {
public:
    Statistics(string);
    void readCSV(string);
    void printProblemMeanHistogram();
private:
    map<string, ProblemStats *> problem_statistics;
};

#endif /* STATS_H */
