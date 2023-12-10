// Copyright 2023 Alexander R. Blanchette

#include <sac_format.hpp>

// Implementation of the interface in sac_format.hpp
namespace sacfmt {
//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
std::streamoff word_position(const size_t word_number) noexcept {
  return static_cast<std::streamoff>(word_number * word_length);
}

word_one int_to_binary(const int num) noexcept {
  word_one bits{};
  if (num >= 0) {
    bits = word_one(static_cast<size_t>(num));
  } else {
    bits = word_one(static_cast<size_t>(std::pow(2, binary_word_size) + num));
  }
  return bits;
}

int binary_to_int(word_one bin) noexcept {
  int result{};
  if (bin.test(binary_word_size - 1)) {
    bin = ~bin;
    result = static_cast<int>(bin.to_ulong());
    result += 1;
    // Change sign to make it negative
    result *= -1;
  } else {
    result = static_cast<int>(bin.to_ulong());
  }
  return result;
}

word_one float_to_binary(const float num) noexcept {
  unsigned_int<float> num_as_uint{0};
  // flawfinder: ignore
  std::memcpy(&num_as_uint, &num, sizeof(float));
  word_one result{num_as_uint};
  return result;
}

float binary_to_float(const word_one &bin) noexcept {
  const auto val = bin.to_ulong();
  float result{};
  // flawfinder: ignore
  memcpy(&result, &val, sizeof(float));
  return result;
}

word_two double_to_binary(const double num) noexcept {
  unsigned_int<double> num_as_uint{0};
  // flawfinder: ignore
  std::memcpy(&num_as_uint, &num, sizeof(double));
  word_two result{num_as_uint};
  return result;
}

double binary_to_double(const word_two &bin) noexcept {
  const auto val = bin.to_ullong();
  double result{};
  // flawfinder: ignore
  memcpy(&result, &val, sizeof(double));
  return result;
}

void remove_leading_spaces(std::string *str) noexcept {
  while ((static_cast<int>(str->front()) <= ascii_space) && (!str->empty())) {
    str->erase(0, 1);
  }
}
void remove_trailing_spaces(std::string *str) noexcept {
  while ((static_cast<int>(str->back()) <= ascii_space) && (!str->empty())) {
    str->pop_back();
  }
}

// Remove leading/trailing white-space and control characters
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

void prep_string(std::string *str, const size_t str_size) noexcept {
  *str = string_cleaning(*str);
  if (str->length() > str_size) {
    str->resize(str_size);
  } else if (str->length() < str_size) {
    *str = str->append(str_size - str->length(), ' ');
  }
}

template <typename T>
void string_bits(T *bits, const std::string &str,
                 const size_t str_size) noexcept {
  constexpr size_t char_size{bits_per_byte};
  std::bitset<char_size> byte{};
  for (size_t i{0}; i < str_size; ++i) {
    size_t character{static_cast<size_t>(str[i])};
    byte = std::bitset<char_size>(character);
    for (size_t j{0}; j < char_size; ++j) {
      (*bits)[(i * char_size) + j] = byte[j];
    }
  }
}

template <typename T>
std::string bits_string(const T &bits, const size_t num_words) noexcept {
  std::string result{};
  result.reserve(num_words * word_length);
  constexpr size_t char_size{bits_per_byte};
  std::bitset<char_size> byte{};
  for (size_t i{0}; i < num_words * binary_word_size; i += char_size) {
    for (size_t j{0}; j < char_size; ++j) [[likely]] {
      byte[j] = bits[i + j];
    }
    result.push_back(static_cast<char>(byte.to_ulong()));
  }
  return result;
}

word_two string_to_binary(std::string str) noexcept {
  constexpr size_t string_size{static_cast<size_t>(2 * word_length)};
  // 1 byte per character
  prep_string(&str, string_size);
  // Two words (8 characters)
  word_two bits{};
  string_bits(&bits, str, string_size);
  return bits;
}

std::string binary_to_string(const word_two &str) noexcept {
  std::string result{bits_string(str, 2)};
  return string_cleaning(result);
}

word_four long_string_to_binary(std::string str) noexcept {
  constexpr size_t string_size{static_cast<size_t>(4 * word_length)};
  prep_string(&str, string_size);
  // Four words (16 characters)
  word_four bits{};
  string_bits(&bits, str, string_size);
  return bits;
}

std::string binary_to_long_string(const word_four &str) noexcept {
  std::string result{bits_string(str, 4)};
  return string_cleaning(result);
}

word_one bool_to_binary(const bool flag) noexcept {
  word_one result{};
  result[0] = flag;
  return result;
}

bool binary_to_bool(const word_one &flag) noexcept { return flag[0]; }

word_two concat_words(const word_pair<word_one> &pair_words) noexcept {
  word_two result{};
  for (size_t i{0}; i < binary_word_size; ++i) [[likely]] {
    result[i] = pair_words.first[i];
    result[i + binary_word_size] = pair_words.second[i];
  }
  return result;
}

word_four concat_words(const word_pair<word_two> &pair_words) noexcept {
  word_four result{};
  for (int i{0}; i < 2 * binary_word_size; ++i) [[likely]] {
    result[i] = pair_words.first[i];
    result[i + (2 * binary_word_size)] = pair_words.second[i];
  }
  return result;
}
//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
word_one read_word(std::ifstream *sac) {
  word_one bits{};
  constexpr int char_size{bits_per_byte};
  // Where we will store the characters
  std::array<char, word_length> word{};
  // Read to our character array
  // This can always hold the source due to careful typing/sizing
  // flawfinder: ignore
  if (sac->read(word.data(), word_length)) {
    // Take each character
    std::bitset<char_size> byte{};
    for (int i{0}; i < word_length; ++i) [[likely]] {
      int character{word[i]};
      byte = std::bitset<char_size>(character);
      // bit-by-bit
      for (int j{0}; j < char_size; ++j) [[likely]] {
        bits[(i * char_size) + j] = byte[j];
      }
    }
  }
  return bits;
}

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

std::vector<double> read_data(std::ifstream *sac, const read_spec &spec) {
  sac->seekg(word_position(spec.start_word));
  std::vector<double> result{};
  result.resize(spec.num_words);
  for (size_t i{0}; i < spec.num_words; ++i) [[likely]] {
    result[i] = static_cast<double>(binary_to_float(read_word(sac)));
  }
  return result;
}
//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
void write_words(std::ofstream *sac_file, const std::vector<char> &input) {
  std::ofstream &sac = *sac_file;
  if (sac.is_open()) {
    for (char character : input) [[likely]] {
      sac.write(&character, sizeof(char));
    }
  }
}

// Template on the typename to make possible to handle float or int
template <typename T>
std::vector<char> convert_to_word(const T input) noexcept {
  std::array<char, word_length> tmp{};
  // Copy bytes from input into the tmp array
  // flawfinder: ignore
  std::memcpy(tmp.data(), &input, word_length);
  std::vector<char> word{};
  word.resize(word_length);
  for (int i{0}; i < word_length; ++i) [[likely]] {
    word[i] = tmp[i];
  }
  return word;
}

// Explicit instantiation
template std::vector<char> convert_to_word(const float input) noexcept;
template std::vector<char> convert_to_word(const int x) noexcept;

std::vector<char> convert_to_word(const double input) noexcept {
  std::array<char, static_cast<size_t>(2) * word_length> tmp{};
  // Copy bytes from input into the tmp array
  // flawfinder: ignore
  std::memcpy(tmp.data(), &input, static_cast<size_t>(2) * word_length);
  std::vector<char> word{};
  word.resize(static_cast<size_t>(2) * word_length);
  for (int i{0}; i < 2 * word_length; ++i) {
    word[static_cast<size_t>(i)] = tmp[i];
  }
  return word;
}

// Variable sized words for the 'K' headers
template <size_t N>
std::array<char, N> convert_to_words(const std::string &str,
                                     int n_words) noexcept {
  std::array<char, N> all_words{};
  // String to null-terminated character array
  const char *c_str = str.c_str();
  for (int i{0}; i < (n_words * word_length); ++i) {
    all_words[static_cast<size_t>(i)] = c_str[i];
  }
  return all_words;
}

// Explicit instantiation
template std::array<char, word_length>
convert_to_words(const std::string &str, const int n_words) noexcept;
template std::array<char, 2 * word_length>
convert_to_words(const std::string &str, const int n_words) noexcept;
template std::array<char, 4 * word_length>
convert_to_words(const std::string &str, const int n_words) noexcept;

std::vector<char> bool_to_word(const bool flag) noexcept {
  std::vector<char> result;
  result.resize(word_length);
  result[0] = static_cast<char>(flag ? 1 : 0);
  for (int i{1}; i < word_length; ++i) {
    result[i] = 0;
  }
  return result;
}
//-----------------------------------------------------------------------------
// Convenience methods
// -----------------------------------------------------------------------------
// Does not assume equal length, if not equal length then they're not equal
// within tolerance
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
bool equal_within_tolerance(const double val1, const double val2,
                            const double tolerance) noexcept {
  return (std::abs(val1 - val2) < tolerance);
}
// Position methods
double degrees_to_radians(const double degrees) noexcept {
  return rad_per_deg * degrees;
}

double radians_to_degrees(const double radians) noexcept {
  return deg_per_rad * radians;
}

double gcarc(const double latitude1, const double longitude1,
             const double latitude2, const double longitude2) noexcept {
  const double lat1{degrees_to_radians(latitude1)};
  const double lon1{degrees_to_radians(longitude1)};
  const double lat2{degrees_to_radians(latitude2)};
  const double lon2{degrees_to_radians(longitude2)};
  double result{radians_to_degrees(
      std::acos(std::sin(lat1) * std::sin(lat2) +
                std::cos(lat1) * std::cos(lat2) * std::cos(lon2 - lon1)))};
  return result;
}

// I wonder if there is a way to do this with n-vectors
double azimuth(const double latitude1, const double longitude1,
               const double latitude2, const double longitude2) noexcept {
  const double lat1{degrees_to_radians(latitude1)};
  const double lon1{degrees_to_radians(longitude1)};
  const double lat2{degrees_to_radians(latitude2)};
  const double lon2{degrees_to_radians(longitude2)};
  const double dlon{lon2 - lon1};
  const double numerator{std::sin(dlon) * std::cos(lat2)};
  const double denominator{(std::cos(lat1) * std::sin(lat2)) -
                           (std::sin(lat1) * std::cos(lat2) * std::cos(dlon))};
  double result{radians_to_degrees(std::atan2(numerator, denominator))};
  while (result < 0.0) {
    result += circle_deg;
  }
  return result;
}

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

double limit_180(const double degrees) noexcept {
  double result{limit_360(degrees)};
  constexpr double hemi{180.0};
  if (result > hemi) {
    result = result - circle_deg;
  }
  return result;
}

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
Trace::Trace() noexcept {
  std::ranges::fill(floats.begin(), floats.end(), unset_float);
  std::ranges::fill(doubles.begin(), doubles.end(), unset_double);
  std::ranges::fill(ints.begin(), ints.end(), unset_int);
  std::ranges::fill(bools.begin(), bools.end(), unset_bool);
  std::ranges::fill(strings.begin(), strings.end(), unset_word);
}

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

double Trace::frequency() const noexcept {
  const double delta_val{delta()};
  if ((delta_val == unset_double) || (delta_val <= 0)) {
    return unset_double;
  }
  return 1.0 / delta_val;
}

bool Trace::geometry_set() const noexcept {
  return ((stla() != unset_double) && (stlo() != unset_double) &&
          (evla() != unset_double) && (evlo() != unset_double));
}

void Trace::calc_gcarc() noexcept {
  Trace::gcarc(
      static_cast<float>(sacfmt::gcarc(stla(), stlo(), evla(), evlo())));
}

void Trace::calc_dist() noexcept {
  dist(static_cast<float>(earth_radius * rad_per_deg * gcarc()));
}

void Trace::calc_az() noexcept {
  az(static_cast<float>(azimuth(evla(), evlo(), stla(), stlo())));
}
void Trace::calc_baz() noexcept {
  baz(static_cast<float>(azimuth(stla(), stlo(), evla(), evlo())));
}

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
void Trace::stla(double input) noexcept {
  if (input != unset_double) {
    input = limit_90(input);
  }
  doubles[sac_map.at(name::stla)] = input;
}
void Trace::stlo(double input) noexcept {
  if (input != unset_double) {
    input = limit_180(input);
  }
  doubles[sac_map.at(name::stlo)] = input;
}
void Trace::evla(double input) noexcept {
  if (input != unset_double) {
    input = limit_90(input);
  }
  doubles[sac_map.at(name::evla)] = input;
}
void Trace::evlo(double input) noexcept {
  if (input != unset_double) {
    input = limit_180(input);
  }
  doubles[sac_map.at(name::evlo)] = input;
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
  const int size{npts() >= 0 ? npts() : 0};
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
  const int size{npts() >= 0 ? npts() : 0};
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
  size_t size{data1().size()};
  size = (((size == 0) && (npts() == unset_int)) ? unset_int : size);
  if (static_cast<int>(size) != npts()) {
    npts(static_cast<int>(size));
  }
}
void Trace::data2(const std::vector<double> &input) noexcept {
  data[sac_map.at(name::data2)] = input;
  // Proagate change as needed
  size_t size{data2().size()};
  size = (((size == 0) && (npts() == unset_int)) ? unset_int : size);
  // Need to make sure this is legal
  // If positive size and not-legal, make spectral
  if (size > 0) {
    // If not legal, make spectral
    if (leven() && (iftype() <= 1)) {
      iftype(2);
    }
    // If legal and different from npts, update npts
    if ((!leven() || (iftype() > 1)) && (static_cast<int>(size) != npts())) {
      npts(static_cast<int>(size));
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

void Trace::resize_data(const size_t size) noexcept {
  resize_data1(size);
  resize_data2(size);
}
//------------------------------------------------------------------------------
// Read
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

void safe_to_read_header(std::ifstream *sac) {
  const read_spec spec{data_word, 0};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for header.");
  }
}

void safe_to_read_footer(std::ifstream *sac) {
  // doubles are two words long
  const read_spec spec{static_cast<size_t>(num_footer) * 2,
                       static_cast<size_t>(sac->tellg())};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for footer.");
  }
}

void safe_to_read_data(std::ifstream *sac, const size_t n_words,
                       const bool data2) {
  const std::string data{data2 ? "data2" : "data1"};
  const read_spec spec{n_words, static_cast<size_t>(sac->tellg())};
  if (!nwords_after_current(sac, spec)) {
    throw io_error("Insufficient filesize for " + data + '.');
  }
}

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

Trace::Trace(const std::filesystem::path &path) {
  std::ifstream file(path, std::ifstream::binary);
  if (!file) {
    throw io_error(path.string() + " cannot be opened to read.");
  }
  safe_to_read_header(&file); // throws io_error if not safe
  //--------------------------------------------------------------------------
  // Header
  delta(binary_to_float(read_word(&file)));
  depmin(binary_to_float(read_word(&file)));
  depmax(binary_to_float(read_word(&file)));
  // Skip 'unused'
  read_word(&file);
  odelta(binary_to_float(read_word(&file)));
  b(binary_to_float(read_word(&file)));
  e(binary_to_float(read_word(&file)));
  o(binary_to_float(read_word(&file)));
  a(binary_to_float(read_word(&file)));
  // Skip 'internal'
  read_word(&file);
  // T# pick headers
  t0(binary_to_float(read_word(&file)));
  t1(binary_to_float(read_word(&file)));
  t2(binary_to_float(read_word(&file)));
  t3(binary_to_float(read_word(&file)));
  t4(binary_to_float(read_word(&file)));
  t5(binary_to_float(read_word(&file)));
  t6(binary_to_float(read_word(&file)));
  t7(binary_to_float(read_word(&file)));
  t8(binary_to_float(read_word(&file)));
  t9(binary_to_float(read_word(&file)));
  f(binary_to_float(read_word(&file)));
  // Response headers
  resp0(binary_to_float(read_word(&file)));
  resp1(binary_to_float(read_word(&file)));
  resp2(binary_to_float(read_word(&file)));
  resp3(binary_to_float(read_word(&file)));
  resp4(binary_to_float(read_word(&file)));
  resp5(binary_to_float(read_word(&file)));
  resp6(binary_to_float(read_word(&file)));
  resp7(binary_to_float(read_word(&file)));
  resp8(binary_to_float(read_word(&file)));
  resp9(binary_to_float(read_word(&file)));
  // Station headers
  stla(binary_to_float(read_word(&file)));
  stlo(binary_to_float(read_word(&file)));
  stel(binary_to_float(read_word(&file)));
  stdp(binary_to_float(read_word(&file)));
  // Event headers
  evla(binary_to_float(read_word(&file)));
  evlo(binary_to_float(read_word(&file)));
  evel(binary_to_float(read_word(&file)));
  evdp(binary_to_float(read_word(&file)));
  mag(binary_to_float(read_word(&file)));
  // User misc headers
  user0(binary_to_float(read_word(&file)));
  user1(binary_to_float(read_word(&file)));
  user2(binary_to_float(read_word(&file)));
  user3(binary_to_float(read_word(&file)));
  user4(binary_to_float(read_word(&file)));
  user5(binary_to_float(read_word(&file)));
  user6(binary_to_float(read_word(&file)));
  user7(binary_to_float(read_word(&file)));
  user8(binary_to_float(read_word(&file)));
  user9(binary_to_float(read_word(&file)));
  // Geometry headers
  dist(binary_to_float(read_word(&file)));
  az(binary_to_float(read_word(&file)));
  baz(binary_to_float(read_word(&file)));
  gcarc(binary_to_float(read_word(&file)));
  // Metadata headers
  sb(binary_to_float(read_word(&file)));
  sdelta(binary_to_float(read_word(&file)));
  depmen(binary_to_float(read_word(&file)));
  cmpaz(binary_to_float(read_word(&file)));
  cmpinc(binary_to_float(read_word(&file)));
  xminimum(binary_to_float(read_word(&file)));
  xmaximum(binary_to_float(read_word(&file)));
  yminimum(binary_to_float(read_word(&file)));
  ymaximum(binary_to_float(read_word(&file)));
  // Skip 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    read_word(&file);
  }
  // Date/time headers
  nzyear(binary_to_int(read_word(&file)));
  nzjday(binary_to_int(read_word(&file)));
  nzhour(binary_to_int(read_word(&file)));
  nzmin(binary_to_int(read_word(&file)));
  nzsec(binary_to_int(read_word(&file)));
  nzmsec(binary_to_int(read_word(&file)));
  // More metadata headers
  nvhdr(binary_to_int(read_word(&file)));
  norid(binary_to_int(read_word(&file)));
  nevid(binary_to_int(read_word(&file)));
  npts(binary_to_int(read_word(&file)));
  nsnpts(binary_to_int(read_word(&file)));
  nwfid(binary_to_int(read_word(&file)));
  nxsize(binary_to_int(read_word(&file)));
  nysize(binary_to_int(read_word(&file)));
  // Skip 'unused'
  read_word(&file);
  iftype(binary_to_int(read_word(&file)));
  idep(binary_to_int(read_word(&file)));
  iztype(binary_to_int(read_word(&file)));
  // Skip 'unused'
  read_word(&file);
  iinst(binary_to_int(read_word(&file)));
  istreg(binary_to_int(read_word(&file)));
  ievreg(binary_to_int(read_word(&file)));
  ievtyp(binary_to_int(read_word(&file)));
  iqual(binary_to_int(read_word(&file)));
  isynth(binary_to_int(read_word(&file)));
  imagtyp(binary_to_int(read_word(&file)));
  imagsrc(binary_to_int(read_word(&file)));
  ibody(binary_to_int(read_word(&file)));
  // Skip 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    read_word(&file);
  }
  // Logical headers
  leven(binary_to_bool(read_word(&file)));
  lpspol(binary_to_bool(read_word(&file)));
  lovrok(binary_to_bool(read_word(&file)));
  lcalda(binary_to_bool(read_word(&file)));
  // Skip 'unused'
  read_word(&file);
  // KSTNM is 2 words (normal)
  kstnm(binary_to_string(read_two_words(&file)));
  // KEVNM is 4 words long (unique!)
  kevnm(binary_to_long_string(read_four_words(&file)));
  // All other 'K' headers are 2 words
  khole(binary_to_string(read_two_words(&file)));
  ko(binary_to_string(read_two_words(&file)));
  ka(binary_to_string(read_two_words(&file)));
  kt0(binary_to_string(read_two_words(&file)));
  kt1(binary_to_string(read_two_words(&file)));
  kt2(binary_to_string(read_two_words(&file)));
  kt3(binary_to_string(read_two_words(&file)));
  kt4(binary_to_string(read_two_words(&file)));
  kt5(binary_to_string(read_two_words(&file)));
  kt6(binary_to_string(read_two_words(&file)));
  kt7(binary_to_string(read_two_words(&file)));
  kt8(binary_to_string(read_two_words(&file)));
  kt9(binary_to_string(read_two_words(&file)));
  kf(binary_to_string(read_two_words(&file)));
  kuser0(binary_to_string(read_two_words(&file)));
  kuser1(binary_to_string(read_two_words(&file)));
  kuser2(binary_to_string(read_two_words(&file)));
  kcmpnm(binary_to_string(read_two_words(&file)));
  knetwk(binary_to_string(read_two_words(&file)));
  kdatrd(binary_to_string(read_two_words(&file)));
  kinst(binary_to_string(read_two_words(&file)));
  //--------------------------------------------------------------------------
  // DATA
  const bool is_data{npts() != unset_int};
  // data1
  const size_t n_words{static_cast<size_t>(npts())};
  if (is_data) {
    // false flags for data1
    safe_to_read_data(&file, n_words, false); // throws io_error if unsafe
    const read_spec spec{n_words, data_word};
    // Originally floats, read as doubles
    data1(read_data(&file, spec));
  }
  // data2 (uneven or spectral data)
  if (is_data && (!leven() || (iftype() > 1))) {
    // true flags for data2
    safe_to_read_data(&file, n_words, true); // throws io_error if unsafe
    const read_spec spec{n_words, static_cast<size_t>(data_word) + npts()};
    data2(read_data(&file, spec));
  }
  //--------------------------------------------------------------------------
  // Footer
  if (nvhdr() == modern_hdr_version) {
    safe_to_read_footer(&file); // throws io_error if not safe
    delta(binary_to_double(read_two_words(&file)));
    b(binary_to_double(read_two_words(&file)));
    e(binary_to_double(read_two_words(&file)));
    o(binary_to_double(read_two_words(&file)));
    a(binary_to_double(read_two_words(&file)));
    t0(binary_to_double(read_two_words(&file)));
    t1(binary_to_double(read_two_words(&file)));
    t2(binary_to_double(read_two_words(&file)));
    t3(binary_to_double(read_two_words(&file)));
    t4(binary_to_double(read_two_words(&file)));
    t5(binary_to_double(read_two_words(&file)));
    t6(binary_to_double(read_two_words(&file)));
    t7(binary_to_double(read_two_words(&file)));
    t8(binary_to_double(read_two_words(&file)));
    t9(binary_to_double(read_two_words(&file)));
    f(binary_to_double(read_two_words(&file)));
    evlo(binary_to_double(read_two_words(&file)));
    evla(binary_to_double(read_two_words(&file)));
    stlo(binary_to_double(read_two_words(&file)));
    stla(binary_to_double(read_two_words(&file)));
    sb(binary_to_double(read_two_words(&file)));
    sdelta(binary_to_double(read_two_words(&file)));
  }
  safe_to_finish_reading(&file); // throws io_error if the file isn't finished
  file.close();
}
//------------------------------------------------------------------------------
// Write
void Trace::write(const std::filesystem::path &path, const bool legacy) const {
  std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
  if (!file) {
    throw io_error(path.string() + " cannot be opened to write.");
  }
  const int header_version{legacy ? old_hdr_version : modern_hdr_version};
  write_words(&file, convert_to_word(static_cast<float>(delta())));
  write_words(&file, convert_to_word(depmin()));
  write_words(&file, convert_to_word(depmax()));
  // Fill 'unused'
  write_words(&file, convert_to_word(depmax()));
  write_words(&file, convert_to_word(odelta()));
  write_words(&file, convert_to_word(static_cast<float>(b())));
  write_words(&file, convert_to_word(static_cast<float>(e())));
  write_words(&file, convert_to_word(static_cast<float>(o())));
  write_words(&file, convert_to_word(static_cast<float>(a())));
  // Fill 'internal'
  write_words(&file, convert_to_word(depmin()));
  write_words(&file, convert_to_word(static_cast<float>(t0())));
  write_words(&file, convert_to_word(static_cast<float>(t1())));
  write_words(&file, convert_to_word(static_cast<float>(t2())));
  write_words(&file, convert_to_word(static_cast<float>(t3())));
  write_words(&file, convert_to_word(static_cast<float>(t4())));
  write_words(&file, convert_to_word(static_cast<float>(t5())));
  write_words(&file, convert_to_word(static_cast<float>(t6())));
  write_words(&file, convert_to_word(static_cast<float>(t7())));
  write_words(&file, convert_to_word(static_cast<float>(t8())));
  write_words(&file, convert_to_word(static_cast<float>(t9())));
  write_words(&file, convert_to_word(static_cast<float>(f())));
  write_words(&file, convert_to_word(resp0()));
  write_words(&file, convert_to_word(resp1()));
  write_words(&file, convert_to_word(resp2()));
  write_words(&file, convert_to_word(resp3()));
  write_words(&file, convert_to_word(resp4()));
  write_words(&file, convert_to_word(resp5()));
  write_words(&file, convert_to_word(resp6()));
  write_words(&file, convert_to_word(resp7()));
  write_words(&file, convert_to_word(resp8()));
  write_words(&file, convert_to_word(resp9()));
  write_words(&file, convert_to_word(static_cast<float>(stla())));
  write_words(&file, convert_to_word(static_cast<float>(stlo())));
  write_words(&file, convert_to_word(stel()));
  write_words(&file, convert_to_word(stdp()));
  write_words(&file, convert_to_word(static_cast<float>(evla())));
  write_words(&file, convert_to_word(static_cast<float>(evlo())));
  write_words(&file, convert_to_word(evel()));
  write_words(&file, convert_to_word(evdp()));
  write_words(&file, convert_to_word(mag()));
  write_words(&file, convert_to_word(user0()));
  write_words(&file, convert_to_word(user1()));
  write_words(&file, convert_to_word(user2()));
  write_words(&file, convert_to_word(user3()));
  write_words(&file, convert_to_word(user4()));
  write_words(&file, convert_to_word(user5()));
  write_words(&file, convert_to_word(user6()));
  write_words(&file, convert_to_word(user7()));
  write_words(&file, convert_to_word(user8()));
  write_words(&file, convert_to_word(user9()));
  write_words(&file, convert_to_word(dist()));
  write_words(&file, convert_to_word(az()));
  write_words(&file, convert_to_word(baz()));
  write_words(&file, convert_to_word(gcarc()));
  write_words(&file, convert_to_word(static_cast<float>(sb())));
  write_words(&file, convert_to_word(static_cast<float>(sdelta())));
  write_words(&file, convert_to_word(depmen()));
  write_words(&file, convert_to_word(cmpaz()));
  write_words(&file, convert_to_word(cmpinc()));
  write_words(&file, convert_to_word(xminimum()));
  write_words(&file, convert_to_word(xmaximum()));
  write_words(&file, convert_to_word(yminimum()));
  write_words(&file, convert_to_word(ymaximum()));
  // Fill 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    write_words(&file, convert_to_word(az()));
  }
  write_words(&file, convert_to_word(nzyear()));
  write_words(&file, convert_to_word(nzjday()));
  write_words(&file, convert_to_word(nzhour()));
  write_words(&file, convert_to_word(nzmin()));
  write_words(&file, convert_to_word(nzsec()));
  write_words(&file, convert_to_word(nzmsec()));
  write_words(&file, convert_to_word(header_version));
  write_words(&file, convert_to_word(norid()));
  write_words(&file, convert_to_word(nevid()));
  write_words(&file, convert_to_word(npts()));
  write_words(&file, convert_to_word(nsnpts()));
  write_words(&file, convert_to_word(nwfid()));
  write_words(&file, convert_to_word(nxsize()));
  write_words(&file, convert_to_word(nysize()));
  // Fill 'unused'
  write_words(&file, convert_to_word(nysize()));
  write_words(&file, convert_to_word(iftype()));
  write_words(&file, convert_to_word(idep()));
  write_words(&file, convert_to_word(iztype()));
  // Fill 'unused'
  write_words(&file, convert_to_word(iztype()));
  write_words(&file, convert_to_word(iinst()));
  write_words(&file, convert_to_word(istreg()));
  write_words(&file, convert_to_word(ievreg()));
  write_words(&file, convert_to_word(ievtyp()));
  write_words(&file, convert_to_word(iqual()));
  write_words(&file, convert_to_word(isynth()));
  write_words(&file, convert_to_word(imagtyp()));
  write_words(&file, convert_to_word(imagsrc()));
  write_words(&file, convert_to_word(ibody()));
  // Fill 'unused' (xcommon_skip_num)
  for (int i{0}; i < common_skip_num; ++i) {
    write_words(&file, convert_to_word(ibody()));
  }
  write_words(&file, bool_to_word(leven()));
  write_words(&file, bool_to_word(lpspol()));
  write_words(&file, bool_to_word(lovrok()));
  write_words(&file, bool_to_word(lcalda()));
  // Fill 'unused'
  write_words(&file, bool_to_word(lcalda()));
  // Strings are special
  std::array<char, static_cast<size_t>(2) * word_length> two_words{
      convert_to_words<sizeof(two_words)>(kstnm(), 2)};
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  std::array<char, static_cast<size_t>(4) * word_length> four_words{
      convert_to_words<sizeof(four_words)>(kevnm(), 4)};
  write_words(&file, std::vector<char>(four_words.begin(), four_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(khole(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ko(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ka(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt0(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt1(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt2(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt3(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt4(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt5(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt6(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt7(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt8(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt9(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kf(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser0(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser1(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser2(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kcmpnm(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(knetwk(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kdatrd(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kinst(), 2);
  write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));
  // Data
  for (double dub : data1()) [[likely]] {
    write_words(&file, convert_to_word(static_cast<float>(dub)));
  }
  if (!leven() || (iftype() > 1)) {
    for (double dub : data2()) {
      write_words(&file, convert_to_word(static_cast<float>(dub)));
    }
  }
  if (header_version == modern_hdr_version) {
    // Write footer
    write_words(&file, convert_to_word(delta()));
    write_words(&file, convert_to_word(b()));
    write_words(&file, convert_to_word(e()));
    write_words(&file, convert_to_word(o()));
    write_words(&file, convert_to_word(a()));
    write_words(&file, convert_to_word(t0()));
    write_words(&file, convert_to_word(t1()));
    write_words(&file, convert_to_word(t2()));
    write_words(&file, convert_to_word(t3()));
    write_words(&file, convert_to_word(t4()));
    write_words(&file, convert_to_word(t5()));
    write_words(&file, convert_to_word(t6()));
    write_words(&file, convert_to_word(t7()));
    write_words(&file, convert_to_word(t8()));
    write_words(&file, convert_to_word(t9()));
    write_words(&file, convert_to_word(f()));
    write_words(&file, convert_to_word(evlo()));
    write_words(&file, convert_to_word(evla()));
    write_words(&file, convert_to_word(stlo()));
    write_words(&file, convert_to_word(stla()));
    write_words(&file, convert_to_word(sb()));
    write_words(&file, convert_to_word(sdelta()));
  }
  file.close();
}

void Trace::legacy_write(const std::filesystem::path &path) const {
  write(path, true);
}
}; // namespace sacfmt
