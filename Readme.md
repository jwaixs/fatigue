# Fatigue

A tool to measure and register fatigue level of a person, after evaluating a
simple quiz.

[![Build Status](https://travis-ci.org/noudald/fatigue.svg?branch=master)](https://travis-ci.org/noudald/fatigue)

## Getting Started


### Installing

Clone the repository
```
git clone https://github.com/jwaixs/fatigue.git
```
Create a build dir and compile the source code with your favorite compile. I
would compile fatigue with Ninja:
```
mkdir fatigue/build && cd fatigue/build
cmake -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release ..
ninja -j4
```

### Running the tests

To run the unit tests please run
```
./build/tests
```

### Run fatigue

Fatigue has a couple of options. You can run a speed quiz (`--problem-quiz`)
or a memory quiz (`--memory-quiz`). Results are written to a csv file defined
in `--result-per-question-csv`. Afterwards you can do statistics on the
result csv using the `--stats` option. For example
```
> ./build/main --problem-quiz --result-per-question-csv ./speed-results.csv

Press enter to start the problem quiz.
5 + 7 > 12
4 + 8 > 12
0 + 9 > 9
6 + 0 > 6
2 + 3 > 5
Stop quiz.
Number of questions: 5
Number of mistakes: 0
Total time spend: 10.124
Time spend per question: 2.0248
5 + 7 = 12, 1, 2.21
4 + 8 = 12, 1, 1.859
0 + 9 = 9, 1, 1.718
6 + 0 = 6, 1, 2.379
2 + 3 = 5, 1, 1.958
```
After some time you will have enough information to apply statistics. Below
are some results I obtained after tracking my speed results for three months.
```
> ./build/main --result-per-question-csv ./speed-results.csv --stats
Speed problem histogram (num of tries):
0.658000 - 0.800800: ------------| (69)
0.800800 - 0.943600: -----------------------------------------------| (260)
0.943600 - 1.086400: ----------------------------------------------------| (290)
1.086400 - 1.229200: ------------------------------------------------------------| (331)
1.229200 - 1.372000: --------------------------------------------| (246)
1.372000 - 1.514800: --------------------------| (147)
1.514800 - 1.657600: ---------------| (83)
1.657600 - 1.800400: ------------| (68)
1.800400 - 1.943200: ------| (38)
1.943200 - 2.086000: ---------| (50)
2.086000 - 2.228800: -------| (39)
2.228800 - 2.371600: ---| (21)
2.371600 - 2.514400: ---| (22)
2.514400 - 2.657200: ---| (19)
2.657200 - 2.800000: --| (13)
2.800000 - 2.942800: ---| (17)
```
The speed per hour and per day
```
> ./build-main --result-per-question-csv ./speed-results.csv --stats \
               --display-median

Speed problem per hour (median / num of tries):
8 :00-9 :00: -------------------------------------------------------------| (1.39645/20)
9 :00-10:00: -------------------| (1.27435/195)
10:00-11:00: | (1.21948/355)
11:00-12:00: --------| (1.24062/280)
12:00-13:00: -----------| (1.25032/145)
13:00-14:00: ------------| (1.25335/240)
14:00-15:00: ------------------| (1.27079/150)
15:00-16:00: ----------------| (1.26405/165)
16:00-17:00: ---------------------------------------------| (1.34994/170)
20:00-21:00: | (0/10)
21:00-22:00: | (0/10)

Speed problem per day (median / num of tries):
Mon: --| (1.23163/325)
Tue: ------| (1.24867/405)
Wed: ----------------| (1.29553/415)
Thu: | (1.22502/260)
Fri: --------| (1.25639/270)
Sat: ------------------------------------------------------------| (1.49076/30)
Sun: ----------------------------------| (1.37355/40)
```
It is planned to have more statistical tools in the future.

## Contributing

If you want to contribute to this project please contact me. Any help is
appreciated.

### Code format and style

All code is formatted with clang-format, based on the Google c++ style
guide:
```
clang-format -assume-filename=.clang-format -i *.cpp *.h
```
We use Google's cpplint for code styling. Cpplint should not produce any
errors. It is possible to disable some errors locally using
`check_style.sh`.

## License

This project is licensed under the MIT License - see the LICENSE file for
details.
