CXX=clang++
CFLAGS=-I. -std=gnu++14
BOOSTFLAGS=-lboost_filesystem -lboost_system -lboost_program_options
DEPS=quiz.h problem.h stats.h memory_problem.h memory_quiz.h
OBJECTS=quiz.o problem.o stats.o memory_problem.o memory_quiz.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJECTS)
	$(CXX) -o $@ $^ main.cpp $(CFLAGS) $(BOOSTFLAGS)

checker: $(OBJECTS)
	$(CXX) -o $@ $^ checker.cpp $(CFLAGS) $(BOOSTFLAGS)
	./checker

clean:
	rm -f *.o main checker
