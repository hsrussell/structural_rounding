#include "../rounding/include/pipeline.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

const std::string graphFolderPath = "../experiments/large-graphs/";
const std::string output = "output.txt";

/**
 * Splits a string by a given delimiter
 */
std::vector<std::string> splits(const std::string& s, char delimiter)
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

void run_is()
{
  //file format
  std::cout << "file,";
  std::cout << "n,k,r,e,actual_k,";
  std::cout << "time_vertex_deletion,";
  std::cout << "time_IS_partial_approx,";
  std::cout << "time_lifting,";
  std::cout << "time_IS_full_approx,";
  std::cout << "edit_set,edit_set_size,";
  std::cout << "IS_partial_approx,IS_partial_approx_size,";
  std::cout << "IS_lifting_approx,IS_lifting_approx_size,";
  std::cout << "IS_full_approx,IS_full_approx_size";

  std::cout << std::endl;
  system("rm -r ../experiments/large-graphs/edit");
  system("mkdir ../experiments/large-graphs/edit");
  //for every folder
  for (auto & p : fs::directory_iterator(graphFolderPath))
  {
    std::vector<std::string> folder = splits(p.path().string(), '/');
    if (folder.back().compare("edit"))
    {
      std::vector<std::string> params = splits(folder[3], '-');
      for (int i = 1; i <= stoi(params[1]); i++)
      {
        std::stringstream cmd;
        cmd << "mkdir ../experiments/large-graphs/edit/" << folder[3] << "-" << i;
        system(cmd.str().c_str());
        //for every file in the folder
        for (auto & pp : fs::directory_iterator(p.path().string()))
        {
          std::stringstream command;
          std::vector<std::string> files = splits(pp.path().string(), '/');
          std::cout << files[4] << "," << std::flush;
          command << "../rounding/output -a" << " " << i << " " << pp;
          system(command.str().c_str());
        }
      }
    }
  }
}

void run_ps()
{
  //file format
  std::cout << "file,";
  std::cout << "n,k,r,e,actual_k,";
  std::cout << "time_vertex_deletion,";
  std::cout << "time_kernelization,";
  std::cout << "time_lifting,";
  std::cout << "time_DS_full_approx,";
  std::cout << "edit_set,edit_set_size,";
  std::cout << "DS_partial_approx,DS_partial_approx_size,";
  std::cout << "DS_lifting_approx,DS_lifting_approx_size,";
  std::cout << "DS_full_approx,DS_full_approx_size";

  std::cout << std::endl;
  system("rm -r ../experiments/large-graphs/edit");
  system("mkdir ../experiments/large-graphs/edit");
  //for every folder
  for (auto & p : fs::directory_iterator(graphFolderPath))
  {
    std::vector<std::string> folder = splits(p.path().string(), '/');
    if (folder.back().compare("edit"))
    {
      std::vector<std::string> params = splits(folder[3], '-');
      int rMax = std::min(10, stoi(params[1]));
      for (int i = 1; i <= rMax; i++)
      {
        std::stringstream cmd;
        cmd << "mkdir ../experiments/large-graphs/edit/" << folder[3] << "-" << i;
        system(cmd.str().c_str());
        //for every file in the folder
        for (auto & pp : fs::directory_iterator(p.path().string()))
        {
          std::stringstream command;
          std::vector<std::string> files = splits(pp.path().string(), '/');
          // std::cout << files[4] << "," << std::flush;
          command << "../rounding/test-rounding -p" << " " << i << " " << pp;
          system(command.str().c_str());
        }
      }
    }
  }
}

void run_ds()
{
  //file format
  std::cout << "file,";
  std::cout << "n,k,r,e,actual_k,";
  std::cout << "time_vertex_deletion,";
  std::cout << "time_kernelization,";
  std::cout << "time_lifting,";
  std::cout << "time_DS_full_approx,";
  std::cout << "edit_set,edit_set_size,";
  std::cout << "DS_partial_approx,DS_partial_approx_size,";
  std::cout << "DS_lifting_approx,DS_lifting_approx_size,";
  std::cout << "DS_full_approx,DS_full_approx_size";

  std::cout << std::endl;
  system("rm -r ../experiments/large-graphs/edit");
  system("mkdir ../experiments/large-graphs/edit");
  system("rm -r ../experiments/large-graphs/kernel");
  system("mkdir ../experiments/large-graphs/kernel");
  //for every folder
  for (auto & p : fs::directory_iterator(graphFolderPath))
  {
    std::vector<std::string> folder = splits(p.path().string(), '/');
    if (folder.back().compare("edit"))
    {
      std::vector<std::string> params = splits(folder[3], '-');
      int rMax = std::min(10, stoi(params[1]));
      for (int i = 1; i <= rMax; i++)
      {
        std::stringstream cmd;
        cmd << "mkdir ../experiments/large-graphs/kernel/" << folder[3] << "-" << i;
        cmd << "mkdir ../experiments/large-graphs/edit/" << folder[3] << "-" << i;
        system(cmd.str().c_str());
        //for every file in the folder
        for (auto & pp : fs::directory_iterator(p.path().string()))
        {
          std::stringstream command;
          std::vector<std::string> files = splits(pp.path().string(), '/');
          // std::cout << files[4] << "," << std::flush;
          command << "../rounding/test-rounding -d" << " " << i << " " << pp;
          system(command.str().c_str());
        }
      }
    }
  }
}

/**
 * Run all graphs through the pipeline
 */
int main(int argc, char *argv[])
{
  //compile
  system("make -C ../rounding");
  // Independent Set
  if (strcmp(argv[1], "-a") == 0)
  {
    run_is();
  }
  // Dominating Set
  else if (strcmp(argv[1], "-d") == 0)
  {
    run_ds();
  }
  // Dominating Set
  else if (strcmp(argv[1], "-p") == 0)
  {
    run_ps();
  }
  else
  {
    std::cout << "\nUsage: ./solve_all [-options]\n";
  }
  return 0;
}
