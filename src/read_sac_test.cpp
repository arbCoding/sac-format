#include "read_sac.hpp"

#include <iostream>
#include <fstream>
#include <array>

int main()
{
  std::ifstream sac_file("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);
  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }

  std::array<char, SAC::word_length> test;
  test = SAC::read_next_word(&sac_file);
  SAC::print_words(test);

  return 0;
}
