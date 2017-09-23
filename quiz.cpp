#include <iostream>
#include <random>
#include <functional>

#include "quiz.h"

using namespace std;

Quiz::Quiz() {
    length = 5;
    initQuiz(length);
}

Quiz::Quiz(unsigned int nquestions) {
    length = nquestions;
    initQuiz(length);
}

void Quiz::initQuiz(unsigned int nquestions) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,9);
    auto dice = bind(distribution, generator);

    for (unsigned int i = 0; i < nquestions; i++) {
        int const x = dice();
        int const y = dice();
        int const z = x + y;
        Problem p(to_string(x) + " + " + to_string(y), to_string(z));
        question.push_back(p);
    }

    mistakes = 0;
}

void Quiz::startQuiz() {
    string answer;
    start = clock();
    while (!question.empty()) {
        auto p = question.front();
        question.pop_front();

        cout << p.getProblem() << " > ";
        if (!getline(cin, answer)) {
            // I/O error
            throw "Error";
        }
        if (!p.isCorrect(answer)) {
           cout << "Wrong!" << endl;
           mistakes++;
           question.push_back(p);
        }
    }
    stop = clock();
}

double Quiz::totalTime() {
    return (stop - start) / 100.0;
}

void Quiz::stopQuiz() {
    cout << "Stop quiz." << endl;
    cout << "Number of questions: " << length << endl;
    cout << "Number of mistakes: " << mistakes << endl;
    cout << "Total time spend: " << totalTime() << endl;
    cout << "Time spend per question: " << totalTime() / length << endl;
}
