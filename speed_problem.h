#include <string>

#ifndef SPEED_PROBLEM_H
#define SPEED_PROBLEM_H

using namespace std;

class SpeedProblem {
 public:
  SpeedProblem(string const, string const);
  string getProblem();
  string getSolution();
  bool isCorrect(string const);
  void addTime(double);
  unsigned int getNumberOfTries();
  double getTimeToSolve();

 private:
  string problem;
  string solution;
  unsigned int number_of_tries;
  double time_to_solve;
};

#endif  // SPEED_PROBLEM_H
