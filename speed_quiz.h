#include <deque>
using namespace std;

#include "speed_problem.h"

#ifndef SPEED_QUIZ_H
#define SPEED_QUIZ_H

using namespace std;

class SpeedQuiz {
public:
    SpeedQuiz();
    SpeedQuiz(unsigned int);
    void startQuiz();
    void stopQuiz();
    void writeResults(string);
    void writeResultsPerQuestion(string);
private:
    double totalTime();
    void initQuiz(unsigned int);
    deque<SpeedProblem> question;
    deque<SpeedProblem> correct_answers;
    unsigned int mistakes;
    unsigned int length;
    unsigned int start;
    unsigned int stop;
    bool ran;
};

#endif // SPEED_QUIZ_H
