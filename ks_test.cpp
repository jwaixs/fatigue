#include "ks_test.h"

#include <iostream>
#include <algorithm>
#include <cmath>

CumulativeFunction::CumulativeFunction(std::vector<float> data) {
    std::sort(data.begin(), data.end());
    auto const N = data.size();
    for (unsigned int i = 0; i < data.size(); i++) {
        cumulative_data[data.at(i)] = static_cast<float>(i + 1) / N;
    }
}

void CumulativeFunction::printFunction() {
    for (auto const &cd : cumulative_data) {
        std::cout << cd.first << " " << cd.second << std::endl;
    }
}

float CumulativeFunction::operator()(float const &x) {
    float result = 0.0;
    for (auto const &cd : cumulative_data) {
        if (cd.first > x) {
            break;
        }
        result = cd.second;
    }
    return result;
}

std::vector<float> CumulativeFunction::getStepPositions() {
    std::vector<float> step_positions;
    for (auto const &cd : cumulative_data) {
        step_positions.push_back(cd.first);
    }
    return step_positions;
}


TwoSampleKSTest::TwoSampleKSTest(
        std::vector<float> const &d1,
        std::vector<float> const &d2) : cf1(d1), cf2(d2) {
    n1 = d1.size();
    n2 = d2.size();
}

float TwoSampleKSTest::c(float const alpha) {
    return std::sqrt(-0.5 * std::log(alpha / 2));
}

float TwoSampleKSTest::getDistributionDifference() {
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

bool TwoSampleKSTest::doesRejectAt(float const alpha) {
    auto const M = std::sqrt(static_cast<float>(n1 + n2) / (n1*n2));
    auto const C = c(alpha);
    return getDistributionDifference() > C*M;
}

float TwoSampleKSTest::cinv(float const x) {
    return 2*std::exp(-2*x*x);
}

float TwoSampleKSTest::getpValue() {
    auto const N = std::sqrt(static_cast<float>(n1*n2) / (n1 + n2));
    auto const D = getDistributionDifference();
    return cinv(D*N);
}
