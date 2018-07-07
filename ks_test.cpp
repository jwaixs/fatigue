#include "ks_test.h"

#include <algorithm>
#include <cmath>
#include <iostream>

template <typename T>
CumulativeFunction<T>::CumulativeFunction(std::vector<T> data) {
  std::sort(data.begin(), data.end());
  auto const N = data.size();
  for (unsigned int i = 0; i < data.size(); i++) {
    cumulative_data[data.at(i)] = static_cast<float>(i + 1) / N;
  }
}

template <typename T>
void CumulativeFunction<T>::printFunction() {
  for (auto const &cd : cumulative_data) {
    std::cout << cd.first << " " << cd.second << std::endl;
  }
}

template <typename T>
float CumulativeFunction<T>::operator()(T const &x) {
  float result = 0.0;
  for (auto const &cd : cumulative_data) {
    if (cd.first > x) {
      break;
    }
    result = cd.second;
  }
  return result;
}

template <typename T>
std::vector<T> CumulativeFunction<T>::getStepPositions() {
  std::vector<T> step_positions;
  for (auto const &cd : cumulative_data) {
    step_positions.push_back(cd.first);
  }
  return step_positions;
}

template <typename T>
unsigned int CumulativeFunction<T>::size() {
  return cumulative_data.size();
}

// Explicitly instantiate the template for CumulativeFunction.
template class CumulativeFunction<float>;
template class CumulativeFunction<double>;
template class CumulativeFunction<unsigned int>;
template class CumulativeFunction<int>;
template class CumulativeFunction<unsigned long>;
template class CumulativeFunction<long>;

template <typename T>
TwoSampleKSTest<T>::TwoSampleKSTest(std::vector<T> const &d1,
                                    std::vector<T> const &d2)
    : cf1(d1), cf2(d2) {
  n1 = cf1.size();
  n2 = cf2.size();
}

template <typename T>
float TwoSampleKSTest<T>::c(float const alpha) {
  return std::sqrt(-0.5 * std::log(alpha / 2));
}

template <typename T>
float TwoSampleKSTest<T>::getDistributionDifference() {
  float max_diff = 0.0;

  for (auto const &x : cf1.getStepPositions()) {
    auto const x_diff = std::abs(cf1(x) - cf2(x));
    if (x_diff > max_diff) {
      max_diff = x_diff;
    }
  }
  for (auto const &x : cf2.getStepPositions()) {
    auto const x_diff = std::abs(cf1(x) - cf2(x));
    if (x_diff > max_diff) {
      max_diff = x_diff;
    }
  }

  return max_diff;
}

template <typename T>
bool TwoSampleKSTest<T>::doesRejectAt(float const alpha) {
  auto const M = std::sqrt(static_cast<float>(n1 + n2) / (n1 * n2));
  auto const C = c(alpha);
  return getDistributionDifference() > C * M;
}

template <typename T>
float TwoSampleKSTest<T>::cinv(float const x) {
  return 2 * std::exp(-2 * x * x);
}

template <typename T>
float TwoSampleKSTest<T>::getpValue() {
  auto const N = std::sqrt(static_cast<float>(n1 * n2) / (n1 + n2));
  auto const D = getDistributionDifference();
  return cinv(D * N);
}

// Explicitly instantiate the template for TwoSampleKSTest.
template class TwoSampleKSTest<float>;
template class TwoSampleKSTest<double>;
template class TwoSampleKSTest<int>;
template class TwoSampleKSTest<unsigned int>;
template class TwoSampleKSTest<long>;
template class TwoSampleKSTest<unsigned long>;
