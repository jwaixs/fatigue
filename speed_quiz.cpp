// Copyright 2018 Noud Aldenhoven
//
// Chrono is an unaproved C++11 library. Use them anyway.
//
// cpplint: --filter=-build/c++11

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include <boost/filesystem.hpp>

#include "./ks_test.h"
#include "./speed_quiz.h"
#include "./stats.h"
#include "./tools.h"

SpeedQuiz::SpeedQuiz() {
  length = 5;
  initQuiz(length);
}

SpeedQuiz::SpeedQuiz(unsigned int nquestions) {
  length = nquestions;
  initQuiz(length);
}

void SpeedQuiz::initQuiz(unsigned int nquestions) {
  std::mt19937::result_type seed = time(0);
  std::uniform_int_distribution<int> distribution(0, 9);
  auto dice = std::bind(distribution, std::mt19937(seed));

  for (unsigned int i = 0; i < nquestions; i++) {
    int const x = dice();
    int const y = dice();
    int const z = x + y;
    SpeedProblem p(std::to_string(x) + " + " + std::to_string(y),
                   std::to_string(z));
    question.push_back(p);
  }

  mistakes = 0;
  ran = false;
}

void SpeedQuiz::startQuiz() {
  std::string answer;
  start = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now().time_since_epoch())
              .count();
  unsigned int q_start, q_stop;
  while (!question.empty()) {
    auto p = question.front();
    question.pop_front();

    std::cout << p.getProblem() << " > ";
    q_start = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now().time_since_epoch())
                  .count();
    if (!std::getline(std::cin, answer)) {
      throw std::runtime_error("I/O Error.");
    }
    q_stop = std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now().time_since_epoch())
                 .count();
    p.addTime((q_stop - q_start) / 1000.0);
    if (!p.isCorrect(answer)) {
      std::cout << "Wrong!" << std::endl;
      mistakes++;
      question.push_back(p);
    } else {
      correct_answers.push_back(p);
    }
  }
  stop = std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
             .count();
  ran = true;
}

double SpeedQuiz::totalTime() { return (stop - start) / 1000.0; }

void SpeedQuiz::stopQuiz() {
  if (ran) {
    std::cout << "Stop quiz." << std::endl;
    std::cout << "Number of questions: " << length << std::endl;
    std::cout << "Number of mistakes: " << mistakes << std::endl;
    std::cout << "Total time spend: " << totalTime() << std::endl;
    std::cout << "Time spend per question: " << totalTime() / length
              << std::endl;

    for (auto &p : correct_answers) {
      std::cout << p.getProblem() << " = " << p.getSolution() << ", "
                << p.getNumberOfTries() << ", " << p.getTimeToSolve()
                << std::endl;
    }
  }
}

std::vector<float> SpeedQuiz::getSpeedData() {
  std::vector<float> speed_data;

  for (auto &p : correct_answers) {
    speed_data.push_back(p.getTimeToSolve());
  }

  return speed_data;
}

float SpeedQuiz::zeroHypothesis(std::string const &csv_path) {
  Statistics speed_stats;
  speed_stats.readSpeedCSV(csv_path);

  auto const ground_truth_data = speed_stats.getSpeedData();
  auto const quiz_speed_data = getSpeedData();

  TwoSampleKSTest<float> ttest(ground_truth_data, quiz_speed_data);

  return ttest.getpValue();
}

void SpeedQuiz::writeResults(std::string filename) {
  if (ran) {
    std::ofstream outfile;

    if (!boost::filesystem::exists(filename.c_str())) {
      outfile.open(filename);
      outfile << "date,nquestions,mistakes,total_time,time/question"
              << std::endl;
    } else {
      outfile.open(filename, std::ios_base::app);
    }

    outfile << getCurrentTime() << "," << length << "," << mistakes << ","
            << totalTime() << "," << totalTime() / length << std::endl;
    outfile.close();
  }
}

void SpeedQuiz::writeResultsPerQuestion(std::string filename) {
  if (ran) {
    std::ofstream outfile;

    if (!boost::filesystem::exists(filename.c_str())) {
      outfile.open(filename);
      outfile << "date,problem,solution,number_of_tries,time_to_solve"
              << std::endl;
    } else {
      outfile.open(filename, std::ios_base::app);
    }

    for (auto &p : correct_answers) {
      outfile << getCurrentTime() << "," << p.getProblem() << ","
              << p.getSolution() << "," << p.getNumberOfTries() << ","
              << p.getTimeToSolve() << std::endl;
    }
    outfile.close();
  }
}
