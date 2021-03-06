// Copyright 2018 Noud Aldenhoven

#include <iostream>
#include <string>

#include <boost/program_options.hpp>
using boost::program_options::bool_switch;
using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
#include <boost/filesystem.hpp>
using boost::filesystem::extension;

#include "./memory_quiz.h"
#include "./speed_quiz.h"
#include "./stats.h"
#include "./stats_type.h"

int main(int argc, char **argv) {
  std::string result_csv;
  std::string result_per_question_csv;
  unsigned int number_of_questions;
  bool do_problem_quiz, do_memory_quiz;
  bool display_median, display_mean;
  bool display_individual;

  options_description desc("Allowed options");
  desc.add_options()("help,h", "Print help usage.")(
      "result-csv,f", value(&result_csv), "Result csv output file.")(
      "result-per-question-csv,g", value(&result_per_question_csv),
      "Result per question csv output file.")(
      "problem-quiz,p", bool_switch(&do_problem_quiz)->default_value(false),
      "Do the problem quiz.")(
      "memory-quiz,m", bool_switch(&do_memory_quiz)->default_value(false),
      "Do the memory quiz.")("number-of-questions,n",
                             value(&number_of_questions)->default_value(5),
                             "Number of questions. (default: 5)")(
      "stats-speed,s", "Display statistics of speed quizes.")(
      "stats-memory,t", "Display statistics of memory quizes.")(
      "display-mean", bool_switch(&display_mean)->default_value(false),
      "Display mean values for stats per hour and day.")(
      "display-median", bool_switch(&display_median)->default_value(false),
      "Display median values for stats per hour and day.")(
      "display-individual",
      bool_switch(&display_individual)->default_value(false),
      "Display each problem individually");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help") || vm.count("h")) {
    std::cout << "Progam to measure your fatigueness doing a small quiz."
              << std::endl
              << desc;
    return 0;
  }

  if (vm.count("stats-speed") || vm.count("s")) {
    Statistics stats;
    if (extension(result_per_question_csv) == ".csv") {
      stats.readSpeedCSV(result_per_question_csv);
    } else if (extension(result_per_question_csv) == ".sql" ||
               extension(result_per_question_csv) == ".sqlite" ||
               extension(result_per_question_csv) == ".sqlite3") {
      stats.readSpeedSQL(result_per_question_csv);
    } else {
      throw std::runtime_error("Unknown file extension '" +
                               extension(result_per_question_csv) + "' of " +
                               result_per_question_csv + ".");
    }

    std::cout << "Speed problem histogram (num of tries):" << std::endl;
    stats.printProblemMeanHistogram();

    if (display_median) {
      std::cout << std::endl
                << "Speed problem per hour (median / num of tries):"
                << std::endl;
      stats.printSpeedProblemPerHour(StatsType::Median);
    }
    if (display_mean) {
      std::cout << std::endl
                << "Speed problem per hour (mean / num of tries):" << std::endl;
      stats.printSpeedProblemPerHour(StatsType::Mean);
    }

    if (display_median) {
      std::cout << std::endl
                << "Speed problem per day (median / num of tries):"
                << std::endl;
      stats.printSpeedProblemPerDay(StatsType::Median);
    }
    if (display_mean) {
      std::cout << std::endl
                << "Speed problem per day (mean / num of tries):" << std::endl;
      stats.printSpeedProblemPerDay(StatsType::Mean);
    }

    if (display_individual) {
      std::cout << std::endl << "Speed problem per problem:" << std::endl;
      stats.printSpeedProblemPerProblem();
    }

    stats.printSpeedProblemPerWeek(52);

    return 0;
  }

  if (vm.count("stats-memory") || vm.count("t")) {
    Statistics stats;
    if (extension(result_per_question_csv) == ".csv") {
      stats.readMemoryCSV(result_per_question_csv);
    } else if (extension(result_per_question_csv) == ".sql" ||
               extension(result_per_question_csv) == ".sqlite" ||
               extension(result_per_question_csv) == ".sqlite3") {
      stats.readMemorySQL(result_per_question_csv);
    } else {
      throw std::runtime_error("Unknown file extension '" +
                               extension(result_per_question_csv) + "' of " +
                               result_per_question_csv + ".");
    }
    stats.readMemoryCSV(result_per_question_csv);

    std::cout << std::endl
              << "Memory problem histogram (num of tries):" << std::endl;
    stats.printMemoryHistogram();

    return 0;
  }

  if (do_problem_quiz && do_memory_quiz) {
    std::cout << "Cannot do problem quiz and memory quiz at the same time."
              << " Please select only one of the two options." << std::endl;
  }

  if (do_problem_quiz) {
    SpeedQuiz speed_quiz(number_of_questions);
    std::cout << "Press enter to start the problem quiz.";
    std::cin.get();
    speed_quiz.startQuiz();
    speed_quiz.stopQuiz();

    if (vm.count("results-csv") > 0 || vm.count("f") > 0) {
      speed_quiz.writeResults(result_csv);
    }

    if (vm.count("result-per-question-csv") > 0 || vm.count("g") > 0) {
      auto const pvalue = speed_quiz.zeroHypothesis(result_per_question_csv);
      if (pvalue < 1.0) {
        std::cout << "Null hypothesis can be rejected with p-value " << pvalue
                  << "." << std::endl;
      }
      speed_quiz.writeResultsPerQuestion(result_per_question_csv);
    } else {
      std::cout << "result-per-question-csv is not set, not saving results."
                << std::endl;
    }
  }

  if (do_memory_quiz) {
    MemoryQuiz memory_quiz(number_of_questions, 1000);
    std::cout << "Press enter to start the memory quiz.";
    std::cin.get();
    memory_quiz.startQuiz();
    memory_quiz.stopQuiz();

    if (vm.count("result-per-question-csv") > 0 || vm.count("g") > 0) {
      memory_quiz.writeResultsPerMemoryProblem(result_per_question_csv);
    } else {
      std::cout << "result-per-question-csv is not set, not saving results."
                << std::endl;
    }
  }

  return 0;
}
