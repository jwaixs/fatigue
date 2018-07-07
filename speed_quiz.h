// Copyright 2018 Noud Aldenhoven

#ifndef SPEED_QUIZ_H_
#define SPEED_QUIZ_H_

#include <deque>
#include <string>
#include <vector>

#include "./speed_problem.h"

class SpeedQuiz {
 public:
  SpeedQuiz();
  explicit SpeedQuiz(unsigned int);
  void startQuiz();
  void stopQuiz();
  void writeResults(std::string);
  void writeResultsPerQuestion(std::string);
  float zeroHypothesis(std::string const &);

 private:
  double totalTime();
  void initQuiz(unsigned int);
  std::deque<SpeedProblem> question;
  std::deque<SpeedProblem> correct_answers;
  unsigned int mistakes;
  unsigned int length;
  unsigned int start;
  unsigned int stop;
  bool ran;
  std::vector<float> getSpeedData();
};

#endif  // SPEED_QUIZ_H_
