#include <iostream>

#include "problem.h"

using namespace std;

Problem::Problem(string const p, string const s) {
    problem = p;
    solution = s;
}

string Problem::getProblem() {
    return problem;
}

string Problem::getSolution() {
    return solution;
}

bool Problem::isCorrect(string const answer) {
    return answer == solution;
}
