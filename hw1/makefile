CXX = g++
CXXFLAGS = -std=c++17 -pthread -O2

all: main

check: main
	./main

test: test

# main: main.o matrix.o thread_pool.o
# 	$(CXX) $(CXXFLAGS) main.o matrix.o thread_pool.o -o main

# test: test.o matrix.o thread_pool.o
# 	$(CXX) $(CXXFLAGS) test.o matrix.o thread_pool.o -o test
# 	./test

main: main.o matrix.o 
	$(CXX) $(CXXFLAGS) main.o matrix.o  -o main

test: test.o matrix.o
	$(CXX) $(CXXFLAGS) test.o matrix.o -o test
	./test


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main test *.o
