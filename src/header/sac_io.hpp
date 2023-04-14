#ifndef SAC_IO_H
#define SAC_IO_H

// This will be used to declare constants
// and to forward declare functions for binary SAC-file In/Out
// Implmentation is elsewhere
// SAC (Seismic Analysis Code) files

// std::cout
#include <iostream>
// std::ifstream (in), std::ofstream (out)
#include <fstream>
// std::array
#include <array>
// std::copy
#include <algorithm>
// std::vector
#include <vector>
// std::memcpy(), std::copy() [strings]
#include <cstring>
// std::bitset
# include <bitset>
// std::pow
# include <cmath>

namespace SAC
{
// Size of data chunks in binary SAC files (called words)
constexpr int word_length{4}; // bytes
constexpr int bits_per_byte{8};
// Each word is 32-bits (4 bytes)
constexpr int binary_word_size{word_length * bits_per_byte};
// First word of time-series
constexpr int data_word{158};
// Values for unset variables
// These are SAC magic values
// I'm setting there here explicitly to conform with the standard
constexpr int unset_int{-12345};
constexpr float unset_float{-12345.0f};
constexpr double unset_double{-12345.0};
constexpr bool unset_bool{0};
const std::string unset_two_words{"-12345  "};
const std::string unset_four_words{"-12345          "};

// Convert word_number to the position (simple, but useful)
int word_position(int word_number);

//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
// Read the next (single) word in the buffer
std::array<char, word_length> read_next_word(std::ifstream* sac);

// Skips the next word, useful for unused headers ('unused' or 'internal' in SAC documentation)
void skip_word(std::ifstream* sac);

// Reads next word as a float
float read_next_float(std::ifstream* sac);

// Reads next word as a double
double read_next_double(std::ifstream* sac);

// Reads next word as integer
int read_next_int(std::ifstream* sac);

// Reads next word as a logical (wasteful format: uses 4 bytes to represent a bool by only using the first and ignoring the other three...)
bool read_next_bool(std::ifstream* sac);

// Read in all the data (assume evenly spaced only)
// Compatible with nvhdr = 6 or 7 (6 is old, 7 is new with footer after data)
// Would need a new function to handle the rare unevenly sampled SAC file (never seen one in the wild before...)
std::vector<float> read_data(std::ifstream* sac, int npts);

// Read the next (n_words) words in the buffer
// Default is 1 word
template <long unsigned int N>
std::array<char, N> read_words(std::ifstream* sac, int n_words = 1);
//-----------------------------------------------------------------------------
// End reading
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Misc
//-----------------------------------------------------------------------------

// Take the words and print them out
template <long unsigned int N>
void print_words(std::array<char, N> words);

// Take a single word vector and print it out
void print_word(std::vector<char> word);

// Take the words and check to see if it has -12345 as it's value (unset)
template <long unsigned int N>
bool is_set(std::array<char, N> words);
//-----------------------------------------------------------------------------
// End misc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
// User defined types that allows the compiler to map the two values to each
// other and perform conversions
// Analogous for integers doesn't seem to work
// Floats to binary and back
union float_to_bits
{
  float value;
  std::bitset<binary_word_size> bits;

  float_to_bits(float x) : value(x){}
  float_to_bits(std::bitset<binary_word_size> binary) : bits(binary){}
};
// Doubles to binary and back
union double_to_bits
{
  double value;
  std::bitset<2 * binary_word_size> bits;

  double_to_bits(double x) : value(x){}
  double_to_bits(std::bitset<2 * binary_word_size> binary) : bits(binary){}
};

// SAC uses 32 bit integers (1 word, 4 bytes)
std::bitset<binary_word_size> int_to_binary(int x);
int binary_to_int(std::bitset<binary_word_size> x);
// SAC uses 32 bit floats (1 word, 4 bytes)
std::bitset<binary_word_size> float_to_binary(float x);
float binary_to_float(std::bitset<binary_word_size> x);
// SAC uses 64 bit floats (2 words, 8 bytes)
std::bitset<2 * binary_word_size> double_to_binary(double x);
double binary_to_double(std::bitset<2 * binary_word_size> x);
// SAC uses either 64 bit strings (2 words, 8 bytes)
std::bitset<2 * binary_word_size> string_to_binary(std::string x);
std::string binary_to_string(std::bitset<2 * binary_word_size> x);
// 128 bit string (4 words, 16 bytes, only KEVNM header)
std::bitset<4 * binary_word_size> long_string_to_binary(std::string x);
std::string binary_to_long_string(std::bitset<4 * binary_word_size> x);
// Shit I forgot this one...
std::bitset<binary_word_size> bool_to_binary(bool x);
bool binary_to_bool(std::bitset<binary_word_size> x);
//-----------------------------------------------------------------------------
// End conversions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
// Using std::vector because more flexible
// Allows writing arbitrary amount of data to file
void write_words(std::ofstream* sac_file, std::vector<char> input);

// Template function to convert to a SAC word
// handles float and int (not string or double)
// Only single word
template <typename T>
std::vector<char> convert_to_word(T x);

// Special for double-precision numbers (2 words, not 1)
std::vector<char> convert_to_word(double x);

// Template function to convert string to SAC word(s)
template <long unsigned int N>
std::array<char, N> convert_to_words(std::string s, int n_words = 1);

// Convert a bool value to a word
std::vector<char> bool_to_word(bool b);
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------

}

#endif
