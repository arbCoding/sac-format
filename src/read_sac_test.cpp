#include "read_sac.hpp"

#include <iostream>
#include <fstream>
#include <array>
#include <cstring> // std::memcpy()

// TODO
// Read in entire header
// Parse values to see if set (not = -12345)
// Convert to a sac object?
// Handle writing out a sac file?

int main()
{
  std::ifstream sac_file("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);
  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }

  std::array<char, 2 * SAC::word_length> test;
  sac_file.seekg(SAC::word_position(110));
  test = SAC::read_words<sizeof(test)>(&sac_file, 2);
  //SAC::print_words(test);
  test = SAC::read_words<sizeof(test)>(&sac_file, 2);
  test = SAC::read_words<sizeof(test)>(&sac_file, 2);
  test = SAC::read_words<sizeof(test)>(&sac_file, 2);
  //SAC::print_words(test);
  //const bool is_set{SAC::is_set(test)};
  //std::cout << '\n' << is_set << '\n';
  // Data begins at word 158 and has NPTS words total (if evenly sampled)
  //
  // IT WORKS!!!!
  //
  std::vector data{SAC::read_data(&sac_file)};
  //for (long unsigned int i{0}; i < data.size(); ++i)
  for (long unsigned int i{0}; i < 25; ++i)
  {
    std::cout << data[i] << '\n';
  }

  return 0;
}
