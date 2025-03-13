CXX = g++
CXXFLAGS = -std=c++17 -pthread -O2


MAIN_OBJ = main.o matrix.o
TEST_OBJ = test.o thread_pool.o

all: main test

# execute main (Part I)
check: main
	./main

# execute test (Part II)
runtest: test
	./test

main: $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f main test *.o