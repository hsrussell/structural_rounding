#ifndef pipeline_hpp
#define pipeline_hpp

#include "../include/independentSet.hpp"
#include "../include/dominatingSet.hpp"
#include "../include/io.hpp"
#include "../include/degeneracy.hpp"
#include "../include/vertexDeletion.hpp"
#include "../include/graphFactoryGMP.hpp"
#include "../include/graphFactoryMatrixGMP.hpp"
#include "../include/solutionIS.hpp"
#include "../include/solutionDS.hpp"
#include "../include/optEditSet.hpp"

//requires c++17
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

typedef std::chrono::high_resolution_clock::time_point Time;
#define duration(dt) std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count()
#define time_now() std::chrono::high_resolution_clock::now()
namespace fs = std::filesystem;
const std::string graphFolder = "large-graphs/";

std::vector<std::string> split(const std::string s, char delimiter);

/**
 * Full experiment for IS
 *
 * Step 0: IS approx on the full graph
 * Step 1: Find edit set
 * Step 2: Remove edit set and save editted graph
 * Step 3: IS set approx on editted graph
 * Step 4: Add the vertices back and lift the solution
 */
SolutionIS* IS_edit_approx_lift(
  Graph *g,
  int r,
  double b,
  SolutionIS* soln,
  std::string filename,
  bool save_edit = false
);

/**
 * Full approx, edit, and kernelization for DS
 *
 * Step 0: DS approx on the full graph
 * Step 1: Find edit set
 * Step 2: Remove edit set and save editted graph
 * Step 3: Find kernel of editted graph and save kernel
 */
SolutionDS* DS_edit_approx(
  Graph *g,
  int r,
  SolutionDS* soln,
  std::string filename = NULL,
  bool save_edit = false
);

/**
 * Lifting for DS
 *
 * Step 0: DS approx on the full graph
 * Step 1: Find edit set
 * Step 2: Remove edit set and save editted graph
 * Step 3: Find kernel of editted graph and save kernel
 */
SolutionDS* DS_lifting(
  Graph *g,
  SolutionDS* soln,
  std::set<int> ds_set
);

void option_a(int r, std::string filename);

void option_m(int n, int k, int r, std::string filename);

void option_k(std::string filename);

void option_i(std::string filename);

void option_o(int r, std::string filename);

void option_d(int r, std::string filename);

void option_c(std::string filename);

/**
 * Temp
 */
void option_p(
  int r,
  std::string filename
);

void option_w(
  std::string filename,
  int exact,
  int r
);

#endif
