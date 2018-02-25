cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
project (fatigue)

find_package(Boost REQUIRED COMPONENTS program_options system filesystem
                                       date_time unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_library(tools tools.cpp ${Boost_LIBRARIES})
add_library(memory_problem memory_problem.cpp)
add_library(memory_quiz memory_quiz.cpp)
target_link_libraries(memory_quiz memory_problem tools ${Boost_LIBRARIES})
add_library(speed_problem speed_problem.cpp)
add_library(speed_quiz speed_quiz.cpp)
target_link_libraries(speed_quiz speed_problem ${Boost_LIBRARIES})
add_library(stats stats.cpp)

add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES} memory_quiz speed_quiz stats)

add_executable(tests tests.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} memory_problem speed_problem stats tools)
