#include <iostream>
#include <random>
#include <functional>

#include "quiz.h"

using namespace std;

Quiz::Quiz() {
    cout << "Quiz constructor." << endl;

    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,9);
    auto dice = bind(distribution, generator);

    for (unsigned int i = 0; i < 10; i++) {
        int const x = dice();
        int const y = dice();
        int const z = x + y;
        Problem p(to_string(x) + " + " + to_string(y), to_string(z));
        question.push_back(p);
    }
}

void Quiz::startQuiz() {
    cout << "Start quiz." << endl;
    while (!question.empty()) {
        auto p = question.front();
        cout << p.getProblem() << " = " << p.getSolution() << endl;
        question.pop_front();
    }
}

void Quiz::stopQuiz() {
    cout << "Stop quiz." << endl;
}
