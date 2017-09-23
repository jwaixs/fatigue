CXX=clang++
CFLAGS=-I.
DEPS=quiz.h problem.h
OBJECTS=quiz.o problem.o

%.o: %.cpp $(DEPS)
	    $(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJECTS)
	$(CXX) -o $@ $^ main.cpp $(CFLAGS)

clean:
	rm *.o main
