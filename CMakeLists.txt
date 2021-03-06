cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

project (fatigue)
set (fatigue_VERSION_MAJOR 0)
set (fatigue_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(Boost REQUIRED COMPONENTS program_options system filesystem
                                       date_time unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_library(tools tools.cpp ${Boost_LIBRARIES})
add_library(ks_test ks_test.cpp)
add_library(memory_problem memory_problem.cpp)
add_library(memory_quiz memory_quiz.cpp)
target_link_libraries(memory_quiz memory_problem tools ${Boost_LIBRARIES})
add_library(speed_problem speed_problem.cpp)
add_library(speed_quiz speed_quiz.cpp)
target_link_libraries(speed_quiz speed_problem ${Boost_LIBRARIES} ks_test)
add_library(stats stats.cpp)
target_link_libraries(stats sqlite3)

add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES} memory_quiz speed_quiz stats)

add_executable(csv2sqlite3 csv2sqlite3.cpp)
target_link_libraries(csv2sqlite3 ${Boost_LIBRARIES} sqlite3)

add_executable(tests tests.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} memory_problem speed_problem
  stats tools ks_test)

enable_testing()
add_test(tests tests)
