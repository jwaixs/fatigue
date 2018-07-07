// Copyright 2018 Noud Aldenhoven

#include <string>
#include <tuple>
#include <vector>

#include "./memory_problem.h"

#ifndef MEMORY_QUIZ_H_
#define MEMORY_QUIZ_H_

class MemoryQuiz {
 public:
  MemoryQuiz();
  MemoryQuiz(unsigned int const number_of_problems,
             unsigned int const sleep_time = 1000,
             unsigned int const nnumbers = 10);
  void startQuiz();
  void stopQuiz();
  void writeResultsPerMemoryProblem(std::string);

 private:
  void initQuiz(unsigned int const);
  std::vector<MemoryProblem> question;
  std::vector<std::tuple<MemoryProblem, std::string, unsigned int>> answers;
  unsigned int sleep_time_in_milliseconds;
  unsigned int number_of_numbers;
  bool ran;
};

#endif  // MEMORY_QUIZ_H_
