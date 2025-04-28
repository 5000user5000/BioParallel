# Makefile for HW3 SSW Assignment (with CUDA)

CXX = g++
NVCC = nvcc
CXXFLAGS = -std=c++17 -O3
NVCCFLAGS = -std=c++17 -O3

# XSIMD include path (請修改為你的實際路徑)
XSIMD_INCLUDE := $(HOME)/Downloads/xsimd/your_install_prefix/include
CXXFLAGS += -I./ -I$(XSIMD_INCLUDE)
NVCCFLAGS += -I./ -I$(XSIMD_INCLUDE)

# Source files
CPP_SRC = main.cpp align_sw.cpp align_sw_simd.cpp fasta_parser.cpp
CU_SRC = align_sw_cuda.cu  # CUDA source

OBJ = $(CPP_SRC:.cpp=.o) $(CU_SRC:.cu=.o)
TARGET = sw_align

all: $(TARGET)

$(TARGET): $(OBJ)
	$(NVCC) $(NVCCFLAGS) -o $@ $^

# 分開規則編譯 .cpp 跟 .cu
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

test: all
	./sw_align seq1.fasta seq2.fasta

clean:
	rm -f $(OBJ) $(TARGET)
