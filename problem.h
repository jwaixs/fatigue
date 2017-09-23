#include <string>

using namespace std;

class Problem {
public:
    Problem(string const, string const);
    string getProblem();
    string getSolution();
    bool isCorrect(string const);
private:
    string problem;
    string solution;
};
