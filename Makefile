CXX=clang++
CFLAGS=-I. -std=gnu++14
DEPS=quiz.h problem.h
OBJECTS=quiz.o problem.o

%.o: %.cpp $(DEPS)
	    $(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJECTS)
	$(CXX) -o $@ $^ main.cpp $(CFLAGS)

checker: $(OBJECTS)
	$(CXX) -o $@ $^ checker.cpp $(CFLAGS)
	./checker

clean:
	rm *.o main checker
