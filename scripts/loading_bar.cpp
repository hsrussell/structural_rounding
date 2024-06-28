#include "loading_bar.hpp"

/**
 * Displays a loading bar
 */
void display_loading_bar(int complete, int total)
{
  double progress = (double) complete / (double) total;
  int barWidth = 70;

  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i)
  {
      if (i < pos) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
  }
  std::cout << "] " << (progress * 100.0) << " %\r";
    std::cout.flush();
}
