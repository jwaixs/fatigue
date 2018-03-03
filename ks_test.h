#ifndef KS_TEST_H
#define KS_TEST_H

#include <vector>
#include <map>

class CumulativeFunction {
public:
    CumulativeFunction(std::vector<float> data);
    void printFunction();
    float operator()(float const &x);
    std::vector<float> getStepPositions();
private:
    std::map<float, float> cumulative_data;
};

#endif /* KS_TEST_H */
