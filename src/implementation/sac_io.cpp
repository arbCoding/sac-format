#include "sac_io.hpp"

// Implementation of the interface in sac_io.hpp

namespace SAC
{
//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
int word_position(const int word_number) { return (word_number * word_length); }

//-----------------------------------------------------------------------------
// Convert integer to binary using two's complement for negatives
//-----------------------------------------------------------------------------
std::bitset<binary_word_size> int_to_binary(const int x)
{
    std::bitset<binary_word_size> bits{};
    if (x >= 0) { bits = std::bitset<binary_word_size>(static_cast<std::size_t>(x)); }
    else { bits = std::bitset<binary_word_size>(static_cast<std::size_t>(std::pow(2, binary_word_size) + x)); }
    return bits;
}
//-----------------------------------------------------------------------------
// End Convert integer to binary using two's complement for negatives
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Convert binary number to integer using two's complement for negatives
//-----------------------------------------------------------------------------
// Check if negative number
// If first number is 1 (true), then number is negative. Otherwise it
// is positive test reads position from right to left (not left to right!)
// hence why it looks like we're reading the final value (for a bitset the
// final value is the left-most value)
// Bitwise not operator flips 0's and 1's
// a) 0001 (+1) -> 1110 (-2)
// c) 1111 (-1) -> 0000 (0)
// Add one
// a) 1110 (-2) -> 1111 (-1)
// c) 0000 (0)  -> 0001 (+1)
int binary_to_int(std::bitset<binary_word_size> x)
{
    int result{};
    if (x.test(binary_word_size - 1))
    {
      x = ~x;
      result = static_cast<int>(x.to_ulong());
      result += 1;
      // Change sign to make it as negative
      result *= -1;
    }
    else { result = static_cast<int>(x.to_ulong()); }
    return result;
}
//-----------------------------------------------------------------------------
// End Convert binary number to integer using two's complement for negatives
//-----------------------------------------------------------------------------

// Union makes this so easy
std::bitset<binary_word_size> float_to_binary(const float x) { return float_to_bits(x).bits; }

float binary_to_float(const std::bitset<binary_word_size> x) { return float_to_bits(x).value; }

// Once again, union to the rescue!
std::bitset<2 * binary_word_size> double_to_binary(const double x) { return double_to_bits(x).bits; }

double binary_to_double(const std::bitset<2 * binary_word_size> x) { return double_to_bits(x).value; }

std::bitset<2 * binary_word_size> string_to_binary(std::string x)
{
    constexpr std::size_t string_size{2 * word_length};
    // 1 byte per character
    constexpr std::size_t char_size{bits_per_byte};
    // String must be of specific length
    // Truncate if needed
    if (x.length() > string_size) { x = x.substr(0, string_size); }
    // Pad if needed
    else if (x.length() < string_size) { x = x.append(string_size - x.length(), ' '); }
    // Two words (8 characters)
    std::bitset<2 * binary_word_size> bits{};
    // 1 character
    std::bitset<char_size> byte{};
    std::size_t character{};
    for (std::size_t i{0}; i < string_size; ++i)
    {
        character = static_cast<std::size_t>(x[i]);
        byte = std::bitset<char_size>(character);
        for (std::size_t j{0}; j < char_size; ++j) { bits[(i * char_size) + j] = byte[j]; }
    }
    return bits;
}

std::string binary_to_string(const std::bitset<2 * binary_word_size> x)
{
    std::string result{};
    constexpr std::size_t char_size{bits_per_byte};
    std::bitset<char_size> byte{};
    // Read character by character
    for (std::size_t i{0}; i < 2 * binary_word_size; i += char_size)
    {
        // Build the character
        for (std::size_t j{0}; j < char_size; ++j) { byte[j] = x[i + j]; }
        result += static_cast<char>(byte.to_ulong());
    }
    // Remove trailing blank spaces
    result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) { return !std::isspace(ch); }).base(), result.end());
    return result;
}

// Same as before, but twice the length
// Cannot use template since compiler cannot decide which function
// to use based upon input (in-case user make string longer than it
// should be)
std::bitset<4 * binary_word_size> long_string_to_binary(std::string x)
{
    constexpr std::size_t string_size{4 * word_length};
    constexpr std::size_t char_size{bits_per_byte};
    // Truncate if needed
    if (x.length() > string_size) { x = x.substr(0, string_size); }
    // Pad if needed
    else if (x.length() < string_size) { x = x.append(string_size - x.length(), ' '); }
    // Four words (16 characters)
    std::bitset<4 * binary_word_size> bits{};
    // One character
    std::bitset<char_size> byte{};
    std::size_t character{};
    for (std::size_t i{0}; i < string_size; ++i)
    {
        character = static_cast<std::size_t>(x[i]);
        byte = std::bitset<char_size>(character);
        for (std::size_t j{0}; j < char_size; ++j) { bits[(i * char_size) + j] = byte[j]; }
    }
    return bits;
}

std::string binary_to_long_string(const std::bitset<4 * binary_word_size> x)
{
    std::string result{};
    constexpr std::size_t char_size{bits_per_byte};
    std::bitset<char_size> byte{};
    // Read character by character
    for (std::size_t i{0}; i < 4 * binary_word_size; i += char_size)
    {
        // It builds character!
        for (std::size_t j{0}; j < char_size; ++j) { byte[j] = x[i + j]; }
        result += static_cast<char>(byte.to_ulong());
    }
    // Remove trailing blank spaces
    result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) { return !std::isspace(ch); }).base(), result.end());
    return result;
}

// SAC uses the right-most value for the bool (little-endian)
// big-endian would be the left-most (read Gulliver's Travels!)
std::bitset<binary_word_size> bool_to_binary(const bool x)
{
    std::bitset<binary_word_size> result{};
    result[0] = x;
    return result;
}

bool binary_to_bool(const std::bitset<binary_word_size> x) { return x[0]; }

std::bitset<2 * binary_word_size> concat_words(const std::bitset<binary_word_size> x, const std::bitset<binary_word_size> y)
{
    std::bitset<2 * binary_word_size> result{};
    for (std::size_t i{0}; i < binary_word_size; ++i) { result[i] = x[i]; result[i + binary_word_size] = y[i]; }
    return result;
}

std::bitset<4 * binary_word_size> concat_words(const std::bitset<2 * binary_word_size> x, const std::bitset<2 * binary_word_size> y)
{
    std::bitset<4 * binary_word_size> result{};
    for (std::size_t i{0}; i < 2 * binary_word_size; ++i) { result[i] = x[i]; result[i + (2 * binary_word_size)] = y[i]; }
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
    constexpr std::size_t char_size{bits_per_byte};
    // Where we will store the characters
    char word[word_length];
    // Read to our character array
    sac->read(word, word_length);
    // Take each character
    std::bitset<char_size> byte{};
    std::size_t character{};
    for (std::size_t i{0}; i < word_length; ++i)
    {
        character = static_cast<std::size_t>(word[i]);
        byte = std::bitset<char_size>(character);
        // bit-by-bit
        for (std::size_t j{0}; j < char_size; ++j) { bits[(i * char_size) + j] = byte[j]; }
    }
    return bits;
}

std::bitset<2 * binary_word_size> read_two_words(std::ifstream* sac)
{
    std::bitset<binary_word_size> word1{read_word(sac)};
    std::bitset<binary_word_size> word2{read_word(sac)};
    if (BYTE_ORDER == LITTLE_ENDIAN) { return concat_words(word1, word2); }
    else { return concat_words(word2, word1); }
}

std::bitset<4 * binary_word_size> read_four_words(std::ifstream* sac)
{
    std::bitset<2 * binary_word_size> word12{read_two_words(sac)};
    std::bitset<2 * binary_word_size> word34{read_two_words(sac)};
    if (BYTE_ORDER == LITTLE_ENDIAN) { return concat_words(word12, word34); }
    else { return concat_words(word34, word12); }
}

std::vector<double> read_data(std::ifstream* sac, const std::size_t n_words, const int start)
{
    sac->seekg(word_position(start));
    std::vector<double> result{};
    result.resize(n_words);
    for (std::size_t i{0}; i < n_words; ++i) { result[i] = static_cast<double>(binary_to_float(read_word(sac))); }
    return result;
}
//-----------------------------------------------------------------------------
// End reading
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
void write_words(std::ofstream* sac_file, const std::vector<char>& input)
{
    std::ofstream& sac = *sac_file;
    if (sac.is_open()) { for (char c : input) { sac.write(&c, sizeof(char)); } }
}

// Template on the typename to make possible to handle float or int
template <typename T>
std::vector<char> convert_to_word(const T x)
{
    char tmp[word_length];
    // Copy bytes from x into the tmp array
    std::memcpy(tmp, &x, word_length);
    std::vector<char> word{};
    word.resize(word_length);
    for (int i{0}; i < word_length; ++i) { word[static_cast<std::size_t>(i)] = tmp[i]; }
    return word;
}

// Explicit instantiation
template std::vector<char> convert_to_word(const float x);
template std::vector<char> convert_to_word(const int x);

std::vector<char> convert_to_word(const double x)
{
    char tmp[2 * word_length];
    // Copy bytes from x into the tmp array
    std::memcpy(tmp, &x, 2 * word_length);
    std::vector<char> word{};
    word.resize(2 * word_length);
    for (int i{0}; i < 2 * word_length; ++i) { word[static_cast<std::size_t>(i)] = tmp[i]; }
    return word;
}

// Variable sized words for the 'K' headers
template <std::size_t N>
std::array<char, N> convert_to_words(const std::string& s, int n_words)
{
    std::array<char, N> all_words;
    // String to null-terminated character array
    const char* c_str = s.c_str();
    for (std::size_t i{0}; i < static_cast<std::size_t>(n_words * word_length); ++i) { all_words[i] = c_str[i]; }
    return all_words;
}

// Explicit instantiation
template std::array<char, word_length> convert_to_words(const std::string& s, int n_words = 1);
template std::array<char, 2 * word_length> convert_to_words(const std::string& s, int n_words = 2);
template std::array<char, 4 * word_length> convert_to_words(const std::string& s, int n_words = 4);

std::vector<char> bool_to_word(const bool b)
{
    std::vector<char> result;
    result.resize(word_length);
    result[0] = b;
    for (std::size_t i{1}; i < word_length; ++i) { result[i] = 0; }
    return result;
}
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------
}
