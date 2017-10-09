#include <vector>
#include <tuple>
#include <string>
using namespace std;

#include "memory_problem.h"

#ifndef MEMORY_QUIZ_H
#define MEMORY_QUIZ_H

class MemoryQuiz {
public:
    MemoryQuiz();
    MemoryQuiz(
    unsigned int const number_of_problems,
    unsigned int const sleep_time = 1000,
    unsigned int const nnumbers = 10);
    void startQuiz();
    void stopQuiz();
    void writeResultsPerMemoryProblem(string);
private:
    void initQuiz(unsigned int const);
    vector<MemoryProblem> question;
    vector<tuple<MemoryProblem, string, unsigned int>> answers;
    unsigned int sleep_time_in_milliseconds;
    unsigned int number_of_numbers;
    bool ran;
};

#endif // MEMORY_QUIZ_H
