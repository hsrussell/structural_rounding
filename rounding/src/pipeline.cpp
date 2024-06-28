#include "../include/pipeline.hpp"

std::vector<std::string> split(const std::string s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

std::string subgraph_filename(int r, std::string filename, std::string folder)
{
  std::vector<std::string> files = split(filename, '/');
  std::stringstream p;
  for (int i = 0; i < files.size()-2; i++)
  {
    if (files[i] != "large-graphs")
    {
      p << files[i] << "/";
    }
    else
    {
      p << "large-graphs/";
      p << folder << "/";
    }
  }
  p << files[files.size()-2] << "-" << r << "/";
  p << folder << "-" << files[files.size()-1];
  p.seekp(-4,p.cur);
  p << "-" << r << ".txt";
  return p.str();
}

void save_editted_graph(Graph *g, int r, std::string filename)
{
  std::string sf = subgraph_filename(r, filename, "edit");
  write_adjacencylist_to_file(sf, g, true);
}

void save_graph_kernel(Graph *g, int r, std::string filename)
{
  std::string sf = subgraph_filename(r, filename, "kernel");
  write_adjacencylist_to_file(sf, g, true);
}

/**
 * Full pipeline
 *
 * Returns the times it takes to:
 *    delete vertices
 *    approximate the IS on the smaller graph
 *    add the vertices back
 */
SolutionIS* IS_edit_approx_lift(
  Graph *g,
  int r,
  double b,
  SolutionIS* soln,
  std::string filename,
  bool save_edit
)
{
  //Step 0: IS approximation on the full graph
  Time t4 = time_now();
  soln->IS_full_approx = IS_approx(g);
  Time t4end = time_now();
  soln->time_IS_full_approx = duration(t4end - t4);
  std::vector<int> vertices = g->get_vertices(false);
  for (auto v : vertices) g->enable_vertex(v);

  //Step 1: Find edit set
  Time t1 = time_now();
  soln->edit_set = local_ratio_recursion(g, r, b);
  Time t1end = time_now();
  soln->time_vertex_deletion = duration(t1end - t1);

  //Step 2: Remove edit set and save editted graph
  for (auto i : soln->edit_set) g->disable_vertex(i);
  if (save_edit) save_editted_graph(g, r, filename);
  soln->actual_k = find_degeneracy(g);

  //Step 3: IS set approx on editted graph
  Time t2 = time_now();
  soln->IS_partial_approx = IS_approx(g);
  Time t2end = time_now();
  soln->time_IS_partial_approx = duration(t2end - t2);

  //Step 4: Add the vertices back and lift the solution
  for (auto i : soln->IS_partial_approx) g->enable_vertex(i);
  Time t3 = time_now();
  soln->IS_lifting_approx
    = vertex_addition_IS(g, soln->edit_set, soln->IS_partial_approx);
  Time t3end = time_now();
  soln->time_lifting = duration(t3end - t3);
  return soln;
}

SolutionDS* DS_edit_approx(
  Graph *g,
  int r,
  SolutionDS* soln,
  std::string filename,
  bool save_edit
)
{
  //Step 0: DS approx on full graph
  Time t4 = time_now();
  soln->DS_full_approx = DS_greedy_approx(g);
  Time t4end = time_now();
  soln->time_DS_full_approx = duration(t4end - t4);
  std::vector<int> vertices = g->get_vertices(false);
  for (auto v : vertices) g->enable_vertex(v);

  //Step 1: edit graph to bounded degeneracy
  Time t1 = time_now();
  soln->edit_set = local_ratio_recursion(g, r, 1);
  Time t1end = time_now();
  soln->time_vertex_deletion = duration(t1end - t1);

  //Step 2: Remove edit set and save editted graph
  for (auto i : soln->edit_set) g->disable_vertex(i);
  if (save_edit) save_editted_graph(g, r, filename);
  soln->actual_k = find_degeneracy(g);

  //Step 3: Find kernel of editted graph and save kernel
  Time t2 = time_now();
  g = DS_poly_kernel(g, soln->DS_full_approx.size());
  Time t2end = time_now();
  soln->time_kernelization = duration(t2end - t2);
  save_graph_kernel(g, r, filename);
  return soln;
}

SolutionDS* DS_lifting(
  Graph *g,
  SolutionDS* soln,
  std::set<int> ds_set
)
{
  soln->DS_partial_approx = ds_set;
  for (auto e : soln->edit_set) g->disable_vertex(e);
  Time t3 = time_now();
  soln->DS_lifting_approx = vertex_addition_DS(g, soln->edit_set, ds_set);
  Time t3end = time_now();
  soln->time_lifting = duration(t3end - t3);
  return soln;
}

/**
 * Edit an existing graph, and make the IS approx
 */
void option_a(int r, std::string filename)
{
  Graph* g = read_adjacencylist_from_file(filename);
  SolutionIS* soln = new SolutionIS();
  std::vector<std::string> path = split(filename, '/');
  soln->filename = path.back();
  soln->n = g->get_n();
  soln->k = find_degeneracy(g);
  soln->r = r;
  soln->e = g->get_edges().size();
  if (g != NULL)
  {
    soln = IS_edit_approx_lift(g, r, 1, soln, filename, true);
    soln->print();
  }
  else std::cout << "Unable to open file";
}

/**
 * Generate a random graph
 */
void option_m(int n, int k, int r, std::string filename)
{
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP(true);
  Graph *g = gf->random_graph_by_cores(n, r, k, filename);
  if (g != NULL)
  {
    if (find_degeneracy(g) != k)
    {
      std::cout << "Error generating graph" << std::endl;
      return;
    }
  }
  else
  {
    std::cerr << n << "-" << k << "-" << r << std::endl;
  }
}

/**
 * Find the degeneracy of a graph
 */
void option_k(std::string filename)
{
  Graph *g = read_adjacencylist_from_file(filename);
  std::cout << find_degeneracy(g) << std::endl;
}

/**
 * Find the IS approximation to a graph
 */
void option_i(std::string filename)
{
  Graph *g = read_adjacencylist_from_file(filename);
  std::set<int> IS = IS_approx(g);
  std::cout << IS.size() << std::endl;
}

/**
 * Find the opt edit set of a graph to degeneracy r
 */
void option_o(int r, std::string filename)
{
  Graph *g = read_adjacencylist_from_file(filename);
  std::set<int> opt_edit = opt_edit_set(
    g,
    r
  );
  for (auto i : opt_edit)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl << opt_edit.size() << std::endl;
}

std::string exec(const char* cmd) {
    std::array<char, 500> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

/**
 * Must have activated cplex-venv
 */
void option_d(int r, std::string filename)
{
  Graph* g = read_adjacencylist_from_file(filename);
  if (g != NULL)
  {
    SolutionDS* soln = new SolutionDS();
    std::vector<std::string> path = split(filename, '/');
    soln->filename = path.back();
    soln->n = g->get_n();
    soln->k = find_degeneracy(g);
    soln->r = r;
    soln->e = g->get_edges().size();
    soln = DS_edit_approx(g, r, soln, filename, true);
    // solve exactly with cplex
    std::stringstream cmd;
    cmd << "python3 ../optimal/solver.py --formulation=DS ";
    cmd << filename;
    std::string ret = exec(cmd.str().c_str());
    // parse cplex output
    std::vector<std::string> tokens = split(ret, ' ');
    std::set<int> ds_set;
    int index = 14;
    while (index < tokens.size())
    {
      ds_set.insert(stoi(tokens[index]));
      index++;
    }
    soln = DS_lifting(g, soln, ds_set);
    soln->print();
  }
  else std::cout << "Unable to open file";
}

/**
 * Find the DS approx without kernelization
 */
void option_p(
  int r,
  std::string filename
)
{
  //read in the full graph
  Graph* g = read_adjacencylist_from_file(filename);
  if (g != NULL)
  {
    SolutionDS* soln = new SolutionDS();
    std::vector<std::string> path = split(filename, '/');
    soln->filename = path.back();
    soln->n = g->get_n();
    soln->k = find_degeneracy(g);
    soln->r = r;
    soln->e = g->get_edges().size();
    //Step 0: DS approx on full graph
    Time t4 = time_now();
    soln->DS_full_approx = DS_greedy_approx(g);
    Time t4end = time_now();
    soln->time_DS_full_approx = duration(t4end - t4);
    std::vector<int> vertices = g->get_vertices(false);
    for (auto v : vertices) g->enable_vertex(v);

    //Step 1: edit graph to bounded degeneracy
    Time t1 = time_now();
    soln->edit_set = local_ratio_recursion(g, r, 1);
    Time t1end = time_now();
    soln->time_vertex_deletion = duration(t1end - t1);

    //Step 2: Remove edit set and save editted graph
    for (auto i : soln->edit_set) g->disable_vertex(i);
    save_editted_graph(g, r, filename);
    soln->actual_k = find_degeneracy(g);

    // solve exactly with cplex
    std::string sf = subgraph_filename(r, filename, "edit");
    std::stringstream cmd;
    cmd << "python3 ../optimal/solver.py --formulation=DS ";
    cmd << sf;
    std::string ret = exec(cmd.str().c_str());
    // parse cplex output
    std::vector<std::string> tokens = split(ret, ' ');
    std::set<int> ds_set;
    int index = 14;
    while (index < tokens.size())
    {
      ds_set.insert(stoi(tokens[index]));
      index++;
    }
    soln = DS_lifting(g, soln, ds_set);
    soln->print();
  }
  else std::cout << "Unable to open file";
}

void option_w(
  std::string filename,
  int exact,
  int r
)
{
  //read in the full graph
  Graph* g = read_adjacencylist_from_file(filename);
  if (g != NULL)
  {
    // edit down the graph
    std::set<int> edit_set = local_ratio_recursion(g, r, 1);
    for (auto i : edit_set)
    {
      g->disable_vertex(i);
    }
    Time t2 = time_now();
    std::set<int> ds_maybe = DS_fpt_approx(g, exact);
    Time t2end = time_now();
    // int kernel_time = duration(t2end - t2);
    if (ds_maybe.size() > 0)
    {
      if (ds_maybe.size() == exact)
      {
        std::cout << "success";
      }
      else
      {
        std::cout << "diff: " << exact << " " << ds_maybe.size();
      }
    }
    else
    {
      std::cout << "no soln no soln";
    }
  }
  else std::cout << "Unable to open file";
}

void option_c(std::string filename)
{
  Graph *g = read_adjacencylist_from_file(filename);
  if (g != NULL)
  {
    std::pair<std::vector<std::set<int>>,std::vector<int>> dg =
      find_degeneracy_order(g);
    std::vector<std::set<int>> cores = dg.first;
    for (auto c : cores)
    {
      for (auto v : c)
      {
        std::cout << v << " ";
      }
      std::cout << std::endl;
    }
  }
  else std::cout << "Unable to open file";
}
