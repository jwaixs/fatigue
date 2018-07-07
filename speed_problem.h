// Copyright 2018 Noud Aldenhoven

#include <string>

#ifndef SPEED_PROBLEM_H_
#define SPEED_PROBLEM_H_

class SpeedProblem {
 public:
  SpeedProblem(std::string const, std::string const);
  std::string getProblem();
  std::string getSolution();
  bool isCorrect(std::string const);
  void addTime(double);
  unsigned int getNumberOfTries();
  double getTimeToSolve();

 private:
  std::string problem;
  std::string solution;
  unsigned int number_of_tries;
  double time_to_solve;
};

#endif  // SPEED_PROBLEM_H_
