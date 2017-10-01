#include <string>
using namespace std;

#ifndef MEMORY_H
#define MEMORY_H

class MemoryProblem {
public:
    MemoryProblem(string const p);
    string getMemoryProblem();
    unsigned int correctNumber(string const answer);
private:
    string memory_problem;
};

#endif /* MEMORY_H */
