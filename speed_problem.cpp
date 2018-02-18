#include <iostream>

#include "speed_problem.h"

using namespace std;

SpeedProblem::SpeedProblem(string const p, string const s) {
    problem = p;
    solution = s;
    number_of_tries = 1;
    time_to_solve = 0.0;
}

string SpeedProblem::getProblem() {
    return problem;
}

string SpeedProblem::getSolution() {
    return solution;
}

bool SpeedProblem::isCorrect(string const answer) {
    if (answer == solution) {
        return true;
    } else {
        number_of_tries++;
        return false;
    }
}

void SpeedProblem::addTime(double time) {
    if (time > 0) {
        time_to_solve += time;
    } else {
        throw runtime_error("Time cannot be negative.");
    }
}

unsigned int SpeedProblem::getNumberOfTries() {
    return number_of_tries;
}

double SpeedProblem::getTimeToSolve() {
    return time_to_solve;
}
