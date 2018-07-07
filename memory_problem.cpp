#include "memory_problem.h"

MemoryProblem::MemoryProblem(string const p) { memory_problem = p; }

string MemoryProblem::getMemoryProblem() { return memory_problem; }

unsigned int MemoryProblem::correctNumber(string const answer) {
  unsigned int correct = 0;

  for (unsigned int i = 0; i < min(memory_problem.length(), answer.length());
       i++) {
    if (answer[i] == memory_problem[i]) {
      correct++;
    }
  }

  return correct;
}
