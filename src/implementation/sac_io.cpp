#include "sac_io.hpp"

// Implementation of the interface in sac_io.hpp

namespace SAC
{

int word_position(int word_number)
{
  return (word_number * word_length);
}

//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
std::bitset<binary_word_size> int_to_binary(int x)
{
  std::bitset<binary_word_size> bits{};
  if (x >= 0)
  {
    bits = std::bitset<binary_word_size>(static_cast<long long unsigned int>(x));
  }
  else
  {
    // 2's compliment
    bits = std::bitset<binary_word_size>(std::pow(2, binary_word_size) + x);
  }
  // Convert to bitset<word_length>
  return bits;
}

int binary_to_int(std::bitset<binary_word_size> x)
{
  int result{};
  // Check if negative number
  // First number is 1 (true), number is negative
  // otherwise it is positive
  // test reads position from right to left (not left to right!)
  // hence why it looks like we're reading the final value
  // (for a bitset the final value is the left-most value)
  if (x.test(binary_word_size - 1))
  {
    // Bitwise not operator flips 0's and 1's
    // -1 -> 6 in for a 4-bit integer
    // e.g. 1001 -> 0110
    x = ~x;
    // Convert back to an integer
    result = x.to_ulong();
    // Add 1 to complete the complement portion
    result += 1;
    // Changed sign to denote it as negative
    result *= -1;
  }
  else
  {
    result = x.to_ulong();
  }
  return result;
}

// Union makes this so easy
std::bitset<binary_word_size> float_to_binary(float x)
{
  return float_to_bits(x).bits;
}

float binary_to_float(std::bitset<binary_word_size> x)
{
  return float_to_bits(x).value;
}

// Once again, union to the rescue!
std::bitset<2 * binary_word_size> double_to_binary(double x)
{
  return double_to_bits(x).bits;
}

double binary_to_double(std::bitset<2 * binary_word_size> x)
{
  return double_to_bits(x).value;
}

std::bitset<2 * binary_word_size> string_to_binary(std::string x)
{
  constexpr long unsigned int string_size{2 * word_length};
  // 1 byte per character
  constexpr long unsigned int char_size{bits_per_byte};
  // x is being passed by value, not reference, so it shouldn't be
  // altered outside of the function
  // String must be of specific length
  if (x.length() > string_size)
  {
    // Truncate if needed
    x = x.substr(0, string_size);
  }
  else if (x.length() < string_size)
  {
    // Pad if needed
    x = x.append(string_size - x.length(), ' ');
  }
  // Two words (8 characters)
  std::bitset<2 * binary_word_size> bits{};
  // 1 character
  std::bitset<char_size> byte{};
  long unsigned int character{};
  for (long unsigned int i{0}; i < string_size; ++i)
  {
    character = static_cast<long unsigned int>(x[i]);
    byte = std::bitset<char_size>(character);
    for (long unsigned int j{0}; j < char_size; ++j)
    {
      bits[(i * char_size) + j] = byte[j];
    }
  }
  return bits;
}

std::string binary_to_string(std::bitset<2 * binary_word_size> x)
{
  std::string result{};
  constexpr long unsigned int char_size{bits_per_byte};
  std::bitset<char_size> byte{};
  // Read character by character
  for (long unsigned int i{0}; i < 2 * binary_word_size; i += char_size)
  {
    // Build the character
    for (long unsigned int j{0}; j < char_size; ++j)
    {
      byte[j] = x[i + j];
    }
    result += static_cast<char>(byte.to_ulong());
  }
  return result;
}

std::bitset<4 * binary_word_size> long_string_to_binary(std::string x)
{
  // Same as before, but twice the length
  // Cannot use template since compiler cannot decide which function
  // to use based upon input (in-case user make string longer than it
  // should be)
  constexpr long unsigned int string_size{4 * word_length};
  constexpr long unsigned int char_size{bits_per_byte};
  if (x.length() > string_size)
  {
    // Truncate if needed
    x = x.substr(0, string_size);
  }
  else if (x.length() < string_size)
  {
    // Pad if needed
    x = x.append(string_size - x.length(), ' ');
  }
  // Four words (16 characters)
  std::bitset<4 * binary_word_size> bits{};
  // One character
  std::bitset<char_size> byte{};
  long unsigned int character{};
  for (long unsigned int i{0}; i < string_size; ++i)
  {
    character = static_cast<long unsigned int>(x[i]);
    byte = std::bitset<char_size>(character);
    for (long unsigned int j{0}; j < char_size; ++j)
    {
      bits[(i * char_size) + j] = byte[j];
    }
  }
  return bits;
}

std::string binary_to_long_string(std::bitset<4 * binary_word_size> x)
{
  std::string result{};
  constexpr long unsigned int char_size{bits_per_byte};
  std::bitset<char_size> byte{};
  // Read character by character
  for (long unsigned int i{0}; i < 4 * binary_word_size; i += char_size)
  {
    // It builds character!
    for (long unsigned int j{0}; j < char_size; ++j)
    {
      byte[j] = x[i + j];
    }
    result += static_cast<char>(byte.to_ulong());
  }
  return result;
}

// SAC uses the right-most value for the bool
std::bitset<binary_word_size> bool_to_binary(bool x)
{
  std::bitset<binary_word_size> result{};
  result[0] = x;
  return result;
}

bool binary_to_bool(std::bitset<binary_word_size> x)
{
  return x[0];
}

std::bitset<2 * binary_word_size> concat_words(std::bitset<binary_word_size> x, std::bitset<binary_word_size> y)
{
  std::bitset<2 * binary_word_size> result{};
  for (size_t i{0}; i < binary_word_size; ++i)
  {
    result[i] = x[i];
    result[i + binary_word_size] = y[i];
  }
  return result;
}

std::bitset<4 * binary_word_size> concat_words(std::bitset<2 * binary_word_size> x, std::bitset<2 * binary_word_size> y)
{
  std::bitset<4 * binary_word_size> result{};
  for (size_t i{0}; i < 2 * binary_word_size; ++i)
  {
    result[i] = x[i];
    result[i + (2 * binary_word_size)] = y[i];
  }
  return result;
}

//-----------------------------------------------------------------------------
// End conversions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
std::bitset<binary_word_size> read_word(std::ifstream* sac)
{
  std::bitset<binary_word_size> bits{};
  constexpr long unsigned int char_size{bits_per_byte};
  // Where we will store the characters
  // Dynamic memory allocation (make sure to delete)
  char words[word_length];
  // Read to our buffer
  sac->read(words, word_length);
  // Take each character
  std::bitset<char_size> byte{};
  long unsigned int character{};
  for (long unsigned int i{0}; i < sizeof(words); ++i)
  {
    character = static_cast<long unsigned int>(words[i]);
    byte = std::bitset<char_size>(character);
    // bit-by-bit
    for (long unsigned int j{0}; j < char_size; ++j)
    {
      bits[(i * char_size) + j] = byte[j];
    }
  }
  return bits;
}

std::bitset<2 * binary_word_size> read_two_words(std::ifstream* sac)
{
  return concat_words(read_word(sac), read_word(sac));
}

std::bitset<4 * binary_word_size> read_four_words(std::ifstream* sac)
{
  return concat_words(read_two_words(sac), read_two_words(sac));
}

std::vector<float> read_data(std::ifstream* sac, size_t n_words, int start)
{
  sac->seekg(word_position(start));
  std::vector<float> result{};
  result.resize(n_words);
  for (size_t i{0}; i < n_words; ++i)
  {
    result[i] = binary_to_float(read_word(sac));
  }
  return result;
}
//-----------------------------------------------------------------------------
// End reading
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Misc
//-----------------------------------------------------------------------------
template <long unsigned int N>
void print_words(std::array<char, N> words)
{
  for (long unsigned int i{0}; i < sizeof(words); ++i)
  {
    std::cout << words[i] << '\t' << static_cast<int>(words[i]) << '\n'; 
  }
}

// Explicit instantiation of template to make linker/compiler behave!
template void print_words(std::array<char, word_length> words);
template void print_words(std::array<char, 2 * word_length> words);
template void print_words(std::array<char, 4 * word_length> words);

void print_word(std::vector<char> word)
{
  // Foreach style loop to make simpler
  for (char c : word)
  {
    std::cout << static_cast<int>(c) << '\n';
  }
}

// Seems to work
// Checks to see if the value is -12345 (characters)
// Which is the SAC special value for unset
template <long unsigned int N>
bool is_set(std::array<char, N> words)
{
  // Need to search for the -12345 pattern
  for (long unsigned int i{0}; i < sizeof(words); ++i)
  {
    if (words[i] == '-')
    {
      return false;
    }
  }
  return true;
}

// Explicit instantiation of template to make linker/compiler behave!
template bool is_set(std::array<char, word_length> words);
template bool is_set(std::array<char, 2 * word_length> words);
template bool is_set(std::array<char, 4 * word_length> words);
//-----------------------------------------------------------------------------
// End misc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
void write_words(std::ofstream* sac_file, std::vector<char> input)
{
  std::ofstream& sac = *sac_file;
  if (sac.is_open())
  {
    // Foreach-loop for funzies instead of usual for-loop
    for (char c : input)
    {
      //sac.write(c, sizeof(char)); // cannot initialize
      sac.write((char*) &c, sizeof(char));
    }
  }
}

// Template on the typename to make possible to handle float or int
template <typename T>
std::vector<char> convert_to_word(T x)
{
  char tmp[word_length];
  // Copy bytes from x into the tmp array
  std::memcpy(tmp, &x, word_length);
  std::vector<char> word{};
  word.resize(word_length);
  for (int i{0}; i < 4; ++i)
  {
    word[static_cast<long unsigned int>(i)] = tmp[i];
  }
  return word;
}

// Explicit instantiation
template std::vector<char> convert_to_word(float x);
template std::vector<char> convert_to_word(int x);

std::vector<char> convert_to_word(double x)
{
  char tmp[2 * word_length];
  // Copy bytes from x into the tmp array
  std::memcpy(tmp, &x, 2 * word_length);
  std::vector<char> word{};
  word.resize(2 * word_length);
  for (int i{0}; i < 4; ++i)
  {
    word[static_cast<long unsigned int>(i)] = tmp[i];
  }
  return word;
}

// Veriable sized words for the 'K' headers
template <long unsigned int N>
std::array<char, N> convert_to_words(std::string s, int n_words)
{
  std::array<char, N> all_words;
  // String to null-terminated character array
  const char* c_str = s.c_str();
  for (int i{0}; i < (n_words * word_length); ++i)
  {
    all_words[static_cast<long unsigned int>(i)] = static_cast<int>(c_str[i]);
  }
  return all_words;
}

// Explicit instantiation
template std::array<char, word_length> convert_to_words(std::string s, int n_words = 1);
template std::array<char, 2 * word_length> convert_to_words(std::string s, int n_words = 2);
template std::array<char, 4 * word_length> convert_to_words(std::string s, int n_words = 4);

std::vector<char> bool_to_word(bool b)
{
  std::vector<char> result;
  result.resize(4);
  result[0] = b;
  for (long unsigned int i{1}; i < 4; ++i)
  {
    result[i] = 0;
  }
  return result;
}
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------

}
