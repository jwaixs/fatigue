cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
project (fatigue)

find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_library(tools tools.cpp)
add_library(memory_problem memory_problem.cpp)
add_library(memory_quiz memory_quiz.cpp)
target_link_libraries(memory_quiz memory_problem tools ${Boost_LIBRARIES})
add_library(problem problem.cpp)
add_library(quiz quiz.cpp)
target_link_libraries(quiz problem)
add_library(stats stats.cpp)

add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES} memory_quiz quiz stats)
