#include "sac_io.hpp"

#include <iostream>
#include <string>
#include <fstream>

int main()
{
  std::string file_name = "../data/IM.NV31..BHZ.M.2023.094.222304.SAC";
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
  [[maybe_unused]] std::bitset<SAC::binary_word_size> one_word{};
  [[maybe_unused]] std::bitset<2 * SAC::binary_word_size> two_words{};
  [[maybe_unused]] std::bitset<4 * SAC::binary_word_size> four_words{};
  one_word = SAC::read_word(&file);
  std::cout << "f_Delta:\t" <<  SAC::binary_to_float(one_word) << '\n';

  // Word position of first integer value
  file.seekg(SAC::word_position(70));

  // Should be the first integer value
  one_word = SAC::read_word(&file);
  std::cout << "NZYear:\t" << SAC::binary_to_int(one_word) << '\n';

  // Word position of first logical value
  file.seekg(SAC::word_position(105));
  one_word = SAC::read_word(&file);
  std::cout << "LEven:\t" << SAC::binary_to_bool(one_word) << '\n';

  // Word position of first two word value
  file.seekg(SAC::word_position(110));
  //std::bitset<SAC::binary_word_size> word1{SAC::read_word(&file)};
  //std::bitset<SAC::binary_word_size> word2{SAC::read_word(&file)};
  //two_words = SAC::concat_words(word1, word2);
  two_words = SAC::read_two_words(&file);
  std::cout << "KStNm:\t" << SAC::binary_to_string(two_words) << '\n';

  // Word position of only four word value
  file.seekg(SAC::word_position(112));
  //std::bitset<2 * SAC::binary_word_size> word3{SAC::concat_words(SAC::read_word(&file), SAC::read_word(&file))};
  //std::bitset<2 * SAC::binary_word_size> word4{SAC::concat_words(SAC::read_word(&file), SAC::read_word(&file))};
  //four_words = SAC::concat_words(word3, word4);
  four_words = SAC::read_four_words(&file);
  std::cout << "KEvNm:\t" << SAC::binary_to_long_string(four_words) << '\n';

  return 0;
}
