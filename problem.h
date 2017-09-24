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
private:
    string problem;
    string solution;
};

#endif // PROBLEM_H
