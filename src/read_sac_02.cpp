#include <iostream>
#include <fstream>
#include <array>

// SAC uses words that are 4 characters long
static constexpr int word_length{4};

// Currently this can read a single word and spit out all the characters
// in that word
// There is no checking to see if the characters are real or jibberish
// There is no converting to the correct format (string, integer, float)
// Nor is the word being returned to the caller so that it can be passed
// elsewhere
// This is "pass by pointer" (hence the * and the ->)
std::array<char, word_length> read_next_word(std::ifstream* sac)
{
  // If it isn't defined as static, it cannot be returned (will be destroyed on end of function)
  char word[word_length] = {};
  sac->read((char*) &word, word_length);
  std::array<char, word_length> result;
  for (int i{0}; i < word_length; ++i)
  {
    result[static_cast<long unsigned int>(i)] = word[i];
  }
  return result;
}

int word_position(int word_number)
{
  return (word_number * word_length);
}

// argc is the argument count, 
// argv[] is the argument vector
// it is an array of pointers to strings
int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    std::cout << argv[1] << '\n';
  }
  else
  {
    std::cerr << "You must provide a single input file to be read.\n";
    return 1;
  }
  std::ifstream sac_file(argv[1], std::ifstream::binary);
  //std::ifstream sac_file("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);
  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }
  sac_file.seekg(word_position(110)); // KSTNM is 2 words (8 characters long)
  std::array<char, word_length> test;
  test = read_next_word(&sac_file);
  //test = read_next_word(&sac_file);
  for (long unsigned int i{0}; i < sizeof(test); ++i)
  {
    std::cout << test[i] << '\t' << static_cast<int>(test[i]) <<  '\n';
  }
  std::cout << '\n';

  // It works! We can keep reading subsequent words!
  // Next is to defined a funciton that will take an argument for the
  // number of words to read and return that as a character array
  test = read_next_word(&sac_file);
  for (long unsigned int i{0}; i < sizeof(test); ++i)
  {
    std::cout << test[i] << '\t' << static_cast<int>(test[i]) << '\n';
  }

  return 0;
}
