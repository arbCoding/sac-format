#include "sac_io.hpp"

#include <iostream>
#include <string>
#include <fstream>

int main(int arg_count, char *arg_array[])
{
  if (arg_count != 2)
  {
    std::printf("Usage: %s <file>\n", arg_array[0]);
    return 1;
  }
  std::string file_name = arg_array[1];
  std::cout << "File:\t\t" << file_name << '\n';
  // Open it in binary mode
  std::ifstream file(file_name, std::ios::binary);
  // Make sure it didn't fail
  if (!file)
  {
    std::cerr << "File could not be read...\n";
  }
  // Be sure we're at the beginning of the file
  file.seekg(0);
  // Our containers for the binary data
  std::bitset<SAC::binary_word_size> one_word{};
  std::bitset<2 * SAC::binary_word_size> two_words{};
  std::bitset<4 * SAC::binary_word_size> four_words{};
  one_word = SAC::read_word(&file);
  std::cout << "f_Delta:\t" <<  SAC::binary_to_float(one_word) << '\n';

  // Word position of first integer value
  file.seekg(SAC::word_position(70));

  // Should be the first integer value
  one_word = SAC::read_word(&file);
  std::cout << "NZYear:\t\t" << SAC::binary_to_int(one_word) << '\n';

  // Word position of first logical value
  file.seekg(SAC::word_position(105));
  one_word = SAC::read_word(&file);
  std::cout << "LEven:\t\t" << SAC::binary_to_bool(one_word) << '\n';

  // Word position of first two word value
  file.seekg(SAC::word_position(110));
  two_words = SAC::read_two_words(&file);
  std::cout << "KStNm:\t\t" << SAC::binary_to_string(two_words) << '\n';

  // Word position of only four word value
  file.seekg(SAC::word_position(112));
  four_words = SAC::read_four_words(&file);
  std::cout << "KEvNm:\t\t" << SAC::binary_to_long_string(four_words) << '\n';

  return 0;
}
