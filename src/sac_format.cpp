// Copyright 2023-2024 Alexander R. Blanchette

/*!
  \file sac_format.cpp

  \brief Implementation of the sac-format library.

  \author Alexander R. Blanchette

  The full implementation of the entire sac-format library. Including
  the Trace class, all methods, and all functions. Everything in this file
  is targeted for testing coverage.
  */

#include "sac-format/sac_format.hpp"

// Implementation of the interface in sac_format.hpp
namespace sacfmt {
//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
/*!
  \brief Calculates position of word in SAC-file.

  Multiplies given word number by the word-length in bytes (defined by the SAC
  format.)

  @param[in] word_number Number of desired word in file stream.
  @returns std::streamoff Position in SAC-file of desired word (in bytes).
  */
std::streamoff word_position(const size_t word_number) noexcept {
  return static_cast<std::streamoff>(word_number * word_length);
}

/*!
  \brief Convert unsigned integer to 32-bit (one word) binary bitset.

  This sets the current bit using bitwise and, updates the bit to manipulate
  and performs a right-shift (division by 2) until the number is zero.

  @param[in] num Number to be converted.
  @returns ::word_one Converted value.
 */
word_one uint_to_binary(uint num) noexcept {
  word_one bits{};
  for (size_t pos{0}; pos < bits.size(); ++pos) {
    if (num > 0) {
      // Bitwise and to set flag.
      bits.set(pos, static_cast<bool>(num & 1));
      // Right-shift bits by 1, same as division by 2
      num >>= 1;
    } else {
      break;
    }
  }
  return bits;
}

/*!
  \brief Convert integer to 32-bit (one word) binary bitset.

  Uses two's complement to convert an integer into a binary value.

  @param[in] num Number to be converted.
  @returns ::word_one Converted value.
  */
word_one int_to_binary(int num) noexcept {
  word_one bits{};
  if (num >= 0) {
    bits = uint_to_binary(static_cast<uint>(num));
  } else {
    bits = uint_to_binary(static_cast<uint>(-num));
    // Complement
    bits.flip();
    bits = bits.to_ulong() + 1;
  }
  return bits;
}

/*!
  \brief Convert 32-bit (one word) binary bitset to integer.

  Uses two's complement to convert a binary value into an integer.

  @param[in] bin Binary value to be converted.
  @returns int Converted value.
 */
int binary_to_int(word_one bin) noexcept {
  int result{};
  if (bin.test(binary_word_size - 1)) {
    // Complement
    bin.flip();
    result = static_cast<int>(bin.to_ulong());
    result += 1;
    // Change sign to make it negative
    result *= -1;
  } else {
    result = static_cast<int>(bin.to_ulong());
  }
  return result;
}

/*!
  \brief Convert floating-point value to 32-bit (one word) binary bitset.

  Converts float to unsigned-integer of same size for storage in bitset.

  @param[in] num Float value to be converted.
  @returns ::word_one Converted value.
 */
word_one float_to_binary(const float num) noexcept {
  unsigned_int<float> num_as_uint{0};
  // flawfinder: ignore
  std::memcpy(&num_as_uint, &num, sizeof(float));
  word_one result{num_as_uint};
  return result;
}

/*!
  \brief Convert 32-bit (one word) binary bitset to a floating-point value.

  Converts bitset to unsigned long then to float.

  @param[in] bin ::word_one Binary value to be converted.
  @returns float Converted value.
  */
float binary_to_float(const word_one &bin) noexcept {
  const auto val = bin.to_ulong();
  float result{};
  // flawfinder: ignore
  memcpy(&result, &val, sizeof(float));
  return result;
}

/*!
  \brief Convert double-precision value to 64-bit (two words) binary bitset.

  Converts double to unsigned-integer of same size for storage in bitset.

  @param[in] num Double value to be converted.
  @returns ::word_two Converted value.
 */
word_two double_to_binary(const double num) noexcept {
  unsigned_int<double> num_as_uint{0};
  // flawfinder: ignore
  std::memcpy(&num_as_uint, &num, sizeof(double));
  word_two result{num_as_uint};
  return result;
}

/*!
  \brief Convert 64-bit (two words) binary bitset to double-precision value.

  Converts bitset to unsigned long long then to double.

  @param[in] bin ::word_two Binary value to be converted.
  @returns double Converted value.
 */
double binary_to_double(const word_two &bin) noexcept {
  const auto val = bin.to_ullong();
  double result{};
  // flawfinder: ignore
  memcpy(&result, &val, sizeof(double));
  return result;
}

/*!
  \brief Remove all leading spaces from a string.

  This edits the string in-place.

  @param[in, out] str std::string* String to have spaces removed.
 */
void remove_leading_spaces(std::string *str) noexcept {
  while ((static_cast<int>(str->front()) <= ascii_space) && (!str->empty())) {
    str->erase(0, 1);
  }
}

/*!
  \brief Remove all trailing spaces from a string.

  This edits the string in-place.

  @param[in, out] str std::string* String to have spaces removed.
 */
void remove_trailing_spaces(std::string *str) noexcept {
  while ((static_cast<int>(str->back()) <= ascii_space) && (!str->empty())) {
    str->pop_back();
  }
}

/*!
  \brief Remove leading/trailing spaces and control characters from a string.

  @param[in] str std::string String to be cleaned.
  @returns std::string Cleaned string.
 */
std::string string_cleaning(const std::string &str) noexcept {
  std::string result{str};
  size_t null_position{str.find('\0')};
  if (null_position != std::string::npos) {
    result.erase(null_position);
  }
  remove_leading_spaces(&result);
  remove_trailing_spaces(&result);
  return result;
}

/*!
  \brief Cleans string and then truncates/pads as necessary.

  This edits the string in-place.

  @param[in, out] str std::string* String to be prepared.
  @param[in] str_size Desired string length.
 */
void prep_string(std::string *str, const size_t str_size) noexcept {
  *str = string_cleaning(*str);
  if (str->length() > str_size) {
    str->resize(str_size);
  } else if (str->length() < str_size) {
    *str = str->append(str_size - str->length(), ' ');
  }
}

/*!
  \brief Template function to convert string into binary bitset.

  Note that this edits the bitset in place.

  @param[out] bits Destintation bitset for the string (result).
  @param[in] str String to undergo conversion.
  @param[in] str_size Desired string size in words (4 chars = 1 word).
 */
template <typename T>
void string_bits(T *bits, const std::string &str,
                 const size_t str_size) noexcept {
  constexpr size_t char_size{bits_per_byte};
  char_bit byte{};
  for (size_t i{0}; i < str_size; ++i) {
    size_t character{static_cast<size_t>(str[i])};
    byte = char_bit(character);
    for (size_t j{0}; j < char_size; ++j) {
      (*bits)[(i * char_size) + j] = byte[j];
    }
  }
}

/*!
  \brief Template function to convert binary bitset to string.

  @param[in] bits Source bitset for the string.
  @param[in] num_words Length of string in words (4 chars = 1 word)
  @returns std::string String converted from bitset.
 */
template <typename T>
std::string bits_string(const T &bits, const size_t num_words) noexcept {
  std::string result{};
  result.reserve(num_words * word_length);
  constexpr size_t char_size{bits_per_byte};
  char_bit byte{};
  for (size_t i{0}; i < num_words * binary_word_size; i += char_size) {
    for (size_t j{0}; j < char_size; ++j) [[likely]] {
      byte[j] = bits[i + j];
    }
    result.push_back(static_cast<char>(byte.to_ulong()));
  }
  return result;
}

/*!
  \brief Convert string to a 64-bit (two word) binary bitset

  If the string is longer than 8 characters, then only the first 8 characters
  are kept. If the string is less than 8 characters long, it is right-padded
  with spaces.

  @param[in] str String to be converted to a bitset.
  @returns ::word_two Converted binary bitset.
 */
word_two string_to_binary(std::string str) noexcept {
  constexpr size_t string_size{2 * word_length};
  // 1 byte per character
  prep_string(&str, string_size);
  // Two words (8 characters)
  word_two bits{};
  string_bits(&bits, str, string_size);
  return bits;
}

/*!
  \brief Convert a 64-bit (two word) binary bitset to a string.

  @param[in] str ::word_two to be converted to a string.
  @returns std::string Converted string.
 */
std::string binary_to_string(const word_two &str) noexcept {
  std::string result{bits_string(str, 2)};
  return string_cleaning(result);
}

/*!
  \brief Convert a string to a 128-bit (four word) binary bitset.

  If the string is longer than 16 characters, then only the first 16 characters
  are kept. If the string is less than 16 characters long, it is right-padded
  with spaces.

  Exclusively used to work with the kEvNm header.

  @param[in] str String to be converted to a bitset.
  @returns ::word_four Converted binary bitset.
 */
word_four long_string_to_binary(std::string str) noexcept {
  constexpr size_t string_size{4 * word_length};
  prep_string(&str, string_size);
  // Four words (16 characters)
  word_four bits{};
  string_bits(&bits, str, string_size);
  return bits;
}

/*!
  \brief Convert a 128-bit (four word) binary bitset to a string.

  Exclusively used to work with the kEvNm header.

  @param[in] str ::word_four to be converted to a string.
  @returns std::string Converted string.
 */
std::string binary_to_long_string(const word_four &str) noexcept {
  std::string result{bits_string(str, 4)};
  return string_cleaning(result);
}

/*!
  \brief Convert a boolean to a 32-bit (one word) binary bitset.

  @param[in] flag Boolean value to be converted to a bitset (sets zeroth
  element).
  @returns ::word_one Converted binary bitset.
 */
word_one bool_to_binary(const bool flag) noexcept {
  word_one result{};
  result[0] = flag;
  return result;
}

/*!
  \brief Convert a 32-bit (one word) binary bitset to a boolean.

  @param[in] flag ::word_one binary bitset to be converted (takes zeroth
  element).
  @returns boolean Converted boolean value.
 */
bool binary_to_bool(const word_one &flag) noexcept { return flag[0]; }

/*!
  \brief Concatenate two ::word_one binary strings into a single ::word_two
  string.

  Useful for reading strings from SAC-files.

  @param[in] pair_words word_pair Words to be concatenated.
  @returns ::word_two Concatenated words.
 */
word_two concat_words(const word_pair<word_one> &pair_words) noexcept {
  word_two result{};
  for (size_t i{0}; i < binary_word_size; ++i) [[likely]] {
    result[i] = pair_words.first[i];
    result[i + binary_word_size] = pair_words.second[i];
  }
  return result;
}

/*!
  \brief Concatenate two ::word_two binary strings into a single ::word_four
  string.

  Exclusively used to read kEvNm header from SAC-file.

  @param[in] pair_words word_pair Words to be concatenated.
  @returns ::word_four Concatenated words.
 */
word_four concat_words(const word_pair<word_two> &pair_words) noexcept {
  word_four result{};
  constexpr size_t two_words{2 * binary_word_size};
  for (size_t i{0}; i < two_words; ++i) [[likely]] {
    result[i] = pair_words.first[i];
    result[i + two_words] = pair_words.second[i];
  }
  return result;
}
//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
/*!
  \brief Read one word (32 bits, useful for non-strings) from a binary SAC-File.

  Note that this modifies the position of the reader within the stream (to the
  end of the read word).

  @param[in, out] sac std::ifstream* Input binary SAC-file.
  @returns ::word_one Binary bitset representation of single word.
 */
word_one read_word(std::ifstream *sac) {
  word_one bits{};
  constexpr size_t char_size{bits_per_byte};
  // Where we will store the characters
  std::array<char, word_length> word{};
  // Read to our character array
  // This can always hold the source due to careful typing/sizing
  // flawfinder: ignore
  if (sac->read(word.data(), word_length)) {
    // Take each character
    for (size_t i{0}; i < word_length; ++i) [[likely]] {
      uint character{static_cast<uint>(word[i])};
      char_bit byte{character};
      // bit-by-bit
      for (size_t j{0}; j < char_size; ++j) [[likely]] {
        bits[(i * char_size) + j] = byte[j];
      }
    }
  }
  return bits;
}

/*!
  \brief Read two words (64 bits, useful for most strings) from a binary
  SAC-file.

  Note that this modifies the position of the reader within the stream (to the
  end of the read words).

  @param[in, out] sac std::ifstream* Input binary SAC-file.
  @returns ::word_two Binary bitset representation of two words.
 */
word_two read_two_words(std::ifstream *sac) {
  const word_one first_word{read_word(sac)};
  const word_one second_word{read_word(sac)};
  word_pair<word_one> pair_words{};
  if constexpr (std::endian::native == std::endian::little) {
    pair_words.first = first_word;
    pair_words.second = second_word;
  } else {
    pair_words.first = second_word;
    pair_words.second = first_word;
  }
  return concat_words(pair_words);
}

/*!
  \brief Read four words (128 bits, kEvNm only) from a binary SAC-file.

  Note that this modifies the position of the reader within the stream (to the
  end of the read words).

  @param[in, out] sac std::ifstream* Input binary SAC-file.
  @returns ::word_four Binary bitset representation of four words.
 */
word_four read_four_words(std::ifstream *sac) {
  const word_two first_words{read_two_words(sac)};
  const word_two second_words{read_two_words(sac)};
  word_pair<word_two> pair_words{};
  if constexpr (std::endian::native == std::endian::little) {
    pair_words.first = first_words;
    pair_words.second = second_words;
  } else {
    pair_words.first = second_words;
    pair_words.second = first_words;
  }
  return concat_words(pair_words);
}

/*!
  \brief Reader arbitrary number of words (useful for vectors) from a binary
  SAC-file.

  Note that this modifies the position of the reader within the stream (to the
  end of the read words).

  @param[in, out] sac std::ifstream* Input binary SAC-file.
  @param[in] spec read_spec Reading specification.
  @returns std::vector<double> Data vector read in.
 */
std::vector<double> read_data(std::ifstream *sac, const read_spec &spec) {
  sac->seekg(word_position(spec.start_word));
  std::vector<double> result{};
  result.resize(spec.num_words);
  std::for_each(result.begin(), result.end(), [&sac](double &value) {
    value = static_cast<double>(binary_to_float(read_word(sac)));
  });
  return result;
}
//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
/*!
  \brief Write arbitrary number of words (useful for vectors) to a binary
  SAC-file.

  Note that this modifies the position of the writer within the stream (to the
  end of the written words).

  @param[in, out] sac_file std::ofstream* Output binary SAC-file.
  @param[in] input std::vector<char> Character vector representation of data for
  writing.
 */
void write_words(std::ofstream *sac_file, const std::vector<char> &input) {
  std::ofstream &sac = *sac_file;
  if (sac.is_open()) {
    std::for_each(input.begin(), input.end(), [&sac](const char &character) {
      sac.write(&character, sizeof(char));
    });
  }
}

/*!
  \brief Template function to convert input value into a std::vector<char> for
  writing.

  @param[in] input Input value (float or int) to convert.
  @return std::vector<char> Prepared for writing to binary SAC-file.
 */
template <typename T>
std::vector<char> convert_to_word(const T input) noexcept {
  std::array<char, word_length> tmp{};
  // Copy bytes from input into the tmp array
  // flawfinder: ignore
  std::memcpy(tmp.data(), &input, word_length);
  std::vector<char> word{};
  word.reserve(word_length);
  std::for_each(tmp.begin(), tmp.end(),
                [&word](const char &character) { word.push_back(character); });
  return word;
}

// Explicit instantiation
template std::vector<char> convert_to_word(const float input) noexcept;
template std::vector<char> convert_to_word(const int x) noexcept;

/*!
  \brief Convert double value into a std::vector<char> for writing.

  @param[in] input Input value to convert (double).
  @return std::vector<char> Prepared for writing to binary SAC-file.
 */
std::vector<char> convert_to_word(const double input) noexcept {
  constexpr size_t n_words{static_cast<size_t>(2) * word_length};
  std::array<char, n_words> tmp{};
  // Copy bytes from input into the tmp array
  // flawfinder: ignore
  std::memcpy(tmp.data(), &input, n_words);
  std::vector<char> word{};
  word.reserve(n_words);
  std::for_each(tmp.begin(), tmp.end(),
                [&word](const char &character) { word.push_back(character); });
  return word;
}

// Variable sized words for the 'K' headers
/*!
  \brief Template function to convert input string value into a std::array<char>
  for writing.

  @param[in] str Input string to convert.
  @param[in] n_words Number of words
  @return std::array<char, N> Prepared for writing to a binary SAC-file.
  */
template <size_t N>
std::array<char, N> convert_to_words(const std::string &str,
                                     const size_t n_words) noexcept {
  std::vector<char> tmp{};
  tmp.reserve(n_words);
  std::for_each(str.begin(), str.end(),
                [&tmp](const char &character) { tmp.push_back(character); });
  std::array<char, N> all_words{};
  // Move vector to array
  std::move(tmp.begin(), tmp.end(), all_words.begin());
  return all_words;
}

// Explicit instantiation
template std::array<char, word_length>
convert_to_words(const std::string &str, const size_t n_words) noexcept;
template std::array<char, 2 * word_length>
convert_to_words(const std::string &str, const size_t n_words) noexcept;
template std::array<char, 4 * word_length>
convert_to_words(const std::string &str, const size_t n_words) noexcept;

/*!
  \brief Convert boolean to a word for writing.

  @param[in] flag Boolean to be converted.
  @returns std::vector<char> Prepared value for writing.
  */
std::vector<char> bool_to_word(const bool flag) noexcept {
  std::vector<char> result;
  result.resize(word_length);
  std::fill(result.begin() + 1, result.end(), 0);
  result[0] = static_cast<char>(flag ? 1 : 0);
  return result;
}
//-----------------------------------------------------------------------------
// Convenience methods
// -----------------------------------------------------------------------------
// Does not assume equal length, if not equal length then they're not equal
// within tolerance
/*!
  \brief Check if two std::vector<double> are equal within a tolerance limit.

  Default tolerance is f_eps.

  @param[in] vector1 First data vector in comparison.
  @param[in] vector2 Second data vector in comparison.
  @param[in] tolerance Numerical equality tolerance (default f_eps).
  @returns bool Boolean equality value.
 */
bool equal_within_tolerance(const std::vector<double> &vector1,
                            const std::vector<double> &vector2,
                            const double tolerance) noexcept {
  if (vector1.size() != vector2.size()) {
    return false;
  }
  for (size_t i{0}; i < vector1.size(); ++i) [[likely]] {
    if (!equal_within_tolerance(vector1[i], vector2[i], tolerance)) {
      return false;
    }
  }
  return true;
}

/*!
  \brief Check if  two double values are equal within a tolerance limit.

  Default tolerance is f_eps.

  @param[in] val1 First double in comparison.
  @param[in] val2 Second double in comparison.
  @param[in] tolerance Numerical equality tolerance (default f_eps).
  @returns bool Boolean equality value.
 */
bool equal_within_tolerance(const double val1, const double val2,
                            const double tolerance) noexcept {
  return std::abs(val1 - val2) < tolerance;
}
// Position methods
/*!
  \brief Convert decimal degrees to radians.

  \f[\color{orange}
  r=d\cdot\frac{\pi}{180^{\circ}}
  \f]

  @param[in] degrees Angle in decimal degrees to be converted.
  @returns double Angle in radians.
 */
double degrees_to_radians(const double degrees) noexcept {
  return rad_per_deg * degrees;
}

/*!
  \brief Convert radians to decimal degrees.

  \f[\color{orange}
  d=r\cdot\frac{180^{\circ}}{\pi}
  \f]

  @param[in] radians Angle in radians to be converted.
  @returns double Angle in decimal degrees.
 */
double radians_to_degrees(const double radians) noexcept {
  return deg_per_rad * radians;
}

/*!
  \brief Coordinate constructor

  @param[in] value Double value of coordinate
  @param[in] degrees Boolean value, true if degrees (false = radians).
  */
coord::coord(const double value, const bool degrees) noexcept {
  if (degrees) {
    deg = value;
    rad = degrees_to_radians(value);
  } else {
    rad = value;
    deg = radians_to_degrees(value);
  }
}

/*!
  \brief Set coordinate value using decimal degrees.

  @param[in] value double coordinate in decimal degrees.
 */
void coord::degrees(const double value) noexcept {
  deg = value;
  rad = degrees_to_radians(value);
}

/*!
  \brief Set coordainate value using radians.

  @param[in] value double coordinate in radians.
 */
void coord::radians(const double value) noexcept {
  rad = value;
  deg = radians_to_degrees(value);
}

/*!
  \brief Calculate great circle arc distance in decimal degrees between two
  points.

  Assumes spherical Earth (in future will include flatenning and adjustable
  radius for other bodies/greater accuracy).

  \f$\color{orange}\phi\f$ is latitude.
  \f$\color{orange}\lambda\f$ is longitude.
  \f$\color{orange}\Delta\f$ is great circle arc distance (gcarc).

  \f[\color{orange}
  \Delta = cos^{-1}\left(
  sin(\phi_{1})sin(\phi_{2}) + cos(\phi_{1})cos(\phi_{2})
  cos(\lambda_{2}-\lambda_{1})
  \right)
  \f]

  @param[in] location1 point of first location.
  @param[in] location2 point of second location
  @returns double The great circle arc distance in decimal degrees.
 */
double gcarc(const point location1, const point location2) noexcept {
  return radians_to_degrees(
      std::acos(std::sin(location1.latitude.radians()) *
                    std::sin(location2.latitude.radians()) +
                std::cos(location1.latitude.radians()) *
                    std::cos(location2.latitude.radians()) *
                    std::cos(location2.longitude.radians() -
                             location1.longitude.radians())));
}

/*!
  \brief Calculate azimuth between two points.

  Assumes spherical Earth (in future may update to solve on a more general
  body).

  \f$\color{orange}\phi\f$ is latitude.
  \f$\color{orange}\lambda\f$ is longitude.
  \f$\color{orange}\theta\f$ is azimuth.

  \f[\color{orange}
  \theta=tan^{-1}\left(
  \frac{sin(\delta\lambda)cos(\phi_{2})}{cos(\phi_{1})sin(\phi_{2})
  - sin(\phi_{1})cos(\phi_{2})cos(\delta\lambda)}
  \right)
  \f]

  @param[in] location1 point of first location.
  @param[in] location2 point of second location.
  @returns double The azimuth from the first location to the second location.
  */
double azimuth(const point location1, const point location2) noexcept {
  const double numerator{
      std::sin(location2.longitude.radians() - location1.longitude.radians()) *
      std::cos(location2.latitude.radians())};
  const double denominator{(std::cos(location1.latitude.radians()) *
                            std::sin(location2.latitude.radians())) -
                           (std::sin(location1.latitude.radians()) *
                            std::cos(location2.latitude.radians()) *
                            std::cos(location2.longitude.radians() -
                                     location1.longitude.radians()))};
  double result{radians_to_degrees(std::atan2(numerator, denominator))};
  while (result < 0.0) {
    result += circle_deg;
  }
  return result;
}

/*!
  \brief Takes a decimal degree value and constrains it to full circle using
  symmetry.

  \f[\color{orange}
  \left[-\infty,\infty\right]\rightarrow\left[0, 360\right]
  \f]

  @param[in] degrees Decimal degrees to be constrained.
  @returns double Value within limits.
 */
double limit_360(const double degrees) noexcept {
  double result{degrees};
  while (std::abs(result) > circle_deg) {
    if (result > circle_deg) {
      result -= circle_deg;
    } else {
      result += circle_deg;
    }
  }
  if (result < 0) {
    result += circle_deg;
  }
  return result;
}

/*!
  \brief Takes a decimal degree value and constrains it to a half circle using
  symmetry.

  \f[\color{orange}
  \left[-\infty,\infty\right]\rightarrow (-180,180]
  \f]

  @param[in] degrees Decimal degrees to be constrained.
  @returns double Value within limits.
 */
double limit_180(const double degrees) noexcept {
  double result{limit_360(degrees)};
  constexpr double hemi{180.0};
  if (result > hemi) {
    result = result - circle_deg;
  }
  return result;
}

/*!
  \brief Takes a decimal degree value and constrains it to a quarter circle
  using symmetry.

  \f[\color{orange}
  \left[-\infty,\infty\right]\rightarrow\left[-90,90\right]
  \f]

  @param[in] degrees Decimal degrees to be constrained.
  @returns double Value within limits.
  */
double limit_90(const double degrees) noexcept {
  double result{limit_180(degrees)};
  constexpr double quarter{90.0};
  if (result > quarter) {
    result = (2 * quarter) - result;
  } else if (result < -quarter) {
    result = (-2 * quarter) - result;
  }
  return result;
}
//------------------------------------------------------------------------------
// Trace
//------------------------------------------------------------------------------
/*!
  \brief Trace default constructor.

  Fills all values with their default (unset) values.
  Data vectors are of size zero.

  @returns Default created Trace object.
 */
Trace::Trace() noexcept {
  std::fill(floats.begin(), floats.end(), unset_float);
  std::fill(doubles.begin(), doubles.end(), unset_double);
  std::fill(ints.begin(), ints.end(), unset_int);
  std::fill(bools.begin(), bools.end(), unset_bool);
  std::fill(strings.begin(), strings.end(), unset_word);
}

/*!
  \brief Trace equality operator.

  @param[in] this First Trace in comparison (LHS).
  @param[in] other Second Trace in comparison (RHS).
  @returns bool Truth value of equality.
 */
bool Trace::operator==(const Trace &other) const noexcept {
  if (floats != other.floats) {
    return false;
  }
  if (doubles != other.doubles) {
    return false;
  }
  if (ints != other.ints) {
    return false;
  }
  if (strings != other.strings) {
    return false;
  }
  if (!equal_within_tolerance(data[0], other.data[0])) {
    return false;
  }
  if (!equal_within_tolerance(data[1], other.data[1])) {
    return false;
  }
  return true;
}
// Convenience functions
/*!
  \brief Calculates gcarc, dist, az, and baz from stla, stlo, evla, and evlo.
 */
void Trace::calc_geometry() noexcept {
  if (geometry_set()) {
    calc_gcarc();
    calc_dist();
    calc_az();
    calc_baz();
  } else {
    gcarc(unset_double);
    dist(unset_double);
    az(unset_double);
    baz(unset_double);
  }
}

/*!
  \brief Calculate frequency from delta.

  \f[\color{orange}
  f=\frac{1}{\delta}
  \f]

  @returns double Frequency.
*/
double Trace::frequency() const noexcept {
  const double delta_val{delta()};
  if ((delta_val == unset_double) || (delta_val <= 0)) {
    return unset_double;
  }
  return 1.0 / delta_val;
}

/*!
  \brief Determine if locations are set for geometry calculation.

  @returns bool True if able to calculate geometry.
 */
bool Trace::geometry_set() const noexcept {
  return (stla() != unset_double) && (stlo() != unset_double) &&
         (evla() != unset_double) && (evlo() != unset_double);
}

/*!
  \brief Calculate great-circle arc-distance (gcarc).
 */
void Trace::calc_gcarc() noexcept {
  Trace::gcarc(
      static_cast<float>(sacfmt::gcarc(station_location(), event_location())));
}

/*!
  \brief Calculate distance (using gcarc).

  Assumes spherical Earth (in future may update to include flattening
  and different planteray bodies).

  \f[\color{orange}
  d=r_{E}\cdot\Delta
  \f]
 */
void Trace::calc_dist() noexcept {
  dist(static_cast<float>(earth_radius * rad_per_deg * gcarc()));
}

/*!
  \brief Calculate azimuth.

  \f[\color{orange}
  Station\rightarrow Event
  \f]
 */
void Trace::calc_az() noexcept {
  az(static_cast<float>(azimuth(event_location(), station_location())));
}

/*!
  \brief Calculate back-azimuth.

  \f[\color{orange}
  Event\rightarrow Station
  \f]
 */
void Trace::calc_baz() noexcept {
  baz(static_cast<float>(azimuth(station_location(), event_location())));
}

/*!
  \brief Get date string.

  @returns std::string Date (YYYY-JJJ).
 */
std::string Trace::date() const noexcept {
  // Require all to be set
  if ((nzyear() == unset_int) || (nzjday() == unset_int)) {
    return unset_word;
  }
  std::ostringstream oss{};
  oss << nzyear();
  oss << '-';
  oss << nzjday();
  return oss.str();
}

/*!
  \brief Get time string.

  @returns sstd::string Time (HH::MM:SS.sss).
 */
std::string Trace::time() const noexcept {
  // Require all to be set
  if ((nzhour() == unset_int) || (nzmin() == unset_int) ||
      (nzsec() == unset_int) || (nzmsec() == unset_int)) {
    return unset_word;
  }
  std::ostringstream oss{};
  oss << nzhour();
  oss << ':';
  oss << nzmin();
  oss << ':';
  oss << nzsec();
  oss << '.';
  oss << nzmsec();
  return oss.str();
}

// Getters
// Floats
float Trace::depmin() const noexcept {
  return floats[sac_map.at(name::depmin)];
}
float Trace::depmax() const noexcept {
  return floats[sac_map.at(name::depmax)];
}
float Trace::odelta() const noexcept {
  return floats[sac_map.at(name::odelta)];
}
float Trace::resp0() const noexcept { return floats[sac_map.at(name::resp0)]; }
float Trace::resp1() const noexcept { return floats[sac_map.at(name::resp1)]; }
float Trace::resp2() const noexcept { return floats[sac_map.at(name::resp2)]; }
float Trace::resp3() const noexcept { return floats[sac_map.at(name::resp3)]; }
float Trace::resp4() const noexcept { return floats[sac_map.at(name::resp4)]; }
float Trace::resp5() const noexcept { return floats[sac_map.at(name::resp5)]; }
float Trace::resp6() const noexcept { return floats[sac_map.at(name::resp6)]; }
float Trace::resp7() const noexcept { return floats[sac_map.at(name::resp7)]; }
float Trace::resp8() const noexcept { return floats[sac_map.at(name::resp8)]; }
float Trace::resp9() const noexcept { return floats[sac_map.at(name::resp9)]; }
float Trace::stel() const noexcept { return floats[sac_map.at(name::stel)]; }
float Trace::stdp() const noexcept { return floats[sac_map.at(name::stdp)]; }
float Trace::evel() const noexcept { return floats[sac_map.at(name::evel)]; }
float Trace::evdp() const noexcept { return floats[sac_map.at(name::evdp)]; }
float Trace::mag() const noexcept { return floats[sac_map.at(name::mag)]; }
float Trace::user0() const noexcept { return floats[sac_map.at(name::user0)]; }
float Trace::user1() const noexcept { return floats[sac_map.at(name::user1)]; }
float Trace::user2() const noexcept { return floats[sac_map.at(name::user2)]; }
float Trace::user3() const noexcept { return floats[sac_map.at(name::user3)]; }
float Trace::user4() const noexcept { return floats[sac_map.at(name::user4)]; }
float Trace::user5() const noexcept { return floats[sac_map.at(name::user5)]; }
float Trace::user6() const noexcept { return floats[sac_map.at(name::user6)]; }
float Trace::user7() const noexcept { return floats[sac_map.at(name::user7)]; }
float Trace::user8() const noexcept { return floats[sac_map.at(name::user8)]; }
float Trace::user9() const noexcept { return floats[sac_map.at(name::user9)]; }
float Trace::dist() const noexcept { return floats[sac_map.at(name::dist)]; }
float Trace::az() const noexcept { return floats[sac_map.at(name::az)]; }
float Trace::baz() const noexcept { return floats[sac_map.at(name::baz)]; }
float Trace::gcarc() const noexcept { return floats[sac_map.at(name::gcarc)]; }
float Trace::depmen() const noexcept {
  return floats[sac_map.at(name::depmen)];
}
float Trace::cmpaz() const noexcept { return floats[sac_map.at(name::cmpaz)]; }
float Trace::cmpinc() const noexcept {
  return floats[sac_map.at(name::cmpinc)];
}
float Trace::xminimum() const noexcept {
  return floats[sac_map.at(name::xminimum)];
}
float Trace::xmaximum() const noexcept {
  return floats[sac_map.at(name::xmaximum)];
}
float Trace::yminimum() const noexcept {
  return floats[sac_map.at(name::yminimum)];
}
float Trace::ymaximum() const noexcept {
  return floats[sac_map.at(name::ymaximum)];
}
// Doubles
double Trace::delta() const noexcept {
  return doubles[sac_map.at(name::delta)];
}
double Trace::b() const noexcept { return doubles[sac_map.at(name::b)]; }
double Trace::e() const noexcept { return doubles[sac_map.at(name::e)]; }
double Trace::o() const noexcept { return doubles[sac_map.at(name::o)]; }
double Trace::a() const noexcept { return doubles[sac_map.at(name::a)]; }
double Trace::t0() const noexcept { return doubles[sac_map.at(name::t0)]; }
double Trace::t1() const noexcept { return doubles[sac_map.at(name::t1)]; }
double Trace::t2() const noexcept { return doubles[sac_map.at(name::t2)]; }
double Trace::t3() const noexcept { return doubles[sac_map.at(name::t3)]; }
double Trace::t4() const noexcept { return doubles[sac_map.at(name::t4)]; }
double Trace::t5() const noexcept { return doubles[sac_map.at(name::t5)]; }
double Trace::t6() const noexcept { return doubles[sac_map.at(name::t6)]; }
double Trace::t7() const noexcept { return doubles[sac_map.at(name::t7)]; }
double Trace::t8() const noexcept { return doubles[sac_map.at(name::t8)]; }
double Trace::t9() const noexcept { return doubles[sac_map.at(name::t9)]; }
double Trace::f() const noexcept { return doubles[sac_map.at(name::f)]; }
double Trace::stla() const noexcept { return doubles[sac_map.at(name::stla)]; }
double Trace::stlo() const noexcept { return doubles[sac_map.at(name::stlo)]; }
double Trace::evla() const noexcept { return doubles[sac_map.at(name::evla)]; }
double Trace::evlo() const noexcept { return doubles[sac_map.at(name::evlo)]; }
double Trace::sb() const noexcept { return doubles[sac_map.at(name::sb)]; }
double Trace::sdelta() const noexcept {
  return doubles[sac_map.at(name::sdelta)];
}
// Ints
int Trace::nzyear() const noexcept { return ints[sac_map.at(name::nzyear)]; }
int Trace::nzjday() const noexcept { return ints[sac_map.at(name::nzjday)]; }
int Trace::nzhour() const noexcept { return ints[sac_map.at(name::nzhour)]; }
int Trace::nzmin() const noexcept { return ints[sac_map.at(name::nzmin)]; }
int Trace::nzsec() const noexcept { return ints[sac_map.at(name::nzsec)]; }
int Trace::nzmsec() const noexcept { return ints[sac_map.at(name::nzmsec)]; }
int Trace::nvhdr() const noexcept { return ints[sac_map.at(name::nvhdr)]; }
int Trace::norid() const noexcept { return ints[sac_map.at(name::norid)]; }
int Trace::nevid() const noexcept { return ints[sac_map.at(name::nevid)]; }
int Trace::npts() const noexcept { return ints[sac_map.at(name::npts)]; }
int Trace::nsnpts() const noexcept { return ints[sac_map.at(name::nsnpts)]; }
int Trace::nwfid() const noexcept { return ints[sac_map.at(name::nwfid)]; }
int Trace::nxsize() const noexcept { return ints[sac_map.at(name::nxsize)]; }
int Trace::nysize() const noexcept { return ints[sac_map.at(name::nysize)]; }
int Trace::iftype() const noexcept { return ints[sac_map.at(name::iftype)]; }
int Trace::idep() const noexcept { return ints[sac_map.at(name::idep)]; }
int Trace::iztype() const noexcept { return ints[sac_map.at(name::iztype)]; }
int Trace::iinst() const noexcept { return ints[sac_map.at(name::iinst)]; }
int Trace::istreg() const noexcept { return ints[sac_map.at(name::istreg)]; }
int Trace::ievreg() const noexcept { return ints[sac_map.at(name::ievreg)]; }
int Trace::ievtyp() const noexcept { return ints[sac_map.at(name::ievtyp)]; }
int Trace::iqual() const noexcept { return ints[sac_map.at(name::iqual)]; }
int Trace::isynth() const noexcept { return ints[sac_map.at(name::isynth)]; }
int Trace::imagtyp() const noexcept { return ints[sac_map.at(name::imagtyp)]; }
int Trace::imagsrc() const noexcept { return ints[sac_map.at(name::imagsrc)]; }
int Trace::ibody() const noexcept { return ints[sac_map.at(name::ibody)]; }
// Bools
bool Trace::leven() const noexcept { return bools[sac_map.at(name::leven)]; }
bool Trace::lpspol() const noexcept { return bools[sac_map.at(name::lpspol)]; }
bool Trace::lovrok() const noexcept { return bools[sac_map.at(name::lovrok)]; }
bool Trace::lcalda() const noexcept { return bools[sac_map.at(name::lcalda)]; }
// Strings
std::string Trace::kstnm() const noexcept {
  return strings[sac_map.at(name::kstnm)];
}
std::string Trace::kevnm() const noexcept {
  return strings[sac_map.at(name::kevnm)];
}
std::string Trace::khole() const noexcept {
  return strings[sac_map.at(name::khole)];
}
std::string Trace::ko() const noexcept { return strings[sac_map.at(name::ko)]; }
std::string Trace::ka() const noexcept { return strings[sac_map.at(name::ka)]; }
std::string Trace::kt0() const noexcept {
  return strings[sac_map.at(name::kt0)];
}
std::string Trace::kt1() const noexcept {
  return strings[sac_map.at(name::kt1)];
}
std::string Trace::kt2() const noexcept {
  return strings[sac_map.at(name::kt2)];
}
std::string Trace::kt3() const noexcept {
  return strings[sac_map.at(name::kt3)];
}
std::string Trace::kt4() const noexcept {
  return strings[sac_map.at(name::kt4)];
}
std::string Trace::kt5() const noexcept {
  return strings[sac_map.at(name::kt5)];
}
std::string Trace::kt6() const noexcept {
  return strings[sac_map.at(name::kt6)];
}
std::string Trace::kt7() const noexcept {
  return strings[sac_map.at(name::kt7)];
}
std::string Trace::kt8() const noexcept {
  return strings[sac_map.at(name::kt8)];
}
std::string Trace::kt9() const noexcept {
  return strings[sac_map.at(name::kt9)];
}
std::string Trace::kf() const noexcept { return strings[sac_map.at(name::kf)]; }
std::string Trace::kuser0() const noexcept {
  return strings[sac_map.at(name::kuser0)];
}
std::string Trace::kuser1() const noexcept {
  return strings[sac_map.at(name::kuser1)];
}
std::string Trace::kuser2() const noexcept {
  return strings[sac_map.at(name::kuser2)];
}
std::string Trace::kcmpnm() const noexcept {
  return strings[sac_map.at(name::kcmpnm)];
}
std::string Trace::knetwk() const noexcept {
  return strings[sac_map.at(name::knetwk)];
}
std::string Trace::kdatrd() const noexcept {
  return strings[sac_map.at(name::kdatrd)];
}
std::string Trace::kinst() const noexcept {
  return strings[sac_map.at(name::kinst)];
}
// Data
std::vector<double> Trace::data1() const noexcept {
  return data[sac_map.at(name::data1)];
}
std::vector<double> Trace::data2() const noexcept {
  return data[sac_map.at(name::data2)];
}
// Setters
// Floats
void Trace::depmin(const float input) noexcept {
  floats[sac_map.at(name::depmin)] = input;
}
void Trace::depmax(const float input) noexcept {
  floats[sac_map.at(name::depmax)] = input;
}
void Trace::odelta(const float input) noexcept {
  floats[sac_map.at(name::odelta)] = input;
}
void Trace::resp0(const float input) noexcept {
  floats[sac_map.at(name::resp0)] = input;
}
void Trace::resp1(const float input) noexcept {
  floats[sac_map.at(name::resp1)] = input;
}
void Trace::resp2(const float input) noexcept {
  floats[sac_map.at(name::resp2)] = input;
}
void Trace::resp3(const float input) noexcept {
  floats[sac_map.at(name::resp3)] = input;
}
void Trace::resp4(const float input) noexcept {
  floats[sac_map.at(name::resp4)] = input;
}
void Trace::resp5(const float input) noexcept {
  floats[sac_map.at(name::resp5)] = input;
}
void Trace::resp6(const float input) noexcept {
  floats[sac_map.at(name::resp6)] = input;
}
void Trace::resp7(const float input) noexcept {
  floats[sac_map.at(name::resp7)] = input;
}
void Trace::resp8(const float input) noexcept {
  floats[sac_map.at(name::resp8)] = input;
}
void Trace::resp9(const float input) noexcept {
  floats[sac_map.at(name::resp9)] = input;
}
void Trace::stel(const float input) noexcept {
  floats[sac_map.at(name::stel)] = input;
}
void Trace::stdp(const float input) noexcept {
  floats[sac_map.at(name::stdp)] = input;
}
void Trace::evel(const float input) noexcept {
  floats[sac_map.at(name::evel)] = input;
}
void Trace::evdp(const float input) noexcept {
  floats[sac_map.at(name::evdp)] = input;
}
void Trace::mag(const float input) noexcept {
  floats[sac_map.at(name::mag)] = input;
}
void Trace::user0(const float input) noexcept {
  floats[sac_map.at(name::user0)] = input;
}
void Trace::user1(const float input) noexcept {
  floats[sac_map.at(name::user1)] = input;
}
void Trace::user2(const float input) noexcept {
  floats[sac_map.at(name::user2)] = input;
}
void Trace::user3(const float input) noexcept {
  floats[sac_map.at(name::user3)] = input;
}
void Trace::user4(const float input) noexcept {
  floats[sac_map.at(name::user4)] = input;
}
void Trace::user5(const float input) noexcept {
  floats[sac_map.at(name::user5)] = input;
}
void Trace::user6(const float input) noexcept {
  floats[sac_map.at(name::user6)] = input;
}
void Trace::user7(const float input) noexcept {
  floats[sac_map.at(name::user7)] = input;
}
void Trace::user8(const float input) noexcept {
  floats[sac_map.at(name::user8)] = input;
}
void Trace::user9(const float input) noexcept {
  floats[sac_map.at(name::user9)] = input;
}
void Trace::dist(const float input) noexcept {
  floats[sac_map.at(name::dist)] = input;
}
void Trace::az(const float input) noexcept {
  floats[sac_map.at(name::az)] = input;
}
void Trace::baz(const float input) noexcept {
  floats[sac_map.at(name::baz)] = input;
}
void Trace::gcarc(const float input) noexcept {
  floats[sac_map.at(name::gcarc)] = input;
}
void Trace::depmen(const float input) noexcept {
  floats[sac_map.at(name::depmen)] = input;
}
void Trace::cmpaz(const float input) noexcept {
  floats[sac_map.at(name::cmpaz)] = input;
}
void Trace::cmpinc(const float input) noexcept {
  floats[sac_map.at(name::cmpinc)] = input;
}
void Trace::xminimum(const float input) noexcept {
  floats[sac_map.at(name::xminimum)] = input;
}
void Trace::xmaximum(const float input) noexcept {
  floats[sac_map.at(name::xmaximum)] = input;
}
void Trace::yminimum(const float input) noexcept {
  floats[sac_map.at(name::yminimum)] = input;
}
void Trace::ymaximum(const float input) noexcept {
  floats[sac_map.at(name::ymaximum)] = input;
}
// Doubles
void Trace::delta(const double input) noexcept {
  doubles[sac_map.at(name::delta)] = input;
}
void Trace::b(const double input) noexcept {
  doubles[sac_map.at(name::b)] = input;
}
void Trace::e(const double input) noexcept {
  doubles[sac_map.at(name::e)] = input;
}
void Trace::o(const double input) noexcept {
  doubles[sac_map.at(name::o)] = input;
}
void Trace::a(const double input) noexcept {
  doubles[sac_map.at(name::a)] = input;
}
void Trace::t0(const double input) noexcept {
  doubles[sac_map.at(name::t0)] = input;
}
void Trace::t1(const double input) noexcept {
  doubles[sac_map.at(name::t1)] = input;
}
void Trace::t2(const double input) noexcept {
  doubles[sac_map.at(name::t2)] = input;
}
void Trace::t3(const double input) noexcept {
  doubles[sac_map.at(name::t3)] = input;
}
void Trace::t4(const double input) noexcept {
  doubles[sac_map.at(name::t4)] = input;
}
void Trace::t5(const double input) noexcept {
  doubles[sac_map.at(name::t5)] = input;
}
void Trace::t6(const double input) noexcept {
  doubles[sac_map.at(name::t6)] = input;
}
void Trace::t7(const double input) noexcept {
  doubles[sac_map.at(name::t7)] = input;
}
void Trace::t8(const double input) noexcept {
  doubles[sac_map.at(name::t8)] = input;
}
void Trace::t9(const double input) noexcept {
  doubles[sac_map.at(name::t9)] = input;
}
void Trace::f(const double input) noexcept {
  doubles[sac_map.at(name::f)] = input;
}
void Trace::stla(const double input) noexcept {
  double clean_input{input};
  if (clean_input != unset_double) {
    clean_input = limit_90(clean_input);
  }
  doubles[sac_map.at(name::stla)] = clean_input;
}
void Trace::stlo(const double input) noexcept {
  double clean_input{input};
  if (clean_input != unset_double) {
    clean_input = limit_180(clean_input);
  }
  doubles[sac_map.at(name::stlo)] = clean_input;
}
void Trace::evla(const double input) noexcept {
  double clean_input{input};
  if (clean_input != unset_double) {
    clean_input = limit_90(clean_input);
  }
  doubles[sac_map.at(name::evla)] = clean_input;
}
void Trace::evlo(const double input) noexcept {
  double clean_input{input};
  if (clean_input != unset_double) {
    clean_input = limit_180(clean_input);
  }
  doubles[sac_map.at(name::evlo)] = clean_input;
}
void Trace::sb(const double input) noexcept {
  doubles[sac_map.at(name::sb)] = input;
}
void Trace::sdelta(const double input) noexcept {
  doubles[sac_map.at(name::sdelta)] = input;
}
// Ints
void Trace::nzyear(const int input) noexcept {
  ints[sac_map.at(name::nzyear)] = input;
}
void Trace::nzjday(const int input) noexcept {
  ints[sac_map.at(name::nzjday)] = input;
}
void Trace::nzhour(const int input) noexcept {
  ints[sac_map.at(name::nzhour)] = input;
}
void Trace::nzmin(const int input) noexcept {
  ints[sac_map.at(name::nzmin)] = input;
}
void Trace::nzsec(const int input) noexcept {
  ints[sac_map.at(name::nzsec)] = input;
}
void Trace::nzmsec(const int input) noexcept {
  ints[sac_map.at(name::nzmsec)] = input;
}
void Trace::nvhdr(const int input) noexcept {
  ints[sac_map.at(name::nvhdr)] = input;
}
void Trace::norid(const int input) noexcept {
  ints[sac_map.at(name::norid)] = input;
}
void Trace::nevid(const int input) noexcept {
  ints[sac_map.at(name::nevid)] = input;
}
void Trace::npts(const int input) noexcept {
  if ((input >= 0) || (input == unset_int)) {
    ints[sac_map.at(name::npts)] = input;
    const size_t size{static_cast<size_t>(input >= 0 ? input : 0)};
    resize_data(size);
  }
}
void Trace::nsnpts(const int input) noexcept {
  ints[sac_map.at(name::nsnpts)] = input;
}
void Trace::nwfid(const int input) noexcept {
  ints[sac_map.at(name::nwfid)] = input;
}
void Trace::nxsize(const int input) noexcept {
  ints[sac_map.at(name::nxsize)] = input;
}
void Trace::nysize(const int input) noexcept {
  ints[sac_map.at(name::nysize)] = input;
}
void Trace::iftype(const int input) noexcept {
  ints[sac_map.at(name::iftype)] = input;
  const size_t size{npts() >= 0 ? static_cast<size_t>(npts()) : 0};
  // Uneven 2D data not supported as not in specification
  if ((input > 1) && !leven()) {
    leven(true);
  }
  resize_data2(size);
}
void Trace::idep(const int input) noexcept {
  ints[sac_map.at(name::idep)] = input;
}
void Trace::iztype(const int input) noexcept {
  ints[sac_map.at(name::iztype)] = input;
}
void Trace::iinst(const int input) noexcept {
  ints[sac_map.at(name::iinst)] = input;
}
void Trace::istreg(const int input) noexcept {
  ints[sac_map.at(name::istreg)] = input;
}
void Trace::ievreg(const int input) noexcept {
  ints[sac_map.at(name::ievreg)] = input;
}
void Trace::ievtyp(const int input) noexcept {
  ints[sac_map.at(name::ievtyp)] = input;
}
void Trace::iqual(const int input) noexcept {
  ints[sac_map.at(name::iqual)] = input;
}
void Trace::isynth(const int input) noexcept {
  ints[sac_map.at(name::isynth)] = input;
}
void Trace::imagtyp(const int input) noexcept {
  ints[sac_map.at(name::imagtyp)] = input;
}
void Trace::imagsrc(const int input) noexcept {
  ints[sac_map.at(name::imagsrc)] = input;
}
void Trace::ibody(const int input) noexcept {
  ints[sac_map.at(name::ibody)] = input;
}
// Bools
void Trace::leven(const bool input) noexcept {
  bools[sac_map.at(name::leven)] = input;
  const size_t size{npts() >= 0 ? static_cast<size_t>(npts()) : 0};
  // Uneven 2D data not supported since not in specification
  if (!input && (iftype() > 1)) {
    iftype(unset_int);
  }
  resize_data2(size);
}
void Trace::lpspol(const bool input) noexcept {
  bools[sac_map.at(name::lpspol)] = input;
}
void Trace::lovrok(const bool input) noexcept {
  bools[sac_map.at(name::lovrok)] = input;
}
void Trace::lcalda(const bool input) noexcept {
  bools[sac_map.at(name::lcalda)] = input;
}
// Strings
void Trace::kstnm(const std::string &input) noexcept {
  strings[sac_map.at(name::kstnm)] = input;
}
void Trace::kevnm(const std::string &input) noexcept {
  strings[sac_map.at(name::kevnm)] = input;
}
void Trace::khole(const std::string &input) noexcept {
  strings[sac_map.at(name::khole)] = input;
}
void Trace::ko(const std::string &input) noexcept {
  strings[sac_map.at(name::ko)] = input;
}
void Trace::ka(const std::string &input) noexcept {
  strings[sac_map.at(name::ka)] = input;
}
void Trace::kt0(const std::string &input) noexcept {
  strings[sac_map.at(name::kt0)] = input;
}
void Trace::kt1(const std::string &input) noexcept {
  strings[sac_map.at(name::kt1)] = input;
}
void Trace::kt2(const std::string &input) noexcept {
  strings[sac_map.at(name::kt2)] = input;
}
void Trace::kt3(const std::string &input) noexcept {
  strings[sac_map.at(name::kt3)] = input;
}
void Trace::kt4(const std::string &input) noexcept {
  strings[sac_map.at(name::kt4)] = input;
}
void Trace::kt5(const std::string &input) noexcept {
  strings[sac_map.at(name::kt5)] = input;
}
void Trace::kt6(const std::string &input) noexcept {
  strings[sac_map.at(name::kt6)] = input;
}
void Trace::kt7(const std::string &input) noexcept {
  strings[sac_map.at(name::kt7)] = input;
}
void Trace::kt8(const std::string &input) noexcept {
  strings[sac_map.at(name::kt8)] = input;
}
void Trace::kt9(const std::string &input) noexcept {
  strings[sac_map.at(name::kt9)] = input;
}
void Trace::kf(const std::string &input) noexcept {
  strings[sac_map.at(name::kf)] = input;
}
void Trace::kuser0(const std::string &input) noexcept {
  strings[sac_map.at(name::kuser0)] = input;
}
void Trace::kuser1(const std::string &input) noexcept {
  strings[sac_map.at(name::kuser1)] = input;
}
void Trace::kuser2(const std::string &input) noexcept {
  strings[sac_map.at(name::kuser2)] = input;
}
void Trace::kcmpnm(const std::string &input) noexcept {
  strings[sac_map.at(name::kcmpnm)] = input;
}
void Trace::knetwk(const std::string &input) noexcept {
  strings[sac_map.at(name::knetwk)] = input;
}
void Trace::kdatrd(const std::string &input) noexcept {
  strings[sac_map.at(name::kdatrd)] = input;
}
void Trace::kinst(const std::string &input) noexcept {
  strings[sac_map.at(name::kinst)] = input;
}

// Data
void Trace::data1(const std::vector<double> &input) noexcept {
  data[sac_map.at(name::data1)] = input;
  // Propagate change as needed
  int size{static_cast<int>(data1().size())};
  size = (((size == 0) && (npts() == unset_int)) ? unset_int : size);
  if (size != npts()) {
    npts(size);
  }
}

void Trace::data2(const std::vector<double> &input) noexcept {
  data[sac_map.at(name::data2)] = input;
  // Proagate change as needed
  int size{static_cast<int>(data2().size())};
  size = (((size == 0) && (npts() == unset_int)) ? unset_int : size);
  // Need to make sure this is legal
  // If positive size and not-legal, make spectral
  if (size > 0) {
    // If not legal, make spectral
    if (leven() && (iftype() <= 1)) {
      iftype(2);
    }
    // If legal and different from npts, update npts
    if ((!leven() || (iftype() > 1)) && (size != npts())) {
      npts(size);
    }
  }
}

void Trace::resize_data1(const size_t size) noexcept {
  if (size != data1().size()) {
    std::vector<double> new_data1{data1()};
    new_data1.resize(size, 0.0);
    data1(new_data1);
  }
}

void Trace::resize_data2(const size_t size) noexcept {
  // Data2 is legal
  if (!leven() || (iftype() > 1)) {
    if (size != data2().size()) {
      std::vector<double> new_data2{data2()};
      new_data2.resize(size, 0.0);
      data2(new_data2);
    }
  } else {
    if (!data2().empty()) {
      std::vector<double> new_data2{};
      data2(new_data2);
    }
  }
}

/*!
  \brief Resize data vectors (only if eligible).

  Will always resize data1, data2 only resizes if it can have non-zero size.
 */
void Trace::resize_data(const size_t size) noexcept {
  resize_data1(size);
  resize_data2(size);
}
//------------------------------------------------------------------------------
// Read
//------------------------------------------------------------------------------
/*!
  \brief Determine if the SAC-file has enough remaining data to read the
  requested amount of data.

  @param[in] sac std::ifstream* SAC-file to read.
  @param[in] spec read_spec reading specification.
  @returns bool Truth value (true = safe to read).
 */
bool nwords_after_current(std::ifstream *sac, const read_spec &spec) noexcept {
  bool result{false};
  if (sac->good()) {
    sac->seekg(0, std::ios::end);
    const std::size_t final_pos{static_cast<size_t>(sac->tellg())};
    // Doesn't like size_t since it wants to allow
    // the possibility of negative offsets (not how I use it)
    sac->seekg(static_cast<std::streamoff>(spec.start_word));
    const std::size_t diff{final_pos - spec.start_word};
    result = (diff >= (spec.num_words * word_length));
  }
  return result;
}

/*!
  \brief Determine if the SAC-file is large enough to contain a complete header.

  This must be run prior to reading the data vector(s) and footer (if
  applicable), not after.

  @param[in] sac std::ifstream* SAC-file to read.
  @throw io_error If unsafe to read.
 */
void safe_to_read_header(std::ifstream *sac) {
  const read_spec spec{data_word, 0};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for header.");
  }
}

/*!
  \brief Determines if the SAC-file has enough space remaining to contain a
  complete footer.

  This must be run after reading the header and data vector(s), not before.

  @param[in] sac std::ifstream* SAC-file to read.
  @throw io_error If unsafe to read.
 */
void safe_to_read_footer(std::ifstream *sac) {
  // doubles are two words long
  const read_spec spec{static_cast<size_t>(num_footer) * 2,
                       static_cast<size_t>(sac->tellg())};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for footer.");
  }
}

/*!
  \brief Determines if the SAC-file has enough space remaining to contain a
  complete data vector.

  This must be run after reading the header (and first data vector if
  applicable) and before the footer (if applicable).

  @param[in] sac std::ifstream* SAC-file to read.
  @param[in] n_words Number of values in data vector.
  @param[in] data2 bool True if reading data2, false (default) if reading data1.
  @throw io_error If unsafe to read.
 */
void safe_to_read_data(std::ifstream *sac, const size_t n_words,
                       const bool data2) {
  const std::string data{data2 ? "data2" : "data1"};
  const read_spec spec{n_words, static_cast<size_t>(sac->tellg())};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for " + data + '.');
  }
}

/*!
  \brief Determines if the SAC-file is finished.

  This must run after reading the header, data vector(s), and footer (if
  applicable). This checks to ensure there is no additional data in the SAC-file
  (there shouldn't be, and out of safety it throws an io_error to inform the
  user if there are shenanigans).

  @param[in] sac std::ifstream* SAC-file to be checked.
  @throw io_error If the file is not finished.
 */
void safe_to_finish_reading(std::ifstream *sac) {
  const std::streamoff current_pos{sac->tellg()};
  sac->seekg(0, std::ios::end);
  const std::streamoff end_pos{sac->tellg()};
  sac->seekg(current_pos, std::ios::beg);
  // How far are we from the end of the file?
  const std::streamoff diff{end_pos - current_pos};
  // If there is more, something weird happened...
  if (diff != 0) {
    std::ostringstream oss{};
    oss << "Filesize exceeds data specification with ";
    oss << diff;
    oss << " bytes excess. Data corruption suspected.";
    throw io_error(oss.str());
  }
}

void Trace::read_float_headers_starter(std::ifstream *sac_file) {
  delta(binary_to_float(read_word(sac_file)));
  depmin(binary_to_float(read_word(sac_file)));
  depmax(binary_to_float(read_word(sac_file)));
  // Skip 'unused'
  read_word(sac_file);
  odelta(binary_to_float(read_word(sac_file)));
  b(binary_to_float(read_word(sac_file)));
  e(binary_to_float(read_word(sac_file)));
  o(binary_to_float(read_word(sac_file)));
  a(binary_to_float(read_word(sac_file)));
  // Skip 'internal'
  read_word(sac_file);
}

void Trace::read_float_headers_t(std::ifstream *sac_file) {
  t0(binary_to_float(read_word(sac_file)));
  t1(binary_to_float(read_word(sac_file)));
  t2(binary_to_float(read_word(sac_file)));
  t3(binary_to_float(read_word(sac_file)));
  t4(binary_to_float(read_word(sac_file)));
  t5(binary_to_float(read_word(sac_file)));
  t6(binary_to_float(read_word(sac_file)));
  t7(binary_to_float(read_word(sac_file)));
  t8(binary_to_float(read_word(sac_file)));
  t9(binary_to_float(read_word(sac_file)));
  f(binary_to_float(read_word(sac_file)));
}

void Trace::read_float_headers_resp(std::ifstream *sac_file) {
  resp0(binary_to_float(read_word(sac_file)));
  resp1(binary_to_float(read_word(sac_file)));
  resp2(binary_to_float(read_word(sac_file)));
  resp3(binary_to_float(read_word(sac_file)));
  resp4(binary_to_float(read_word(sac_file)));
  resp5(binary_to_float(read_word(sac_file)));
  resp6(binary_to_float(read_word(sac_file)));
  resp7(binary_to_float(read_word(sac_file)));
  resp8(binary_to_float(read_word(sac_file)));
  resp9(binary_to_float(read_word(sac_file)));
}

void Trace::read_float_headers_station_event(std::ifstream *sac_file) {
  // Station headers
  stla(binary_to_float(read_word(sac_file)));
  stlo(binary_to_float(read_word(sac_file)));
  stel(binary_to_float(read_word(sac_file)));
  stdp(binary_to_float(read_word(sac_file)));
  // Event headers
  evla(binary_to_float(read_word(sac_file)));
  evlo(binary_to_float(read_word(sac_file)));
  evel(binary_to_float(read_word(sac_file)));
  evdp(binary_to_float(read_word(sac_file)));
  mag(binary_to_float(read_word(sac_file)));
}

void Trace::read_float_headers_user(std::ifstream *sac_file) {
  user0(binary_to_float(read_word(sac_file)));
  user1(binary_to_float(read_word(sac_file)));
  user2(binary_to_float(read_word(sac_file)));
  user3(binary_to_float(read_word(sac_file)));
  user4(binary_to_float(read_word(sac_file)));
  user5(binary_to_float(read_word(sac_file)));
  user6(binary_to_float(read_word(sac_file)));
  user7(binary_to_float(read_word(sac_file)));
  user8(binary_to_float(read_word(sac_file)));
  user9(binary_to_float(read_word(sac_file)));
}

void Trace::read_float_headers_geometry(std::ifstream *sac_file) {
  dist(binary_to_float(read_word(sac_file)));
  az(binary_to_float(read_word(sac_file)));
  baz(binary_to_float(read_word(sac_file)));
  gcarc(binary_to_float(read_word(sac_file)));
}

void Trace::read_float_headers_meta(std::ifstream *sac_file) {
  sb(binary_to_float(read_word(sac_file)));
  sdelta(binary_to_float(read_word(sac_file)));
  depmen(binary_to_float(read_word(sac_file)));
  cmpaz(binary_to_float(read_word(sac_file)));
  cmpinc(binary_to_float(read_word(sac_file)));
  xminimum(binary_to_float(read_word(sac_file)));
  xmaximum(binary_to_float(read_word(sac_file)));
  yminimum(binary_to_float(read_word(sac_file)));
  ymaximum(binary_to_float(read_word(sac_file)));
  // Skip 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    read_word(sac_file);
  }
}

void Trace::read_float_headers(std::ifstream *sac_file) {
  read_float_headers_starter(sac_file);
  read_float_headers_t(sac_file);
  read_float_headers_resp(sac_file);
  read_float_headers_station_event(sac_file);
  read_float_headers_user(sac_file);
  read_float_headers_geometry(sac_file);
  read_float_headers_meta(sac_file);
}

void Trace::read_int_headers_datetime(std::ifstream *sac_file) {
  nzyear(binary_to_int(read_word(sac_file)));
  nzjday(binary_to_int(read_word(sac_file)));
  nzhour(binary_to_int(read_word(sac_file)));
  nzmin(binary_to_int(read_word(sac_file)));
  nzsec(binary_to_int(read_word(sac_file)));
  nzmsec(binary_to_int(read_word(sac_file)));
}

void Trace::read_int_headers_meta(std::ifstream *sac_file) {
  nvhdr(binary_to_int(read_word(sac_file)));
  norid(binary_to_int(read_word(sac_file)));
  nevid(binary_to_int(read_word(sac_file)));
  npts(binary_to_int(read_word(sac_file)));
  nsnpts(binary_to_int(read_word(sac_file)));
  nwfid(binary_to_int(read_word(sac_file)));
  nxsize(binary_to_int(read_word(sac_file)));
  nysize(binary_to_int(read_word(sac_file)));
  // Skip 'unused'
  read_word(sac_file);
  iftype(binary_to_int(read_word(sac_file)));
  idep(binary_to_int(read_word(sac_file)));
  iztype(binary_to_int(read_word(sac_file)));
  // Skip 'unused'
  read_word(sac_file);
  iinst(binary_to_int(read_word(sac_file)));
  istreg(binary_to_int(read_word(sac_file)));
  ievreg(binary_to_int(read_word(sac_file)));
  ievtyp(binary_to_int(read_word(sac_file)));
  iqual(binary_to_int(read_word(sac_file)));
  isynth(binary_to_int(read_word(sac_file)));
  imagtyp(binary_to_int(read_word(sac_file)));
  imagsrc(binary_to_int(read_word(sac_file)));
  ibody(binary_to_int(read_word(sac_file)));
  // Skip 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    read_word(sac_file);
  }
}

void Trace::read_int_headers(std::ifstream *sac_file) {
  read_int_headers_datetime(sac_file);
  read_int_headers_meta(sac_file);
}

void Trace::read_bool_headers(std::ifstream *sac_file) {
  // Logical headers
  leven(binary_to_bool(read_word(sac_file)));
  lpspol(binary_to_bool(read_word(sac_file)));
  lovrok(binary_to_bool(read_word(sac_file)));
  lcalda(binary_to_bool(read_word(sac_file)));
  // Skip 'unused'
  read_word(sac_file);
}

void Trace::read_string_headers(std::ifstream *sac_file) {
  // KSTNM is 2 words (normal)
  kstnm(binary_to_string(read_two_words(sac_file)));
  // KEVNM is 4 words long (unique!)
  kevnm(binary_to_long_string(read_four_words(sac_file)));
  // All other 'K' headers are 2 words
  khole(binary_to_string(read_two_words(sac_file)));
  ko(binary_to_string(read_two_words(sac_file)));
  ka(binary_to_string(read_two_words(sac_file)));
  kt0(binary_to_string(read_two_words(sac_file)));
  kt1(binary_to_string(read_two_words(sac_file)));
  kt2(binary_to_string(read_two_words(sac_file)));
  kt3(binary_to_string(read_two_words(sac_file)));
  kt4(binary_to_string(read_two_words(sac_file)));
  kt5(binary_to_string(read_two_words(sac_file)));
  kt6(binary_to_string(read_two_words(sac_file)));
  kt7(binary_to_string(read_two_words(sac_file)));
  kt8(binary_to_string(read_two_words(sac_file)));
  kt9(binary_to_string(read_two_words(sac_file)));
  kf(binary_to_string(read_two_words(sac_file)));
  kuser0(binary_to_string(read_two_words(sac_file)));
  kuser1(binary_to_string(read_two_words(sac_file)));
  kuser2(binary_to_string(read_two_words(sac_file)));
  kcmpnm(binary_to_string(read_two_words(sac_file)));
  knetwk(binary_to_string(read_two_words(sac_file)));
  kdatrd(binary_to_string(read_two_words(sac_file)));
  kinst(binary_to_string(read_two_words(sac_file)));
}

void Trace::read_datas(std::ifstream *sac_file) {
  const bool is_data{npts() != unset_int};
  // data1
  const size_t n_words{static_cast<size_t>(npts())};
  if (is_data) {
    // false flags for data1
    safe_to_read_data(sac_file, n_words, false);  // throws io_error if unsafe
    const read_spec spec{n_words, data_word};
    // Originally floats, read as doubles
    data1(read_data(sac_file, spec));
  }
  // data2 (uneven or spectral data)
  if (is_data && (!leven() || (iftype() > 1))) {
    // true flags for data2
    safe_to_read_data(sac_file, n_words, true);  // throws io_error if unsafe
    const read_spec spec{n_words, data_word + static_cast<size_t>(npts())};
    data2(read_data(sac_file, spec));
  }
}

void Trace::read_footers(std::ifstream *sac_file) {
  delta(binary_to_double(read_two_words(sac_file)));
  b(binary_to_double(read_two_words(sac_file)));
  e(binary_to_double(read_two_words(sac_file)));
  o(binary_to_double(read_two_words(sac_file)));
  a(binary_to_double(read_two_words(sac_file)));
  t0(binary_to_double(read_two_words(sac_file)));
  t1(binary_to_double(read_two_words(sac_file)));
  t2(binary_to_double(read_two_words(sac_file)));
  t3(binary_to_double(read_two_words(sac_file)));
  t4(binary_to_double(read_two_words(sac_file)));
  t5(binary_to_double(read_two_words(sac_file)));
  t6(binary_to_double(read_two_words(sac_file)));
  t7(binary_to_double(read_two_words(sac_file)));
  t8(binary_to_double(read_two_words(sac_file)));
  t9(binary_to_double(read_two_words(sac_file)));
  f(binary_to_double(read_two_words(sac_file)));
  evlo(binary_to_double(read_two_words(sac_file)));
  evla(binary_to_double(read_two_words(sac_file)));
  stlo(binary_to_double(read_two_words(sac_file)));
  stla(binary_to_double(read_two_words(sac_file)));
  sb(binary_to_double(read_two_words(sac_file)));
  sdelta(binary_to_double(read_two_words(sac_file)));
}

/*!
  \brief Binary SAC-file reader.

  @param[in] path std::filesystem::path SAC-file to be read.
  @returns Trace read in-file.
  @throw io_error If the file is not safe to read for whatever reason.
  @throw std::exception (disk failure).
 */
Trace::Trace(const std::filesystem::path &path) {
  std::ifstream file(path, std::ifstream::binary);
  if (!file) {
    throw io_error(path.string() + " cannot be opened to read.");
  }
  safe_to_read_header(&file);  // throws io_error if not safe
  read_float_headers(&file);
  read_int_headers(&file);
  read_bool_headers(&file);
  read_string_headers(&file);
  read_datas(&file);
  if (nvhdr() == modern_hdr_version) {
    safe_to_read_footer(&file);  // throws io_error if not safe
    read_footers(&file);
  }
  safe_to_finish_reading(&file);  // throws io_error if the file isn't finished
  file.close();
}
//------------------------------------------------------------------------------
// Write
//------------------------------------------------------------------------------
void Trace::write_data(std::ofstream *sac_file,
                       const std::vector<double> &data_vec) {
  std::for_each(
      data_vec.begin(), data_vec.end(), [&sac_file](const auto &value) {
        write_words(sac_file, convert_to_word(static_cast<float>(value)));
      });
}

void Trace::write_float_headers_starter(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(static_cast<float>(delta())));
  write_words(sac_file, convert_to_word(depmin()));
  write_words(sac_file, convert_to_word(depmax()));
  // Fill 'unused'
  write_words(sac_file, convert_to_word(depmax()));
  write_words(sac_file, convert_to_word(odelta()));
  write_words(sac_file, convert_to_word(static_cast<float>(b())));
  write_words(sac_file, convert_to_word(static_cast<float>(e())));
  write_words(sac_file, convert_to_word(static_cast<float>(o())));
  write_words(sac_file, convert_to_word(static_cast<float>(a())));
  // Fill 'internal'
  write_words(sac_file, convert_to_word(depmin()));
}

void Trace::write_float_headers_t(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(static_cast<float>(t0())));
  write_words(sac_file, convert_to_word(static_cast<float>(t1())));
  write_words(sac_file, convert_to_word(static_cast<float>(t2())));
  write_words(sac_file, convert_to_word(static_cast<float>(t3())));
  write_words(sac_file, convert_to_word(static_cast<float>(t4())));
  write_words(sac_file, convert_to_word(static_cast<float>(t5())));
  write_words(sac_file, convert_to_word(static_cast<float>(t6())));
  write_words(sac_file, convert_to_word(static_cast<float>(t7())));
  write_words(sac_file, convert_to_word(static_cast<float>(t8())));
  write_words(sac_file, convert_to_word(static_cast<float>(t9())));
  write_words(sac_file, convert_to_word(static_cast<float>(f())));
}

void Trace::write_float_headers_resp(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(resp0()));
  write_words(sac_file, convert_to_word(resp1()));
  write_words(sac_file, convert_to_word(resp2()));
  write_words(sac_file, convert_to_word(resp3()));
  write_words(sac_file, convert_to_word(resp4()));
  write_words(sac_file, convert_to_word(resp5()));
  write_words(sac_file, convert_to_word(resp6()));
  write_words(sac_file, convert_to_word(resp7()));
  write_words(sac_file, convert_to_word(resp8()));
  write_words(sac_file, convert_to_word(resp9()));
}

void Trace::write_float_headers_station_event(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(static_cast<float>(stla())));
  write_words(sac_file, convert_to_word(static_cast<float>(stlo())));
  write_words(sac_file, convert_to_word(stel()));
  write_words(sac_file, convert_to_word(stdp()));
  write_words(sac_file, convert_to_word(static_cast<float>(evla())));
  write_words(sac_file, convert_to_word(static_cast<float>(evlo())));
  write_words(sac_file, convert_to_word(evel()));
  write_words(sac_file, convert_to_word(evdp()));
  write_words(sac_file, convert_to_word(mag()));
}

void Trace::write_float_headers_user(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(user0()));
  write_words(sac_file, convert_to_word(user1()));
  write_words(sac_file, convert_to_word(user2()));
  write_words(sac_file, convert_to_word(user3()));
  write_words(sac_file, convert_to_word(user4()));
  write_words(sac_file, convert_to_word(user5()));
  write_words(sac_file, convert_to_word(user6()));
  write_words(sac_file, convert_to_word(user7()));
  write_words(sac_file, convert_to_word(user8()));
  write_words(sac_file, convert_to_word(user9()));
}

void Trace::write_float_headers_geometry(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(dist()));
  write_words(sac_file, convert_to_word(az()));
  write_words(sac_file, convert_to_word(baz()));
  write_words(sac_file, convert_to_word(gcarc()));
}

void Trace::write_float_headers_meta(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(static_cast<float>(sb())));
  write_words(sac_file, convert_to_word(static_cast<float>(sdelta())));
  write_words(sac_file, convert_to_word(depmen()));
  write_words(sac_file, convert_to_word(cmpaz()));
  write_words(sac_file, convert_to_word(cmpinc()));
  write_words(sac_file, convert_to_word(xminimum()));
  write_words(sac_file, convert_to_word(xmaximum()));
  write_words(sac_file, convert_to_word(yminimum()));
  write_words(sac_file, convert_to_word(ymaximum()));
  // Fill 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    write_words(sac_file, convert_to_word(az()));
  }
}

void Trace::write_float_headers(std::ofstream *sac_file) const {
  write_float_headers_starter(sac_file);
  write_float_headers_t(sac_file);
  write_float_headers_resp(sac_file);
  write_float_headers_station_event(sac_file);
  write_float_headers_user(sac_file);
  write_float_headers_geometry(sac_file);
  write_float_headers_meta(sac_file);
}

void Trace::write_int_headers_datetime(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(nzyear()));
  write_words(sac_file, convert_to_word(nzjday()));
  write_words(sac_file, convert_to_word(nzhour()));
  write_words(sac_file, convert_to_word(nzmin()));
  write_words(sac_file, convert_to_word(nzsec()));
  write_words(sac_file, convert_to_word(nzmsec()));
}

void Trace::write_int_headers_meta(std::ofstream *sac_file,
                                   const int hdr_ver) const {
  write_words(sac_file, convert_to_word(hdr_ver));
  write_words(sac_file, convert_to_word(norid()));
  write_words(sac_file, convert_to_word(nevid()));
  write_words(sac_file, convert_to_word(npts()));
  write_words(sac_file, convert_to_word(nsnpts()));
  write_words(sac_file, convert_to_word(nwfid()));
  write_words(sac_file, convert_to_word(nxsize()));
  write_words(sac_file, convert_to_word(nysize()));
  // Fill 'unused'
  write_words(sac_file, convert_to_word(nysize()));
  write_words(sac_file, convert_to_word(iftype()));
  write_words(sac_file, convert_to_word(idep()));
  write_words(sac_file, convert_to_word(iztype()));
  // Fill 'unused'
  write_words(sac_file, convert_to_word(iztype()));
  write_words(sac_file, convert_to_word(iinst()));
  write_words(sac_file, convert_to_word(istreg()));
  write_words(sac_file, convert_to_word(ievreg()));
  write_words(sac_file, convert_to_word(ievtyp()));
  write_words(sac_file, convert_to_word(iqual()));
  write_words(sac_file, convert_to_word(isynth()));
  write_words(sac_file, convert_to_word(imagtyp()));
  write_words(sac_file, convert_to_word(imagsrc()));
  write_words(sac_file, convert_to_word(ibody()));
  // Fill 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    write_words(sac_file, convert_to_word(ibody()));
  }
}

void Trace::write_int_headers(std::ofstream *sac_file,
                              const int hdr_ver) const {
  write_int_headers_datetime(sac_file);
  write_int_headers_meta(sac_file, hdr_ver);
}

void Trace::write_bool_headers(std::ofstream *sac_file) const {
  write_words(sac_file, bool_to_word(leven()));
  write_words(sac_file, bool_to_word(lpspol()));
  write_words(sac_file, bool_to_word(lovrok()));
  write_words(sac_file, bool_to_word(lcalda()));
  // Fill 'unused'
  write_words(sac_file, bool_to_word(lcalda()));
}

void Trace::write_string_headers(std::ofstream *sac_file) const {
  // Strings are special
  std::array<char, static_cast<size_t>(2) * word_length> two_words{
      convert_to_words<sizeof(two_words)>(kstnm(), 2)};
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  std::array<char, static_cast<size_t>(4) * word_length> four_words{
      convert_to_words<sizeof(four_words)>(kevnm(), 4)};
  write_words(sac_file,
              std::vector<char>(four_words.begin(), four_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(khole(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ko(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ka(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt0(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt1(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt2(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt3(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt4(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt5(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt6(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt7(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt8(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt9(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kf(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser0(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser1(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser2(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kcmpnm(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(knetwk(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kdatrd(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kinst(), 2);
  write_words(sac_file, std::vector<char>(two_words.begin(), two_words.end()));
}

void Trace::write_footers(std::ofstream *sac_file) const {
  write_words(sac_file, convert_to_word(delta()));
  write_words(sac_file, convert_to_word(b()));
  write_words(sac_file, convert_to_word(e()));
  write_words(sac_file, convert_to_word(o()));
  write_words(sac_file, convert_to_word(a()));
  write_words(sac_file, convert_to_word(t0()));
  write_words(sac_file, convert_to_word(t1()));
  write_words(sac_file, convert_to_word(t2()));
  write_words(sac_file, convert_to_word(t3()));
  write_words(sac_file, convert_to_word(t4()));
  write_words(sac_file, convert_to_word(t5()));
  write_words(sac_file, convert_to_word(t6()));
  write_words(sac_file, convert_to_word(t7()));
  write_words(sac_file, convert_to_word(t8()));
  write_words(sac_file, convert_to_word(t9()));
  write_words(sac_file, convert_to_word(f()));
  write_words(sac_file, convert_to_word(evlo()));
  write_words(sac_file, convert_to_word(evla()));
  write_words(sac_file, convert_to_word(stlo()));
  write_words(sac_file, convert_to_word(stla()));
  write_words(sac_file, convert_to_word(sb()));
  write_words(sac_file, convert_to_word(sdelta()));
}

/*!
  \brief Binary SAC-file writer.

  @param[in] path std::filesystem::path SAC-file to write.
  @param[in] legacy bool Legacy-write flag (default false = v7, true = v6).
  @throw io_error If the file cannot be written (bad path or bad permissions).
  @throw std::exception Other unwritable issues (not enough space, disk failure,
  etc.).
 */
void Trace::write(const std::filesystem::path &path, const bool legacy) const {
  std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
  if (!file) {
    throw io_error(path.string() + " cannot be opened to write.");
  }
  const int header_version{legacy ? old_hdr_version : modern_hdr_version};
  write_float_headers(&file);
  write_int_headers(&file, header_version);
  write_bool_headers(&file);
  write_string_headers(&file);
  // Data
  std::vector<double> tmp{data1()};
  write_data(&file, tmp);
  if (!leven() || (iftype() > 1)) {
    tmp = data2();
    write_data(&file, tmp);
  }
  if (header_version == modern_hdr_version) {
    // Write footer
    write_footers(&file);
  }
  file.close();
}

/*!
  \brief Binary SAC-file legacy-write convenience function.

  @param[in] path std::filesystem::path SAC-file to be written.
  @throw io_error If the file cannot be written (bad path or bad permissions).
  @throw std::execption Other unwritable issues (not enough space, disk failure,
  etc.).
 */
void Trace::legacy_write(const std::filesystem::path &path) const {
  write(path, true);
}
};  // namespace sacfmt
