GG_FILES = random_graphs.cpp\
		  loading_bar.cpp
GG_OUTPUT = generate

SOLVER_FILES = solver_script.cpp\
			  loading_bar.cpp
SOLVER_OUTPUT = solve_all

CPP_VERSION = -std=c++17 -pg
COMPILE_OPTIONS = -g -pthread -lstdc++fs -lgmp
GMPXX = ../../../gmp-6.1.2/

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
GCC = g++-8
endif

ifeq ($(UNAME), Linux)
GCC = g++
endif

all: clean graph_generation solver

clean:
	rm -f *.o

graph_generation:
	$(GCC) $(CPP_VERSION) -I$(GMPXX) $(GG_FILES) -o $(GG_OUTPUT) $(COMPILE_OPTIONS)

solver:
	$(GCC) $(CPP_VERSION) -I$(GMPXX) $(SOLVER_FILES) -o $(SOLVER_OUTPUT) $(COMPILE_OPTIONS)
