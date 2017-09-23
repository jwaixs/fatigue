#include <deque>

#include "problem.h"

using namespace std;

class Quiz {
public:
    Quiz();
    void startQuiz();
    void stopQuiz();
private:
    deque<Problem> question;
};
