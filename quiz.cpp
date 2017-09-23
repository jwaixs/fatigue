#include <iostream>
#include <random>
#include <functional>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <string>

#include <boost/filesystem.hpp>

#include "quiz.h"

using namespace std;
using namespace boost;

Quiz::Quiz() {
    length = 5;
    initQuiz(length);
}

Quiz::Quiz(unsigned int nquestions) {
    length = nquestions;
    initQuiz(length);
}

void Quiz::initQuiz(unsigned int nquestions) {
    mt19937::result_type seed = time(0);
    uniform_int_distribution<int> distribution(0,9);
    auto dice = bind(distribution, mt19937(seed));

    for (unsigned int i = 0; i < nquestions; i++) {
        int const x = dice();
        int const y = dice();
        int const z = x + y;
        Problem p(to_string(x) + " + " + to_string(y), to_string(z));
        question.push_back(p);
    }

    mistakes = 0;
    ran = false;
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
    ran = true;
}

double Quiz::totalTime() {
    return (stop - start) / 100.0;
}

void Quiz::stopQuiz() {
    if (ran) {
        cout << "Stop quiz." << endl;
        cout << "Number of questions: " << length << endl;
        cout << "Number of mistakes: " << mistakes << endl;
        cout << "Total time spend: " << totalTime() << endl;
        cout << "Time spend per question: " << totalTime() / length << endl;
    }
}

string Quiz::getCurrentTime() {
    /* I have no idea why it is so difficult to ask for the local time. */
    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);
 
    stringstream sstime;
    sstime << put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return sstime.str();
}

void Quiz::writeResults(string filename) {
    if (ran) {
        ofstream outfile;

        if (!filesystem::exists(filename.c_str())) {
            outfile.open(filename);
            outfile << "date,nquestions,mistakes,total time,time/question"
                << endl;
        } else {
            outfile.open(filename, ios_base::app);
        }

        outfile << getCurrentTime()
            << ", " << length
            << ", " << mistakes
            << ", " << totalTime()
            << ", " << totalTime() / length
            << endl;
        outfile.close();
    }
}
