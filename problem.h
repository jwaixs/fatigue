#include <string>

#ifndef PROBLEM_H
#define PROBLEM_H

using namespace std;

class Problem {
public:
    Problem(string const, string const);
    string getProblem();
    string getSolution();
    bool isCorrect(string const);
    void addTime(double);
    unsigned int getNumberOfTries();
    double getTimeToSolve();
private:
    string problem;
    string solution;
    unsigned int number_of_tries;
    double time_to_solve;
};

#endif // PROBLEM_H
