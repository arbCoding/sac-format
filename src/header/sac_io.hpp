#ifndef SAC_IO_HPP_20230619
#define SAC_IO_HPP_20230619

//-----------------------------------------------------------------------------
// Include Statements
//-----------------------------------------------------------------------------
// String comparisons in C++ suck, boost adds needed functionality!
#include <boost/algorithm/string.hpp>
// Standard Library stuff, https://en.cppreference.com/w/cpp/standard_library
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
//-----------------------------------------------------------------------------
// End Include Statements
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Description
//-----------------------------------------------------------------------------
// This will be used to declare constants
// and to forward declare functions for binary SAC-file In/Out
// Implmentation is elsewhere
// SAC (Seismic Analysis Code) files
//-----------------------------------------------------------------------------
// End Description
//-----------------------------------------------------------------------------

namespace SAC
{
//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
// Size of data chunks in binary SAC files (called words)
constexpr int word_length{4}; // bytes
constexpr int bits_per_byte{8}; // binary character size
// Each word is 32-bits (4 bytes)
constexpr int binary_word_size{word_length * bits_per_byte};
// First word of (first) data-section
constexpr int data_word{158};
// Values for unset variables. These are SAC magic values
// I'm setting there here explicitly to conform with the standard
// Ignoring Clang unused variable for these as they are an integral
// part of the SAC data standard
constexpr int unset_int{-12345};
constexpr float unset_float{-12345.0f};
constexpr double unset_double{-12345.0};
constexpr bool unset_bool{false};
// This should work for two and four word string headers
const std::string unset_word{"-12345"};
//-----------------------------------------------------------------------------
// End constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
// You tell it what word you want, it'll return the position in the sac-file
// Use with the seekg() command to get to that position in the file
int word_position(const int word_number);
// These unions work like magic. I assume the system uses what it considers
// the most natural way to map these to each other (uses the system endianness)
//
// User defined types that allow the compiler to map the two values to each
// other and perform conversions
// Floats to binary and back

// Ignoring SonarLint S6147 because we're not using the union to actually store data
// just to do a conversion. Encapsulating in a class is needless overhead in this case.
union float_to_bits
{
    float value;
    std::bitset<binary_word_size> bits;

    explicit float_to_bits(float x) : value(x){}
    explicit float_to_bits(std::bitset<binary_word_size> binary) : bits(binary){}
};
// Doubles to binary and back
union double_to_bits
{
    double value;
    std::bitset<2 * binary_word_size> bits;

    explicit double_to_bits(double x) : value(x){}
    explicit double_to_bits(std::bitset<2 * binary_word_size> binary) : bits(binary){}
};
// Why no union int_to_bits? Tried it, didn't work, had to do something else.

// SAC uses 32 bit integers (1 word, 4 bytes, 1 byte = 1 character)
std::bitset<binary_word_size> int_to_binary(const int x);
int binary_to_int(std::bitset<binary_word_size> x);
// SAC uses 32 bit floats (1 word, 4 bytes)
std::bitset<binary_word_size> float_to_binary(const float x);
float binary_to_float(const std::bitset<binary_word_size> x);
// SAC uses 64 bit doubles (2 words, 8 bytes)
std::bitset<2 * binary_word_size> double_to_binary(const double x);
double binary_to_double(const std::bitset<2 * binary_word_size> x);
// Remove leading/trailing white-space and control characters
std::string string_cleaning(const std::string& str);
// Note the string conversion functions handle over-sized strings
// by truncating them, and undersized strings by padding them with spaces
// SAC uses either 64 bit strings (2 words, 8 bytes, 8 characters)
std::bitset<2 * binary_word_size> string_to_binary(std::string x);
std::string binary_to_string(const std::bitset<2 * binary_word_size> x);
// 128 bit string (4 words, 16 bytes, only KEVNM header, 16 characters)
std::bitset<4 * binary_word_size> long_string_to_binary(std::string x);
std::string binary_to_long_string(const std::bitset<4 * binary_word_size> x);
// Booleans
std::bitset<binary_word_size> bool_to_binary(const bool x);
bool binary_to_bool(const std::bitset<binary_word_size> x);
// Concat words
// For some reason, template functions didn't want to work for these...
std::bitset<2 * binary_word_size> concat_words(const std::bitset<binary_word_size> x, const std::bitset<binary_word_size> y);
std::bitset<4 * binary_word_size> concat_words(const std::bitset<2 * binary_word_size> x, const std::bitset<2 * binary_word_size> y);
//-----------------------------------------------------------------------------
// End conversions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
// Can read 1, 2, or 4 words and return as a binary bitset
// Conversion functions are then used to do the conversions
std::bitset<binary_word_size> read_word(std::ifstream* sac);
std::bitset<2 * binary_word_size> read_two_words(std::ifstream* sac);
std::bitset<4 * binary_word_size> read_four_words(std::ifstream* sac);
// Can read any number of words into a vector of doubles
// Useful for data values
std::vector<double> read_data(std::ifstream* sac, const std::size_t n_words, const int start = data_word);
//-----------------------------------------------------------------------------
// End reading
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
// The below writing functions all work, though they are a bit funky
// Using std::vector because more flexible
// Allows writing arbitrary amount of data to file
void write_words(std::ofstream* sac_file, const std::vector<char>& input);
// Template function to convert to a SAC word
// handles float and int (not string or double)
// Only single word
template <typename T>
std::vector<char> convert_to_word(const T x);
// Special for double-precision numbers (2 words, not 1)
std::vector<char> convert_to_word(const double x);
// Template function to convert string to SAC word(s)
template <std::size_t N>
std::array<char, N> convert_to_words(const std::string& s, int n_words = 1);
// Convert a bool value to a word
std::vector<char> bool_to_word(const bool b);
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------
}
#endif
