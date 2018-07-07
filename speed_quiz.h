#ifndef SPEED_QUIZ_H
#define SPEED_QUIZ_H

#include <deque>
#include <string>
#include <vector>

#include "speed_problem.h"

using namespace std;

class SpeedQuiz {
 public:
  SpeedQuiz();
  SpeedQuiz(unsigned int);
  void startQuiz();
  void stopQuiz();
  void writeResults(string);
  void writeResultsPerQuestion(string);
  float zeroHypothesis(string const &);

 private:
  double totalTime();
  void initQuiz(unsigned int);
  deque<SpeedProblem> question;
  deque<SpeedProblem> correct_answers;
  unsigned int mistakes;
  unsigned int length;
  unsigned int start;
  unsigned int stop;
  bool ran;
  vector<float> getSpeedData();
};

#endif  // SPEED_QUIZ_H
