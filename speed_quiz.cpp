#include <iostream>
#include <random>
#include <functional>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>

#include <boost/filesystem.hpp>

#include "speed_quiz.h"
#include "tools.h"

using namespace std;
using namespace boost;
using namespace chrono;

SpeedQuiz::SpeedQuiz() {
    length = 5;
    initQuiz(length);
}

SpeedQuiz::SpeedQuiz(unsigned int nquestions) {
    length = nquestions;
    initQuiz(length);
}

void SpeedQuiz::initQuiz(unsigned int nquestions) {
    mt19937::result_type seed = time(0);
    uniform_int_distribution<int> distribution(0,9);
    auto dice = bind(distribution, mt19937(seed));

    for (unsigned int i = 0; i < nquestions; i++) {
        int const x = dice();
        int const y = dice();
        int const z = x + y;
        SpeedProblem p(to_string(x) + " + " + to_string(y), to_string(z));
        question.push_back(p);
    }

    mistakes = 0;
    ran = false;
}

void SpeedQuiz::startQuiz() {
    string answer;
    start = duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()
    ).count();
    unsigned int q_start, q_stop;
    while (!question.empty()) {
        auto p = question.front();
        question.pop_front();

        cout << p.getProblem() << " > ";
        q_start = duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()
        ).count();
        if (!getline(cin, answer)) {
            throw runtime_error("I/O Error.");
        }
        q_stop = duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()
        ).count();
        p.addTime((q_stop - q_start) / 1000.0);
        if (!p.isCorrect(answer)) {
            cout << "Wrong!" << endl;
            mistakes++;
            question.push_back(p);
        } else {
            correct_answers.push_back(p);
        }
    }
    stop = duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()
    ).count();
    ran = true;
}

double SpeedQuiz::totalTime() {
    return (stop - start) / 1000.0;
}

void SpeedQuiz::stopQuiz() {
    if (ran) {
        cout << "Stop quiz." << endl;
        cout << "Number of questions: " << length << endl;
        cout << "Number of mistakes: " << mistakes << endl;
        cout << "Total time spend: " << totalTime() << endl;
        cout << "Time spend per question: " << totalTime() / length << endl;

        for (auto &p : correct_answers) {
            cout << p.getProblem() << " = " << p.getSolution()
                << ", " << p.getNumberOfTries()
                << ", " << p.getTimeToSolve() << endl;
        }
    }
}

void SpeedQuiz::writeResults(string filename) {
    if (ran) {
        ofstream outfile;

        if (!filesystem::exists(filename.c_str())) {
            outfile.open(filename);
            outfile << "date,nquestions,mistakes,total_time,time/question"
                << endl;
        } else {
            outfile.open(filename, ios_base::app);
        }

        outfile << getCurrentTime()
            << "," << length
            << "," << mistakes
            << "," << totalTime()
            << "," << totalTime() / length
            << endl;
        outfile.close();
    }
}

void SpeedQuiz::writeResultsPerQuestion(string filename) {
    if (ran) {
        ofstream outfile;

        if (!filesystem::exists(filename.c_str())) {
            outfile.open(filename);
            outfile << "date,problem,solution,number_of_tries,time_to_solve"
                << endl;
        } else {
            outfile.open(filename, ios_base::app);
        }

        for (auto &p : correct_answers) {
            outfile << getCurrentTime()
                << "," << p.getProblem()
                << "," << p.getSolution()
                << "," << p.getNumberOfTries()
                << "," << p.getTimeToSolve() << endl;
        }
        outfile.close();
    }
}
