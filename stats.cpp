#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/range/combine.hpp>
using namespace boost;

#include "stats.h"
#include "tools.h"

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
    number_of_tries = 0;
    number_of_correct_tries = 0;
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

void ProblemStats::addTry(unsigned int tries, double total_time, string date) {
    date_per_try.push_back(date);
    addTry(tries, total_time);
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

vector<string> ProblemStats::getDatePerTry() {
    return date_per_try;
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

MemoryStats::MemoryStats(string const p, string const a) {
    problem = p;
    answer = a;
}

string const MemoryStats::getProblem() {
    return problem;
}

string const MemoryStats::getAnswer() {
    return answer;
}

void MemoryStats::addTry(unsigned int const correct, string const date) {
    correct_per_try.push_back(correct);
    date_per_try.push_back(date);
}

vector<unsigned int> MemoryStats::getCorrectPerTry() {
    return correct_per_try;
}

vector<string> MemoryStats::getDatePerTry() {
    return date_per_try;
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

    string date, problem, answer;
    unsigned int tries;
    double time;
    while (getline(csv, line)) {
        Tokenizer tok(line);
        svec.assign(tok.begin(), tok.end());

        date = svec.at(0);
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
        problem_statistics[problem]->addTry(tries, time, date);
    }
}

void Statistics::printProblemMeanHistogram() {
    // Number of bins to show maximal in histogram
    auto const bins = 50;

    // Minimal number of elements in a histogram bin.
    auto const min_hist_size = 10;

    std::vector<float> time_per_question;
    for (auto const &ps : problem_statistics) {
        for (auto const &time : ps.second->getTimePerTry()) {
            time_per_question.push_back(time);
        }
    }

    auto const min = *std::min_element(time_per_question.begin(),
                                       time_per_question.end());
    auto const max = *std::max_element(time_per_question.begin(),
                                       time_per_question.end());
    auto const bin_length = (max - min) / bins;

    std::vector<int> histogram(bins+1);
    for (auto const time : time_per_question) {
        auto const which_bin = static_cast<int>((time - min) / bin_length);
        histogram.at(which_bin)++;
    }

    auto const hist_max_size = *std::max_element(histogram.begin(), histogram.end());
    for (unsigned int i = 0; i <= bins; i++) {
        if (histogram.at(i) < min_hist_size) {
            continue;
        }

        std::cout << format("%-4f - %-4f: ") % (min + i*bin_length) % (min + (i+1)*bin_length);

        unsigned int const hist_bar_size = (bar_width * histogram.at(i)) / hist_max_size;
        for (unsigned int j = 0; j < hist_bar_size; j++) {
            std::cout << "-";
        }

        std::cout << "| (" << histogram.at(i) << ")" << std::endl;
    }
}

void Statistics::printSpeedProblemPerDay() {
    // Choose Median as default.
    printSpeedProblemPerDay(StatsType::Median);
}

void Statistics::printSpeedProblemPerDay(StatsType const &stats_type) {
    auto const min_tries_threshold = 10;

    std::vector<std::string> days_of_the_week{
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };

    using acc = accumulator_set<double, features<tag::mean, tag::median>>;

    std::map<std::string, acc> time_per_day;
    std::map<std::string, unsigned int> num_of_tries_per_day;

    for (auto const &day : days_of_the_week) {
        acc new_acc;
        time_per_day[day] = new_acc;
        num_of_tries_per_day[day] = 0;
    }

    double time;
    std::string date;
    for (auto const &ps : problem_statistics) {
        auto const &time_per_try = ps.second->getTimePerTry();
        auto const &date_per_try = ps.second->getDatePerTry();
        for (auto const &time_date : boost::combine(time_per_try, date_per_try)) {
            boost::tie(time, date) = time_date;
            auto const pdate = ptimeFromString(date);
            auto const day = getDayOfWeek(pdate);

            time_per_day[day](time);
            num_of_tries_per_day[day]++;
        }
    }

    double min = 0;
    double max = 0;
    std::map<std::string, double> value_time_per_day;
    for (auto const &day : days_of_the_week) {
        if (num_of_tries_per_day[day] > min_tries_threshold) {
            double value;

            if (stats_type == StatsType::Median) {
                value = median(time_per_day[day]);
            } else if (stats_type == StatsType::Mean) {
                value = mean(time_per_day[day]);
            } else {
                throw std::runtime_error("StatsType not implemented.");
            }

            value_time_per_day[day] = value;

            min = (min == 0 || min > value) ? value : min;
            max = max < value ? value : max;
        } else {
            value_time_per_day[day] = 0.0;
        }
    }

    for (auto const &day : days_of_the_week) {
        if (num_of_tries_per_day[day] < min_tries_threshold) {
            continue;
        }

        std::cout << day << ": ";

        auto const value = value_time_per_day[day];
        auto const total_tries = num_of_tries_per_day[day];
        auto const width = (value - min) / (max - min) * bar_width;
        for (unsigned int i = 0; i < width; i++) {
            std::cout << "-";
        }
        std::cout << "| (" << value << "/" << total_tries << ")" << std::endl;
    }
}

void Statistics::printSpeedProblemPerHour() {
    // Choose median per default.
    printSpeedProblemPerHour(StatsType::Median);
}

void Statistics::printSpeedProblemPerHour(StatsType const &stats_type) {
    auto const min_tries_threshold = 10;

    using acc = accumulator_set<double, features<tag::mean, tag::median, tag::count>>;

    std::vector<acc> acc_per_hour;
    acc_per_hour.resize(24);

    double time;
    std::string date;
    for (auto const &ps : problem_statistics) {
        auto const &time_per_try = ps.second->getTimePerTry();
        auto const &date_per_try = ps.second->getDatePerTry();
        for (auto const &time_date : boost::combine(time_per_try, date_per_try)) {
            boost::tie(time, date) = time_date;
            auto const pdate = ptimeFromString(date);
            auto const hour = getHourOfDay(pdate);

            acc_per_hour.at(hour)(time);
        }
    }

    std::vector<double> value_per_hour;
    float min = 0;
    float max = 0;
    for (unsigned int hour = 0; hour < 24; hour++) {
        auto const total_tries = boost::accumulators::count(acc_per_hour.at(hour));

        if (total_tries > min_tries_threshold) {
            double value;
            if (stats_type == StatsType::Median) {
                value = median(acc_per_hour.at(hour));
            } else if (stats_type == StatsType::Mean) {
                value = mean(acc_per_hour.at(hour));
            } else {
                throw std::runtime_error("StatsType not implemented.");
            }
            value_per_hour.push_back(value);

            min = (min == 0 || min > value) ? value : min;
            max = (max < value) ? value : max;
        } else {
            value_per_hour.push_back(0);
        }
    }

    for (unsigned int hour = 0; hour < 24; hour++) {
        auto const value_time = value_per_hour.at(hour);
        auto const total_tries = boost::accumulators::count(acc_per_hour.at(hour));

        if (total_tries < min_tries_threshold) {
            continue;
        }

        std::cout << format("%-2i:00-%-2i:00: ") % hour % (hour + 1);

        auto const width = (value_time - min) / (max - min) * bar_width;
        for (unsigned int i = 0; i < width; i++) {
            std::cout << "-";
        }
        std::cout << "| (" << value_time << "/" << total_tries << ")"
            << std::endl;
    }
}

void Statistics::printSpeedProblemPerProblem() {
    for (auto const &sp : problem_statistics) {
        std::cout << format("%-5s %-4f %-4f %i")
            % sp.first
            % sp.second->getMean()
            % sp.second->getMedian()
            % sp.second->getNumberOfTries() << std::endl;
    }
}
