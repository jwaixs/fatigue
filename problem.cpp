#include <iostream>

#include "problem.h"

using namespace std;

Problem::Problem(string const p, string const s) {
    problem = p;
    solution = s;
    number_of_tries = 1;
    time_to_solve = 0.0;
}

string Problem::getProblem() {
    return problem;
}

string Problem::getSolution() {
    return solution;
}

bool Problem::isCorrect(string const answer) {
    if (answer == solution) {
        return true;
    } else {
        number_of_tries++;
        return false;
    }
}

void Problem::addTime(double time) {
    if (time > 0) {
        time_to_solve += time;
    } else {
        throw runtime_error("Time cannot be negative.");
    }
}

unsigned int Problem::getNumberOfTries() {
    return number_of_tries;
}

double Problem::getTimeToSolve() {
    return time_to_solve;
}
