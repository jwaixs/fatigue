// Copyright 2018 Noud Aldenhoven

#include <algorithm>

#include "./memory_problem.h"

MemoryProblem::MemoryProblem(std::string const p) { memory_problem = p; }

std::string MemoryProblem::getMemoryProblem() { return memory_problem; }

unsigned int MemoryProblem::correctNumber(std::string const answer) {
  unsigned int correct = 0;

  for (unsigned int i = 0;
       i < std::min(memory_problem.length(), answer.length()); i++) {
    if (answer[i] == memory_problem[i]) {
      correct++;
    }
  }

  return correct;
}
