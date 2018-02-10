#include <iostream>
#include <string>
using namespace std;

#include <boost/program_options.hpp>
using namespace boost::program_options;

#include "quiz.h"
#include "memory_quiz.h"
#include "stats.h"

int main(int argc, char **argv) {
    string result_csv;
    string result_per_question_csv;
    unsigned int number_of_questions;
    bool do_problem_quiz, do_memory_quiz;

    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help usage.")
        ("stats,s", "Display statistics of quizes.")
        ("result-csv,f", value(&result_csv), "Result csv output file.")
        ("result-per-question-csv,g", value(&result_per_question_csv),
            "Result per question csv output file.")
        ("number-of-questions,n", value(&number_of_questions)->default_value(5),
            "Number of questions. (default: 5)")
        ("problem-quiz,p", bool_switch(&do_problem_quiz)->default_value(false),
            "Do the problem quiz.")
        ("memory-quiz,m", bool_switch(&do_memory_quiz)->default_value(false),
            "Do the memory quiz.");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help") || vm.count("h")) {
        cout << "Progam to measure your fatigueness doing a small quiz."
            << endl << desc;
        return 0;
    }

    if (vm.count("stats") || vm.count("s")) {
        Statistics stats(result_per_question_csv);

        std::cout << "Speed problem histogram:" << std::endl;
        stats.printProblemMeanHistogram();

        return 0;
    }

    if (do_problem_quiz && do_memory_quiz) {
        cout << "Cannot do problem quiz and memory quiz at the same time."
            << " Please select only one of the two options." << endl;
    }

    if (do_problem_quiz) {
        Quiz quiz(number_of_questions);
        cout << "Press enter to start the problem quiz.";
        cin.get();
        quiz.startQuiz();
        quiz.stopQuiz();

        if (vm.count("results-csv") > 0 || vm.count("f") > 0) {
            quiz.writeResults(result_csv);
        } else {
            cout << "result-csv is not set, not saving results." << endl;
        }

        if (vm.count("result-per-question-csv") > 0 || vm.count("g") > 0) {
            quiz.writeResultsPerQuestion(result_per_question_csv);
        } else {
            cout << "result-per-question-csv is not set, not saving results." << endl;
        }
    }

    if (do_memory_quiz) {
        MemoryQuiz memory_quiz(number_of_questions, 1000);
        cout << "Press enter to start the memory quiz.";
        cin.get();
        memory_quiz.startQuiz();
        memory_quiz.stopQuiz();

        if (vm.count("result-per-question-csv") > 0 || vm.count("g") > 0) {
            memory_quiz.writeResultsPerMemoryProblem(result_per_question_csv);
        } else {
            cout << "result-per-question-csv is not set, not saving results." << endl;
        }
    }

    return 0;
}
