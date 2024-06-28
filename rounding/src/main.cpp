#include "../include/pipeline.hpp"

//requires c++17
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

/**
 * Print a usage message
 */
void usage()
{
  std::cout << "\nUsage: ./test-rounding [-options] [args...]\n";
  std::cout << "Where options include:\n";

  std::cout << "    -a <r> <filename>\n";
  std::cout << "        Run the full pipeline\n\n";

  std::cout << "    -m <n> <k> <r> <filename>\n";
  std::cout << "        Generate a random graph\n\n";

  std::cout << "    -k <filename>\n";
  std::cout << "        Find the degeneracy\n\n";

  std::cout << "    -i <filename>\n";
  std::cout << "        Find the IS approximation\n\n";

  std::cout << "    -o <filename> <r>\n";
  std::cout << "        Find the optimal edit set\n\n";

  std::cout << "    -d <filename> <r>\n";
  std::cout << "        Find the DS approximation\n\n";

  std::cout << "    -help\n";
  std::cout << "        Print this help message\n\n";
}

//main
int main(int argc, char *argv[])
{
  //edit an existing graph, and make the IS approx
  if (strcmp(argv[1], "-a") == 0)
  {
    option_a(atoi(argv[2]), argv[3]);
  }
  //generate a random graph
  else if (strcmp(argv[1], "-m") == 0)
  {
    option_m(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
  }
  //find the degeneracy of a graph
  else if (strcmp(argv[1], "-k") == 0)
  {
    option_k(argv[2]);
  }
  //find the IS approx
  else if (strcmp(argv[1], "-i") == 0)
  {
    option_i(argv[2]);
  }
  //find the opt edit set
  else if (strcmp(argv[1], "-o") == 0)
  {
    option_o(atoi(argv[2]), argv[3]);
  }
  // find the DS approx
  else if (strcmp(argv[1], "-d") == 0)
  {
    option_d(atoi(argv[2]), argv[3]);
  }
  // find the cores of the graph
  else if (strcmp(argv[1], "-c") == 0)
  {
    option_c(argv[2]);
  }
  // temp skip kernelization
  else if (strcmp(argv[1], "-p") == 0)
  {
    option_p(atoi(argv[2]), argv[3]);
  }
  // temp check kernelization
  else if (strcmp(argv[1], "-w") == 0)
  {
    option_w(argv[2], atoi(argv[3]), atoi(argv[4]));
  }
  //print a usage message
  else if (strcmp(argv[1], "-help") == 0)
  {
    usage();
  }
  return 0;
}
