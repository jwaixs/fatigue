#include "quiz.h"

int main(int argc, char **argv) {
    Quiz quiz;
    quiz.startQuiz();
    quiz.stopQuiz();
    quiz.writeResults("./results.csv");
    return 0; 
}
