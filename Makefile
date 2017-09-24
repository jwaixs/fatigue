CXX=clang++
CFLAGS=-I. -std=gnu++14
BOOSTFLAGS=-lboost_filesystem -lboost_system
DEPS=quiz.h problem.h
OBJECTS=quiz.o problem.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJECTS)
	$(CXX) -o $@ $^ main.cpp $(CFLAGS) $(BOOSTFLAGS)

checker: $(OBJECTS)
	$(CXX) -o $@ $^ checker.cpp $(CFLAGS) $(BOOSTFLAGS)
	./checker

clean:
	rm -f *.o main checker
