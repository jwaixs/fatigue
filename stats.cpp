#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using namespace boost;

#include "stats.h"

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "[";

    if (!v.empty()) {
        copy(v.begin(), v.end(), ostream_iterator<T>(out, ", "));
        out << "\b\b";
    }

    out << "]";
    return out;
}

ProblemStats::ProblemStats(string p, string a) {
    problem = p;
    answer = a;
}

string ProblemStats::getProblem() {
    return problem;
}

string ProblemStats::getAnswer() {
    return answer;
}

void ProblemStats::addTry(unsigned int tries, double total_time) {
    number_of_tries += tries;
    number_of_correct_tries++;
    acc(total_time);
    time_per_try.push_back(total_time);
}

unsigned int ProblemStats::getNumberOfTries() {
    return number_of_tries;
}

unsigned int ProblemStats::getNumberOfCorrectTries() {
    return number_of_correct_tries;
}

vector<double> ProblemStats::getTimePerTry() {
    return time_per_try;
}

double ProblemStats::getMean() {
    return mean(acc);
}

double ProblemStats::getMedian() {
    return median(acc);
}

double ProblemStats::getStd() {
    return sqrt(variance(acc));
}

Statistics::Statistics(string csv_path) {
    readCSV(csv_path);
}

void Statistics::readCSV(string csv_path) {
    ifstream csv;
    csv.open(csv_path.c_str());

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector<string> svec;
    string line;

    // Get header, and drop it.
    getline(csv, line);

    string problem, answer;
    unsigned int tries;
    double time;
    while (getline(csv, line)) {
        Tokenizer tok(line);
        svec.assign(tok.begin(), tok.end());

        problem = svec.at(1);
        answer = svec.at(2);
        tries = lexical_cast<unsigned int>(svec.at(3));
        time = lexical_cast<double>(svec.at(4));

        auto const search = problem_statistics.find(problem);
        if (search == problem_statistics.end()) {
            problem_statistics.emplace(
                problem, new ProblemStats(problem, answer)
            );
        }
        problem_statistics[problem]->addTry(tries, time);
    }

    for (auto const ps : problem_statistics) {
        cout << ps.first << " = " << ps.second->getAnswer() << endl;
        cout << ps.second->getTimePerTry()
            << " " << ps.second->getMean()
            << " " << ps.second->getMedian()
            << " " << ps.second->getStd() << endl;
    }
}

