#ifndef SAC_IO_H
#define SAC_IO_H

// This will be used to declare constants
// and to forward declare functions for reading
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
// Size of data chunks in SAC file (SAC calls them 'words')
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

// Skips the next word, useful for unused headers
void skip_word(std::ifstream* sac);

// Reads next word as a float
float read_next_float(std::ifstream* sac);

// Reads next word as integer
int read_next_int(std::ifstream* sac);

// Reads next word as a logical (wasteful format)
bool read_next_bool(std::ifstream* sac);

// Read in all the data (assume evenly spaced only)
// This assume the format is 6 (nvhdr = 6)
// Compatible with nvhdr = 6 or 7 (6 is old, 7 is new with footer after data)
std::vector<float> read_data(std::ifstream* sac, int npts);

// Read the next (n_words) words in the buffer
template <long unsigned int N>
std::array<char, N> read_words(std::ifstream* sac, int n_words = 1);
//-----------------------------------------------------------------------------
// End reading
//-----------------------------------------------------------------------------


// Take the words and print them out
template <long unsigned int N>
void print_words(std::array<char, N> words);

// Take the words and check to see if it has -12345 as it's value (unset)
template <long unsigned int N>
bool is_set(std::array<char, N> words);

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
// Write the next single word to the file
void write_next_word(std::ofstream* sac_file, std::array<char, word_length> input);
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------

}

#endif
