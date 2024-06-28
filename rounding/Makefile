SRC_FILES = src/graph/graph.cpp\
			src/graph/vertex.cpp\
			src/degeneracy.cpp\
			src/graphFactoryGMP.cpp\
			src/graphFactoryMatrixGMP.cpp\
			src/independentSet.cpp\
			src/dominatingSet.cpp\
			src/io.cpp\
			src/vertexDeletion.cpp\
			src/optEditSet.cpp\
			src/solutionIS.cpp\
			src/solutionDS.cpp\
			src/pipeline.cpp\
			src/main.cpp
TEST_FILES = tests/testGraph.cpp\
			 tests/testDegeneracy.cpp\
			 tests/testGraphFactoryGMP.cpp\
			 tests/testGraphFactoryMatrixGMP.cpp\
			 tests/testIndependentSet.cpp\
			 tests/testDominatingSet.cpp\
			 tests/testVertexDeletion.cpp\
			 tests/testOptEditSet.cpp\
			 src/graph/graph.cpp\
			 src/graph/vertex.cpp\
			 src/graph/testUtil.cpp\
			 src/degeneracy.cpp\
			 src/graphFactoryGMP.cpp\
			 src/graphFactoryMatrixGMP.cpp\
			 src/independentSet.cpp\
			 src/dominatingSet.cpp\
			 src/io.cpp\
			 src/vertexDeletion.cpp\
			 src/optEditSet.cpp\
			 src/solutionIS.cpp\
			 src/solutionDS.cpp\
			 src/pipeline.cpp
OUTPUT = test-rounding
CPP_VERSION = -std=c++17 -pg -O3
COMPILE_OPTIONS = -g -pthread -lstdc++fs -lgmp
CATCH = tests/catch2/
GMPXX = ../../../gmp-6.1.2/
TEST_MAIN = tests/tests.cpp

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
GCC = g++-8
endif

ifeq ($(UNAME), Linux)
GCC = g++
endif

all: clean graph

clean:
	rm -f *.o

graph:
	$(GCC) $(CPP_VERSION) -I$(GMPXX) $(SRC_FILES) -o $(OUTPUT) $(COMPILE_OPTIONS)

test_prep:
	$(GCC) $(CPP_VERSION) -I$(CATCH) -c $(TEST_MAIN)
	$(GCC) $(CPP_VERSION) -I$(GMPXX) -I$(CATCH) -o tests/tests tests.o $(TEST_FILES) $(COMPILE_OPTIONS)
