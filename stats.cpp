// Copyright 2018 Noud Aldenhoven
//
// Using boost with using-declarations seems almost impossible to me. Therefore
// disable the cpplint error.
//
// cpplint: --filter=-build/namespaces

#include <sqlite3.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/combine.hpp>
#include <boost/tokenizer.hpp>
using namespace boost;

#include "./stats.h"
#include "./tools.h"

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  out << "[";

  if (!v.empty()) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b";
  }

  out << "]";
  return out;
}

ProblemStats::ProblemStats(std::string p, std::string a) {
  problem = p;
  answer = a;
  number_of_tries = 0;
  number_of_correct_tries = 0;
}

std::string ProblemStats::getProblem() { return problem; }

std::string ProblemStats::getAnswer() { return answer; }

void ProblemStats::addTry(unsigned int tries, double total_time) {
  number_of_tries += tries;
  number_of_correct_tries++;
  acc(total_time);
  time_per_try.push_back(total_time);
}

void ProblemStats::addTry(unsigned int tries, double total_time,
                          std::string date) {
  date_per_try.push_back(date);
  addTry(tries, total_time);
}

unsigned int ProblemStats::getNumberOfTries() { return number_of_tries; }

unsigned int ProblemStats::getNumberOfCorrectTries() {
  return number_of_correct_tries;
}

std::vector<double> ProblemStats::getTimePerTry() { return time_per_try; }

std::vector<std::string> ProblemStats::getDatePerTry() { return date_per_try; }

double ProblemStats::getMean() { return mean(acc); }

double ProblemStats::getMedian() { return median(acc); }

double ProblemStats::getStd() { return sqrt(variance(acc)); }

MemoryStats::MemoryStats(std::string const &p, std::string const &a,
                         std::string const &d, unsigned int c) {
  problem = p;
  answer = a;
  date = d;
  correct = c;
}

std::string const MemoryStats::getProblem() { return problem; }

std::string const MemoryStats::getAnswer() { return answer; }

std::string const MemoryStats::getDate() { return date; }

unsigned int MemoryStats::getCorrect() { return correct; }

void Statistics::readSpeedCSV(std::string const &csv_path) {
  std::ifstream csv;
  csv.open(csv_path.c_str());

  typedef tokenizer<escaped_list_separator<char>> Tokenizer;
  std::vector<std::string> svec;
  std::string line;

  // Get header, and drop it.
  getline(csv, line);

  std::string date, problem, answer;
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
      problem_statistics.emplace(problem, new ProblemStats(problem, answer));
    }
    problem_statistics[problem]->addTry(tries, time, date);
  }

  csv.close();
}

void Statistics::readSpeedSQL(std::string const &sql_path) {
  sqlite3 *db;
  sqlite3_stmt *stmt;

  if (sqlite3_open(sql_path.c_str(), &db) != SQLITE_OK) {
    std::string const err_msg =
        "readSpeedSQL(): could not open SQLite database " + sql_path + ".";
    throw std::runtime_error(err_msg);
  }

  std::string const query =
      "select date, problem, solution, number_of_tries, time_to_solve from "
      "speed;";
  if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
    std::string const err_msg = "readSpeedSQL(): could not execute query '" +
                                query + "', compiling error: '" +
                                sqlite3_errmsg(db) + "'.";
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_msg);
  }

  int ret_code;
  while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
    const std::string date = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
    const std::string problem = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    const std::string solution = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    const int tries = sqlite3_column_int(stmt, 3);
    const double time = sqlite3_column_double(stmt, 4);

    auto const search = problem_statistics.find(problem);
    if (search == problem_statistics.end()) {
      problem_statistics.emplace(problem, new ProblemStats(problem, solution));
    }
    problem_statistics[problem]->addTry(tries, time, date);
  }

  if (ret_code != SQLITE_DONE) {
    std::string const err_msg =
        "readSpeedSQL(): error while executing query '" + query +
        "', compiling error: '" + sqlite3_errmsg(db) + "'.";
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_msg);
  }

  sqlite3_close(db);
  sqlite3_finalize(stmt);
}

void Statistics::readMemoryCSV(std::string const &csv_path) {
  std::ifstream csv;
  csv.open(csv_path.c_str());

  typedef tokenizer<escaped_list_separator<char>> Tokenizer;
  std::vector<std::string> svec;
  std::string line;

  // Get the header, and drop it.
  getline(csv, line);

  std::string data, problem, answer;
  unsigned int correct;
  while (getline(csv, line)) {
    Tokenizer tok(line);
    svec.assign(tok.begin(), tok.end());

    data = svec.at(0);
    problem = svec.at(1);
    answer = svec.at(2);
    correct = lexical_cast<unsigned int>(svec.at(3));

    MemoryStats memory_stat(problem, answer, data, correct);
    memory_statistics.push_back(memory_stat);
  }

  csv.close();
}

void Statistics::readMemorySQL(std::string const &sql_path) {
  throw std::runtime_error("readMemorySQL(): Not implemented.");
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

  auto const min =
      *std::min_element(time_per_question.begin(), time_per_question.end());
  auto const max =
      *std::max_element(time_per_question.begin(), time_per_question.end());
  auto const bin_length = (max - min) / bins;

  if (max <= min) {
    std::string const err_msg =
        "printProblemMeanHistogram(): maximum value "
        "should be bigger than minimum value."
        " !(" +
        std::to_string(max) + " > " + std::to_string(min) + ")";
    throw std::runtime_error(err_msg);
  }

  std::vector<int> histogram(bins + 1);
  for (auto const time : time_per_question) {
    auto const which_bin = static_cast<int>((time - min) / bin_length);
    histogram.at(which_bin)++;
  }

  auto const hist_max_size =
      *std::max_element(histogram.begin(), histogram.end());
  for (unsigned int i = 0; i <= bins; i++) {
    if (histogram.at(i) < min_hist_size) {
      continue;
    }

    std::cout << format("%-4f - %-4f: ") % (min + i * bin_length) %
                     (min + (i + 1) * bin_length);

    unsigned int const hist_bar_size =
        (bar_width * histogram.at(i)) / hist_max_size;
    for (unsigned int j = 0; j < hist_bar_size; j++) {
      std::cout << "-";
    }

    std::cout << "| (" << histogram.at(i) << ")" << std::endl;
  }
}

void Statistics::printMemoryHistogram() {
  // Number of bins to show maximal in histogram.
  auto const bins = 10;

  // Maximum bin length.

  std::vector<unsigned int> histogram(bins + 1);
  for (auto &ms : memory_statistics) {
    auto hist_bin = ms.getCorrect();
    histogram.at(hist_bin)++;
  }

  auto const min = 0;
  auto const max = *std::max_element(histogram.begin(), histogram.end());

  if (!(max > min)) {
    std::string const err_msg =
        "printmemoryHistogram(): maximum value is not "
        "larger than minimum value."
        "!(" +
        std::to_string(max) + " > " + std::to_string(min) + ")";
    throw std::runtime_error(err_msg);
  }

  for (unsigned int i = 0; i <= bins; i++) {
    std::cout << format("%-2i") % i << " correct : ";
    unsigned int const hist_bar_size = (bar_width * histogram.at(i)) / max;
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

  std::vector<std::string> days_of_the_week{"Mon", "Tue", "Wed", "Thu",
                                            "Fri", "Sat", "Sun"};

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

  using acc =
      accumulator_set<double, features<tag::mean, tag::median, tag::count>>;

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
    std::cout << "| (" << value_time << "/" << total_tries << ")" << std::endl;
  }
}

void Statistics::printSpeedProblemPerProblem() {
  for (auto const &sp : problem_statistics) {
    std::cout << format("%-5s %-4f %-4f %i") % sp.first % sp.second->getMean() %
                     sp.second->getMedian() % sp.second->getNumberOfTries()
              << std::endl;
  }
}

void Statistics::printSpeedProblemPerWeek(unsigned int const back) {
  printSpeedProblemPerWeek(back, StatsType::Median);
}

void Statistics::printSpeedProblemPerWeek(unsigned int const back,
                                          StatsType const &stats_type) {
  using acc =
      accumulator_set<double, features<tag::mean, tag::median, tag::count>>;
  std::vector<std::pair<boost::posix_time::ptime, acc>> results_per_week;

  auto const now_time = ptimeFromString(getCurrentTime());
  auto const t = boost::posix_time::hours(7 * 24);
  auto cur_time = now_time - boost::posix_time::hours(back * 7 * 24);

  double time;
  std::string date;
  for (unsigned int i = 0; i < back; i++) {
    auto const cur_year = getYear(cur_time);
    auto const cur_week = getWeek(cur_time);

    acc cur_week_acc;
    for (auto const &ps : problem_statistics) {
      auto const &time_per_try = ps.second->getTimePerTry();
      auto const &date_per_try = ps.second->getDatePerTry();
      for (auto const &time_date : boost::combine(time_per_try, date_per_try)) {
        boost::tie(time, date) = time_date;
        auto const pdate = ptimeFromString(date);
        auto const pweek = getWeek(pdate);
        auto const pyear = getYear(pdate);

        if (pweek == cur_week && pyear == cur_year) {
          cur_week_acc(time);
        }
      }
    }

    results_per_week.push_back(
        std::pair<boost::posix_time::ptime, acc>{cur_time, cur_week_acc});

    cur_time += t;
  }

  for (auto const &pta : results_per_week) {
    auto const cur_year = getYear(pta.first);
    auto const cur_week = getWeek(pta.first);
    auto const num_tries = boost::accumulators::count(pta.second);

    double value;
    if (stats_type == StatsType::Median) {
      value = median(pta.second);
    } else if (stats_type == StatsType::Mean) {
      value = mean(pta.second);
    } else {
      throw std::runtime_error("StatsType not implemented.");
    }

    std::cout << cur_year << " " << cur_week << " " << num_tries << " " << value
              << std::endl;
  }
}

std::vector<float> Statistics::getSpeedData() {
  std::vector<float> speed_data;

  for (auto const &ps : problem_statistics) {
    auto const &time_per_try = ps.second->getTimePerTry();
    for (auto const &t : time_per_try) {
      speed_data.push_back(t);
    }
  }

  return speed_data;
}
