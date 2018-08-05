// Copyright 2018 Noud Aldenhoven
//
// Chrono and thread are unapproved C++11 libraries. Use them anyway.
//
// cpplint: --filter=-build/c++11

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <tuple>

#include <boost/filesystem.hpp>

#include "./memory_quiz.h"
#include "./tools.h"

MemoryQuiz::MemoryQuiz() { MemoryQuiz(5); }

MemoryQuiz::MemoryQuiz(unsigned int const number_of_problems,
                       unsigned int const sleep_time,
                       unsigned int const nnumbers) {
  sleep_time_in_milliseconds = sleep_time;
  number_of_numbers = nnumbers;
  ran = false;
  initQuiz(number_of_problems);
}

void MemoryQuiz::initQuiz(unsigned int const number_of_problems) {
  std::mt19937::result_type seed = time(0);
  std::uniform_int_distribution<int> distribution(0, 9);
  auto dice = std::bind(distribution, std::mt19937(seed));

  for (unsigned int i = 0; i < number_of_problems; i++) {
    std::string problem;
    for (unsigned int j = 0; j < number_of_numbers; j++) {
      problem += std::to_string(dice());
    }
    MemoryProblem memory_problem(problem);
    question.push_back(memory_problem);
  }
}

void MemoryQuiz::startQuiz() {
  for (auto &problem : question) {
    std::cout << "> " << problem.getMemoryProblem();
    std::cout.flush();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(sleep_time_in_milliseconds));
    std::string blanks(problem.getMemoryProblem().size() + 2, ' ');
    std::cout << "\r" << blanks << "\r> ";

    std::string answer;
    std::cin >> answer;
    unsigned int score = problem.correctNumber(answer);
    answers.push_back(std::tuple<MemoryProblem, std::string, unsigned int>{
        problem, answer, score});
  }

  ran = true;
}

void MemoryQuiz::stopQuiz() {
  std::cout << "memory_problem, answer, correct" << std::endl;
  for (auto &answer : answers) {
    std::cout << std::get<0>(answer).getMemoryProblem() << ", "
              << std::get<1>(answer) << ", " << std::get<2>(answer)
              << std::endl;
  }
}

void MemoryQuiz::writeResultsPerMemoryProblem(std::string const filename) {
  if (ran) {
    std::ofstream outfile;

    if (!boost::filesystem::exists(filename.c_str())) {
      outfile.open(filename);
      outfile << "date,memory_problem,answer,correct" << std::endl;
    } else {
      outfile.open(filename, std::ios_base::app);
    }

    for (auto &answer : answers) {
      outfile << getCurrentTime() << ","
              << std::get<0>(answer).getMemoryProblem() << ","
              << std::get<1>(answer) << "," << std::get<2>(answer) << std::endl;
    }
  }
}
