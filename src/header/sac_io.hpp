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
// std::memcpy()
#include <cstring>

namespace SAC
{
// Size of data chunks in binary SAC files (called words)
constexpr int word_length{4};
// First word of time-series
constexpr int data_word{158};

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
// Writing
//-----------------------------------------------------------------------------
// Write the next single word to the file
// This doesn't need to be limited to a single word...
// Would be easy to do an arbitrary number of characters in a vector
// Trade-off: arrays are faster than vectors, but vectors are more flexible
void write_next_word(std::ofstream* sac_file, std::array<char, word_length> input);

// Template function to convert to a SAC word
// handles float and int (not string)
template <typename T>
std::vector<char> convert_to_word(T x);

// Template function to convert string to SAC word(s)
// will need to be able to handle multiple words
template <long unsigned int N>
std::array<char, N> convert_to_words(std::string s, int n_words = 1);
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------

}

#endif
