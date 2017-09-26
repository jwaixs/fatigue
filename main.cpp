#include <iostream>
#include <string>
using namespace std;

#include <boost/program_options.hpp>
using namespace boost::program_options;

#include "quiz.h"
#include "stats.h"

int main(int argc, char **argv) {
    string result_csv;
    string result_per_question_csv;
    unsigned int number_of_questions;

    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help usage.")
        ("stats,s", "Display statistics of quizes.")
        ("result-csv,f", value(&result_csv)->required(), "Result csv output file.")
        ("result-per-question-csv,g", value(&result_per_question_csv)->required(), "Result per question csv output file.")
        ("number-of-questions,n", value(&number_of_questions)->default_value(5), "Number of questions. (default: 5)");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
        cout << "Progam to measure your fatigueness doing a small quiz."
            << endl << desc;
        return 0;
    }

    if (vm.count("stats")) {
        Statistics stats(result_per_question_csv);
        return 0;
    }

    Quiz quiz(number_of_questions);
    cout << "Press enter to start the quiz.";
    cin.get();
    quiz.startQuiz();
    quiz.stopQuiz();
    quiz.writeResults(result_csv);
    quiz.writeResultsPerQuestion(result_per_question_csv);
    return 0; 
}
