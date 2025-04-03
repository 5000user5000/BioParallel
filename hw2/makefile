# Makefile for HW02 SSW Assignment

CXX = g++
CXXFLAGS = -std=c++17 -O3

# XSIMD include path (請修改為你的實際路徑)
XSIMD_INCLUDE := $(HOME)/Downloads/xsimd/your_install_prefix/include
CXXFLAGS += -I./ -I$(XSIMD_INCLUDE)

SRC = main.cpp align_sw.cpp align_sw_simd.cpp fasta_parser.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = sw_align

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

test: all
	./sw_align seq1.fasta seq2.fasta

clean:
	rm -f $(OBJ) $(TARGET)