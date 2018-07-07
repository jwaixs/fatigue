// Copyright 2018 Noud Aldenhoven

#ifndef KS_TEST_H_
#define KS_TEST_H_

#include <map>
#include <vector>

template <typename T>
class CumulativeFunction {
 public:
  explicit CumulativeFunction(std::vector<T>);
  void printFunction();
  float operator()(T const &);
  std::vector<T> getStepPositions();
  unsigned int size();

 private:
  std::map<T, float> cumulative_data;
};

template <typename T>
class TwoSampleKSTest {
 public:
  TwoSampleKSTest(std::vector<T> const &, std::vector<T> const &);
  float c(float const);
  float cinv(float const);
  float getDistributionDifference();
  bool doesRejectAt(float const);
  float getpValue();

 private:
  CumulativeFunction<T> cf1, cf2;
  unsigned int n1, n2;
};

#endif  // KS_TEST_H_
