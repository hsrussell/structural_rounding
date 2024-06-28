#include "../include/solutionDS.hpp"
#include <sstream>

SolutionDS::SolutionDS() { }

void SolutionDS::print() {
  std::stringstream line;
  line << filename << ",";
  line << n << ",";
  line << k << ",";
  line << r << ",";
  line << e << ",";
  line << actual_k << ",";
  line << time_vertex_deletion << ",";
  line << time_kernelization << ",";
  line << time_lifting << ",";
  line << time_DS_full_approx << ",";
  line << set_to_csv(edit_set) << ",";
  line << edit_set.size() << ",";
  line << set_to_csv(DS_partial_approx) << ",";
  line << DS_partial_approx.size() << ",";
  line << set_to_csv(DS_lifting_approx) << ",";
  line << DS_lifting_approx.size() << ",";
  line << set_to_csv(DS_full_approx) << ",";
  line << DS_full_approx.size();
  std::cout << line.str() << std::endl;
}

std::string SolutionDS::set_to_csv(std::set<int> s)
{
  std::stringstream ss;
  ss << "\"[";
  for (auto element : s) ss << "'" << element << "', ";
  if (s.size() != 0) ss.seekp(-2,ss.cur);
  ss << "]\"";
  return ss.str();
}
