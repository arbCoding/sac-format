#include <iostream>  // std::cout
#include <fstream>   // std::ifstream
#include <array>     // std::array
#include <algorithm> // std::copy()

// SAC uses words that are 4 characters long
static constexpr int word_length{4};

// There is no checking to see if the characters are real or jibberish
// There is no converting to the correct format (string, integer, float)
// Nor is the word being returned to the caller so that it can be passed
// elsewhere

// This is "pass by pointer" (hence the * and the ->)
std::array<char, word_length> read_next_word(std::ifstream* sac)
{
  // If it isn't defined as static, it cannot be returned (will be destroyed on end of function)
  char word[word_length] = {};
  // C-like cast, needs to be re-done with a correct form
  // static_cast<char*>(&word) doesn't work
  sac->read((char*) &word, word_length);
  std::array<char, word_length> result;
  for (int i{0}; i < word_length; ++i)
  {
    result[static_cast<long unsigned int>(i)] = word[i];
  }
  return result;
}

// Read in multiple words
// For some reason if I use int instead of long unsigned int it fails to compile (sign conversion error)
// This happens even when I provide it with an int as input
// As it'll never be negative anyway, I deem it okay to use long unsigned int in this case
template <long unsigned int N> // template for array size
std::array<char, N> read_words(std::ifstream* sac, int n_words = 1)
{
  std::array<char, word_length> single_word;
  std::array<char, N> full_value;
  int destination_start{0};
  for (int words{0}; words < n_words; ++words)
  {
    single_word = read_next_word(sac);
    destination_start = words * word_length;
    std::copy(single_word.begin(), single_word.end(), full_value.begin() + destination_start);
  }
  return full_value;
}

int word_position(int word_number)
{
  return (word_number * word_length);
}

template <long unsigned int N> // another template for array size
void print_array(std::array<char, N> input_array)
{
  for (long unsigned int i{0}; i < sizeof(input_array); i++)
  {
    std::cout << input_array[i] << '\t' << static_cast<int>(input_array[i]) << '\n';
  }
}

// This will read a single word and spit out the raw contents
void test_read(std::ifstream* sac)
{
  std::array<char, word_length> test;
  test = read_next_word(sac);
  print_array(test);
}

// This will read in multiple words (2 of them) and spit out the raw contents
void test_read_two(std::ifstream* sac)
{
  std::array<char, 2 * word_length> test;
  test = read_words<sizeof(test)>(sac, sizeof(test));
  print_array(test);
}

// argc is the argument count, 
// argv[] is the argument vector
// it is an array of pointers to strings
// I really should have an argument parser function that makes sure everything is safe..
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

  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }

  sac_file.seekg(word_position(110)); // KSTNM is 2 words (8 characters long)
  // Reading in a single word works!
  test_read(&sac_file);
  test_read(&sac_file);
  std::cout << '\n' << '\n';
  sac_file.seekg(word_position(110));
  // Reading in two words works!
  test_read_two(&sac_file);
  std::cout << '\n' << '\n';
  // What about reading in 4 words?
  // Yes it works perfectly!
  // Next I need a function to check for the special value of -12345 (unset in SAC)
  sac_file.seekg(word_position(110));
  std::array<char, 4 * word_length> test;
  test = read_words<sizeof(test)>(&sac_file, sizeof(test));
  print_array(test);

  return 0;
}
