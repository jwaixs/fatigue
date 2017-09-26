#include <string>
#include <vector>
#include <map>
using namespace std;

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
private:
    string problem;
    string answer;
    unsigned int number_of_tries;
    unsigned int number_of_correct_tries;
    vector<double> time_per_try;
};

class Statistics {
public:
    Statistics(string);
    void readCSV(string);
private:
    map<string, ProblemStats *> problem_statistics;
};

#endif /* STATS_H */
