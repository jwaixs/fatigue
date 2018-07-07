// Copyright 2018 Noud Aldenhoven

#include <string>

#ifndef MEMORY_PROBLEM_H_
#define MEMORY_PROBLEM_H_

class MemoryProblem {
 public:
  explicit MemoryProblem(std::string const p);
  std::string getMemoryProblem();
  unsigned int correctNumber(std::string const answer);

 private:
  std::string memory_problem;
};

#endif  // MEMORY_PROBLEM_H_
