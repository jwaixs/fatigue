#include "ks_test.h"

#include <algorithm>
#include <iostream>

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
