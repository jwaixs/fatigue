// Copyright 2018 Noud Aldenhoven
//
// Using boost with using-declarations seems almost impossible to me. Therefore
// disable the cpplint error.
//
// cpplint: --filter=-build/namespaces

#include <map>
#include <string>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
using namespace boost::accumulators;

#include "./stats_type.h"

#ifndef STATS_H_
#define STATS_H_

class ProblemStats {
 public:
  ProblemStats(std::string, std::string);
  std::string getProblem();
  std::string getAnswer();
  void addTry(unsigned int, double);
  void addTry(unsigned int, double, std::string);
  unsigned int getNumberOfTries();
  unsigned int getNumberOfCorrectTries();
  std::vector<double> getTimePerTry();
  std::vector<std::string> getDatePerTry();
  double getMean();
  double getMedian();
  double getStd();

 private:
  std::string problem;
  std::string answer;
  unsigned int number_of_tries;
  unsigned int number_of_correct_tries;
  std::vector<double> time_per_try;
  std::vector<std::string> date_per_try;
  accumulator_set<double, features<tag::mean, tag::median, tag::variance>> acc;
};

class MemoryStats {
 public:
  MemoryStats(std::string const &, std::string const &, std::string const &,
              unsigned int);
  std::string const getProblem();
  std::string const getAnswer();
  std::string const getDate();
  unsigned int getCorrect();

 private:
  std::string problem;
  std::string answer;
  std::string date;
  unsigned int correct;
};

class Statistics {
 public:
  void readSpeedCSV(std::string const &);
  void readMemoryCSV(std::string const &);
  void printProblemMeanHistogram();
  void printMemoryHistogram();
  void printSpeedProblemPerHour();
  void printSpeedProblemPerHour(StatsType const &);
  void printSpeedProblemPerDay();
  void printSpeedProblemPerDay(StatsType const &);
  void printSpeedProblemPerProblem();
  void printSpeedProblemPerWeek(unsigned int const);
  void printSpeedProblemPerWeek(unsigned int const, StatsType const &);
  std::vector<float> getSpeedData();

 private:
  std::map<std::string, ProblemStats *> problem_statistics;
  std::vector<MemoryStats> memory_statistics;
  uint8_t const bar_width = 60;
};

#endif  // STATS_H_
