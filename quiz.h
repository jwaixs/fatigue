#include <deque>
using namespace std;

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
    void writeResultsPerQuestion(string);
private:
    double totalTime();
    void initQuiz(unsigned int);
    deque<Problem> question;
    deque<Problem> correct_answers;
    unsigned int mistakes;
    unsigned int length;
    unsigned int start;
    unsigned int stop;
    bool ran;
};

#endif // QUIZ_H
