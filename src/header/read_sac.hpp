#ifndef READ_SAC_H
#define READ_SAC_H

// This will be used to declare constants
// and to forward declare functions for reading
// SAC (Seismic Analysis Code) files

// std::cout
#include <iostream>
// std::ifstream
#include <fstream>
// std::array
#include <array>
// std::copy
#include <algorithm>

namespace SAC
{
constexpr int word_length{4};

// Convert word_number to the position (simple, but useful)
int word_position(int word_number);

// Read the next (single) word in the buffer
std::array<char, word_length> read_next_word(std::ifstream* sac);

// Read the next (n_words) words in the buffer
template <long unsigned int N>
std::array<char, N> read_words(std::ifstream* sac, int n_words = 1);

// Take the words and print them out
template <long unsigned int N>
void print_words(std::array<char, N> words);

}

#endif
