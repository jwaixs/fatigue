#ifndef KS_TEST_H
#define KS_TEST_H

#include <vector>
#include <map>

class CumulativeFunction {
public:
    CumulativeFunction(std::vector<float>);
    void printFunction();
    float operator()(float const &);
    std::vector<float> getStepPositions();
private:
    std::map<float, float> cumulative_data;
};

class TwoSampleKSTest {
public:
    TwoSampleKSTest(std::vector<float> const &, std::vector<float> const &);
    float c(float const);
    float cinv(float const);
    float getDistributionDifference();
    bool doesRejectAt(float const);
    float getpValue();
private:
    CumulativeFunction cf1, cf2;
    unsigned int n1, n2;
};

#endif /* KS_TEST_H */
