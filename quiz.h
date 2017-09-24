#include <deque>
#include <ctime>

#include "problem.h"

#ifndef QUIZ_H
#define QUIZ_H

using namespace std;

class Quiz {
public:
    Quiz();
    Quiz(unsigned int);
    void startQuiz();
    void stopQuiz();
    void writeResults(string);
private:
    string getCurrentTime();
    double totalTime();
    void initQuiz(unsigned int);
    deque<Problem> question;
    unsigned int mistakes;
    unsigned int length;
    clock_t start;
    clock_t stop;
    bool ran;
};

#endif // QUIZ_H
