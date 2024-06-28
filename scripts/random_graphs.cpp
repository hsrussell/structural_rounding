#include "loading_bar.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <string>

const std::string graphFolder = "../experiments/generated-graphs";

/**
 * Notation:
 * p = rep number
 * x = number of parameter sets
 * n = number of nodes
 * t = number of k’s for a given n
 * k = degeneracy
 * 0 = zero, marks done
 *
 * input file in the form:
 *
 * The first line is the intger p >= 0 denoting the number of
 * times each set of parameters is used to create a graph, followed
 * by integer x denoting the total number of parameter sets.
 *
 * The next line is two integers n >= 0 and t >= 0, where n is the
 * number of vertices in the graph, and t is the number of cases
 * with different values of k, where k is the degeneracy of the graph
 *
 * In each of the following t lines, the first integer is k,
 * followed by a list of integers for every r, the smallest
 * nonempty core, 1 <= r <= k, wanted in graphs with this n and k
 *
 * The last line is marked by a single 0.
 *
 * Example file:
 * p x
 * n t
 * k1 k2 k3
 * 0
 *
 */

 /**
  * Generates random graphs with a given n, k, and r
  */
int main(int argc, char *argv[])
{
  //compile
  system("make -C ../");
  //remove existing graph files
  system("rm -rf ../experiments/generated-graphs");
  system("mkdir ../experiments/generated-graphs");
  std::ifstream input_file (argv[1]);
  //Assumes properly formatted file
  if (input_file.is_open())
  {
    int rep, total, n;
    input_file >> rep >> total >> n;
    total *= rep;
    int cases = 0;
    int done = 0;
    while (n != 0)
    {
      std::string s;
      std::getline(input_file, s);
      std::getline(input_file, s);
      std::stringstream line(s);
      int k;
      while (line >> k)
      {
        std::stringstream folder;
        folder << "mkdir " << graphFolder << "/" << n << "-" << k;
        system(folder.str().c_str());
        for (int j = 0; j < rep; j++)
        {
          std::stringstream filename;
          filename << folder.str() << "/";
          filename << n << "-" << k << "-" << j << "-graph.txt";
          std::stringstream command;
          command << "../rounding/output -m" << " " << n << " " << k << " " << 1;
          command << filename.str();
          system(command.str().c_str());
          done++;
          display_loading_bar(done, total);
        }
      }
      input_file >> n;
    }
  }

  return 0;
}
