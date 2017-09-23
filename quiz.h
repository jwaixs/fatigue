#include <deque>
#include <ctime>

#include "problem.h"

using namespace std;

class Quiz {
public:
    Quiz();
    Quiz(unsigned int);
    void startQuiz();
    void stopQuiz();
    double totalTime();
private:
    void initQuiz(unsigned int);
    deque<Problem> question;
    unsigned int mistakes;
    unsigned int length;
    clock_t start;
    clock_t stop;
};
