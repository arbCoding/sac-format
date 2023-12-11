// Copyright 2023 Alexander R. Blanchette

#ifndef SAC_FORMAT_HPP_20231115_0945
#define SAC_FORMAT_HPP_20231115_0945
#pragma once
/* Standard library
   https://en.cppreference.com/w/cpp/standard_library */
/* v cpplint thinks these are C-libraries v */
// std::endian
#include <bit>
// std::numbers::pi_v<double>
#include <numbers>
/* ^ cpplint thinks these are C-libraries ^ */
// std::ranges::fill
#include <algorithm>
// std::array
#include <array>
// std::bitset
#include <bitset>
// std::abs, std::acos, std::sin, std::cos, std::atan2, std::pow
#include <cmath>
// std::memcopy
#include <cstring>
// std::filesystem::path
#include <filesystem>
// std::ifstream, std::ifstream::binary, std::ofstream, std::ios::binary,
// std::ios::out, std::ios::trunc
#include <fstream>
// std::exception
#include <stdexcept>
// ostringstream
#include <sstream>
// std::string
#include <string>
// std::unordered_map
#include <unordered_map>
// std::move
#include <utility>
// std::vector
#include <vector>

namespace sacfmt {
//--------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------
// Size of data chunks in binary SAC files (called words)
constexpr int word_length{4};    // bytes
constexpr int bits_per_byte{8};  // binary character size
// Each word is 32-bits (4 bytes)
constexpr int binary_word_size{word_length * bits_per_byte};
// First word of (first) data-section
constexpr std::streamoff data_word{158};
constexpr int unset_int{-12345};
constexpr float unset_float{-12345.0F};
constexpr double unset_double{-12345.0};
constexpr bool unset_bool{false};
// Accuracy precision expected from SAC floats
constexpr float f_eps{2.75e-6F};
// This should work for two and four word string headers
// Used to be a string_view, but they're awkward to
// use with strings...
const std::string unset_word{"-12345"};
using word_one = std::bitset<binary_word_size>;
using word_two = std::bitset<static_cast<size_t>(2) * binary_word_size>;
using word_four = std::bitset<static_cast<size_t>(4) * binary_word_size>;
// This character and below show up as whitespace
constexpr int ascii_space{32};
// Number of variables
constexpr int num_float{39};
constexpr int num_double{22};
constexpr int num_int{26};
constexpr int num_bool{4};
constexpr int num_string{23};
constexpr int num_data{2};
constexpr int num_footer{22};
constexpr int modern_hdr_version{7};
constexpr int old_hdr_version{6};
constexpr int common_skip_num{7};
constexpr double rad_per_deg{std::numbers::pi_v<double> / 180.0};
constexpr double deg_per_rad{1.0 / rad_per_deg};
constexpr double circle_deg{360.0};
constexpr double earth_radius{6378.14};  // km
//--------------------------------------------------------------------------
// Conversions
//--------------------------------------------------------------------------
// Calculate position of word in SAC-file
std::streamoff word_position(size_t word_number) noexcept;
// SAC uses 32 bit ints
word_one int_to_binary(int num) noexcept;
int binary_to_int(word_one bin) noexcept;
// Ensure type-safety for conversions between floats/doubles
// and bitsets
namespace bitset_type {
template <unsigned nbits> struct uint {};
template <> struct uint<bits_per_byte> {
  using type = uint8_t;
};
template <> struct uint<2 * bits_per_byte> {
  using type = uint16_t;
};
template <> struct uint<4 * bits_per_byte> {
  using type = uint32_t;
};
constexpr int bytes{8};
template <> struct uint<bytes * bits_per_byte> {
  using type = uint64_t;
};
}  // namespace bitset_type
template <class T>
using unsigned_int =
    typename bitset_type::uint<sizeof(T) * bits_per_byte>::type;
// Convert floats/doubles to bitsets and back
// SAC uses 32 bit floats
word_one float_to_binary(float num) noexcept;
float binary_to_float(const word_one &bin) noexcept;
// SAC uses 64 bit doubles (2 words, 8 bytes)
word_two double_to_binary(double num) noexcept;
double binary_to_double(const word_two &bin) noexcept;
void remove_leading_spaces(std::string *str) noexcept;
void remove_trailing_spaces(std::string *str) noexcept;
// Remove leading/trailing white-space and control characters
std::string string_cleaning(const std::string &str) noexcept;
//
void prep_string(std::string *str, size_t str_size) noexcept;
//
template <typename T>
void string_bits(T *bits, const std::string &str, size_t str_size) noexcept;
//
template <typename T>
std::string bits_string(const T &bits, size_t num_words) noexcept;
// Note the string conversion functions handle over-sized strings
// by truncating them, and undersized strings by padding them with spaces
// SAC uses either 64 bit strings (2 words, 8 bytes, 8 characters)
word_two string_to_binary(std::string str) noexcept;
std::string binary_to_string(const word_two &str) noexcept;
// 128 bit string (4 words, 16 bytes, only KEVNM header, 16 characters)
word_four long_string_to_binary(std::string str) noexcept;
std::string binary_to_long_string(const word_four &str) noexcept;
// Booleans
word_one bool_to_binary(bool flag) noexcept;
bool binary_to_bool(const word_one &flag) noexcept;
// Concat words
// Struct allay bugprone similar types warning on concat_words
template <typename T> struct word_pair {
  T first{};
  T second{};
};
// For some reason, template functions didn't want to work for these...
word_two concat_words(const word_pair<word_one> &pair_words) noexcept;
word_four concat_words(const word_pair<word_two> &pair_words) noexcept;
//--------------------------------------------------------------------------
// Reading
//--------------------------------------------------------------------------
// Struct to allay clang-tidy bugprone swappable parameters warning
struct read_spec {
  // cppcheck-suppress unusedStructMember
  size_t num_words{};
  // cppcheck-suppress unusedStructMember
  size_t start_word{};
};
// The below functions make reading SAFE and prevent exceptions when
// trying to read a file without a massive performance hit, unless
// you try to use it for every single word (see how it is chunk-checked
// in Trace::Trace to see an efficient/intuitive scheme)
// Does the filesize remaining fit n_words?
bool nwords_after_current(std::ifstream *sac, const read_spec &spec) noexcept;
// Does the filesize fit the header?
void safe_to_read_header(std::ifstream *sac);
// Does the remaining filesize fit the footer?
void safe_to_read_footer(std::ifstream *sac);
// Does the remaining filesize fit the data?
void safe_to_read_data(std::ifstream *sac, size_t n_words, bool data2 = false);
// Is the remaining filesize 0? (It should be, otherwise shenanigans).
void safe_to_finish_reading(std::ifstream *sac);
// Can read 1, 2, or 4 words and return as a binary bitset
// Conversion functions are then used to do the conversions
word_one read_word(std::ifstream *sac);
word_two read_two_words(std::ifstream *sac);
word_four read_four_words(std::ifstream *sac);
// Can read any number of words into a vector of doubles
// Useful for data values
std::vector<double> read_data(std::ifstream *sac, const read_spec &spec);
//--------------------------------------------------------------------------
// Writing
//--------------------------------------------------------------------------
// The below writing functions all work, though they are a bit funky
// Using std::vector because more flexible
// Allows writing arbitrary amount of data to file
void write_words(std::ofstream *sac_file, const std::vector<char> &input);
// Template function to convert to a SAC word
// handles float and int (not string or double)
// Only single word
template <typename T> std::vector<char> convert_to_word(T input) noexcept;
// Special for double-precision numbers (2 words, not 1)
std::vector<char> convert_to_word(double input) noexcept;
// Template function to convert string to SAC word(s)
template <size_t N>
std::array<char, N> convert_to_words(const std::string &str,
                                     int n_words) noexcept;
// Convert a bool value to a word
std::vector<char> bool_to_word(bool flag) noexcept;
bool equal_within_tolerance(const std::vector<double> &vector1,
                            const std::vector<double> &vector2,
                            double tolerance = f_eps) noexcept;
bool equal_within_tolerance(double val1, double val2,
                            double tolerance = f_eps) noexcept;
//--------------------------------------------------------------------------
// Geometric Methods
//--------------------------------------------------------------------------
double degrees_to_radians(double degrees) noexcept;
double radians_to_degrees(double radians) noexcept;
// gcarc
double gcarc(double latitude1, double longitude1, double latitude2,
             double longitude2) noexcept;
// azimuth
double azimuth(double latitude1, double longitude1, double latitude2,
               double longitude2) noexcept;
// Bounds
// [-inf, inf] -> [0, 360]
double limit_360(double degrees) noexcept;
// [-inf, inf] -> [0, 360] -> (-180, 180]
double limit_180(double degrees) noexcept;
// [-inf, inf] -> [0, 360] -> (-180, 180] -> [-90, 90]
double limit_90(double degrees) noexcept;
//--------------------------------------------------------------------------
// Trace Class
//--------------------------------------------------------------------------
enum class name {
  // Floats
  depmin,
  depmax,
  odelta,
  resp0,
  resp1,
  resp2,
  resp3,
  resp4,
  resp5,
  resp6,
  resp7,
  resp8,
  resp9,
  stel,
  stdp,
  evel,
  evdp,
  mag,
  user0,
  user1,
  user2,
  user3,
  user4,
  user5,
  user6,
  user7,
  user8,
  user9,
  dist,
  az,
  baz,
  gcarc,
  depmen,
  cmpaz,
  cmpinc,
  xminimum,
  xmaximum,
  yminimum,
  ymaximum,
  // Doubles
  delta,
  b,
  e,
  o,
  a,
  t0,
  t1,
  t2,
  t3,
  t4,
  t5,
  t6,
  t7,
  t8,
  t9,
  f,
  stla,
  stlo,
  evla,
  evlo,
  sb,
  sdelta,
  // Ints
  nzyear,
  nzjday,
  nzhour,
  nzmin,
  nzsec,
  nzmsec,
  nvhdr,
  norid,
  nevid,
  npts,
  nsnpts,
  nwfid,
  nxsize,
  nysize,
  iftype,
  idep,
  iztype,
  iinst,
  istreg,
  ievreg,
  ievtyp,
  iqual,
  isynth,
  imagtyp,
  imagsrc,
  ibody,
  // Bools
  leven,
  lpspol,
  lovrok,
  lcalda,
  // Strings
  kstnm,
  kevnm,
  khole,
  ko,
  ka,
  kt0,
  kt1,
  kt2,
  kt3,
  kt4,
  kt5,
  kt6,
  kt7,
  kt8,
  kt9,
  kf,
  kuser0,
  kuser1,
  kuser2,
  kcmpnm,
  knetwk,
  kdatrd,
  kinst,
  // Data
  data1,
  data2
};
// Lookup maps
const std::unordered_map<name, const int> sac_map = {
    // Floats
    {name::depmin, 0},
    {name::depmax, 1},
    {name::odelta, 2},
    {name::resp0, 3},
    {name::resp1, 4},
    {name::resp2, 5},
    {name::resp3, 6},
    {name::resp4, 7},
    {name::resp5, 8},
    {name::resp6, 9},
    {name::resp7, 10},
    {name::resp8, 11},
    {name::resp9, 12},
    {name::stel, 13},
    {name::stdp, 14},
    {name::evel, 15},
    {name::evdp, 16},
    {name::mag, 17},
    {name::user0, 18},
    {name::user1, 19},
    {name::user2, 20},
    {name::user3, 21},
    {name::user4, 22},
    {name::user5, 23},
    {name::user6, 24},
    {name::user7, 25},
    {name::user8, 26},
    {name::user9, 27},
    {name::dist, 28},
    {name::az, 29},
    {name::baz, 30},
    {name::gcarc, 31},
    {name::depmen, 32},
    {name::cmpaz, 33},
    {name::cmpinc, 34},
    {name::xminimum, 35},
    {name::xmaximum, 36},
    {name::yminimum, 37},
    {name::ymaximum, 38},
    // Doubles
    {name::delta, 0},
    {name::b, 1},
    {name::e, 2},
    {name::o, 3},
    {name::a, 4},
    {name::t0, 5},
    {name::t1, 6},
    {name::t2, 7},
    {name::t3, 8},
    {name::t4, 9},
    {name::t5, 10},
    {name::t6, 11},
    {name::t7, 12},
    {name::t8, 13},
    {name::t9, 14},
    {name::f, 15},
    {name::stla, 16},
    {name::stlo, 17},
    {name::evla, 18},
    {name::evlo, 19},
    {name::sb, 20},
    {name::sdelta, 21},
    // Ints
    {name::nzyear, 0},
    {name::nzjday, 1},
    {name::nzhour, 2},
    {name::nzmin, 3},
    {name::nzsec, 4},
    {name::nzmsec, 5},
    {name::nvhdr, 6},
    {name::norid, 7},
    {name::nevid, 8},
    {name::npts, 9},
    {name::nsnpts, 10},
    {name::nwfid, 11},
    {name::nxsize, 12},
    {name::nysize, 13},
    {name::iftype, 14},
    {name::idep, 15},
    {name::iztype, 16},
    {name::iinst, 17},
    {name::istreg, 18},
    {name::ievreg, 19},
    {name::ievtyp, 20},
    {name::iqual, 21},
    {name::isynth, 22},
    {name::imagtyp, 23},
    {name::imagsrc, 24},
    {name::ibody, 25},
    // Bools
    {name::leven, 0},
    {name::lpspol, 1},
    {name::lovrok, 2},
    {name::lcalda, 3},
    // Strings
    {name::kstnm, 0},
    {name::kevnm, 1},
    {name::khole, 2},
    {name::ko, 3},
    {name::ka, 4},
    {name::kt0, 5},
    {name::kt1, 6},
    {name::kt2, 7},
    {name::kt3, 8},
    {name::kt4, 9},
    {name::kt5, 10},
    {name::kt6, 11},
    {name::kt7, 12},
    {name::kt8, 13},
    {name::kt9, 14},
    {name::kf, 15},
    {name::kuser0, 16},
    {name::kuser1, 17},
    {name::kuser2, 18},
    {name::kcmpnm, 19},
    {name::knetwk, 20},
    {name::kdatrd, 21},
    {name::kinst, 22},
    // Data
    {name::data1, 0},
    {name::data2, 1}};
class Trace {
public:
  Trace() noexcept;
  // Parametric constructor (read file)
  explicit Trace(const std::filesystem::path &path);
  void write(const std::filesystem::path &path, bool legacy = false) const;
  void legacy_write(const std::filesystem::path &path) const;
  bool operator==(const Trace &other) const noexcept;
  // Convenience functions
  void calc_geometry() noexcept;
  [[nodiscard]] double frequency() const noexcept;
  [[nodiscard]] std::string date() const noexcept;
  [[nodiscard]] std::string time() const noexcept;
  // Getters
  // Floats
  [[nodiscard]] float depmin() const noexcept;
  [[nodiscard]] float depmax() const noexcept;
  [[nodiscard]] float odelta() const noexcept;
  [[nodiscard]] float resp0() const noexcept;
  [[nodiscard]] float resp1() const noexcept;
  [[nodiscard]] float resp2() const noexcept;
  [[nodiscard]] float resp3() const noexcept;
  [[nodiscard]] float resp4() const noexcept;
  [[nodiscard]] float resp5() const noexcept;
  [[nodiscard]] float resp6() const noexcept;
  [[nodiscard]] float resp7() const noexcept;
  [[nodiscard]] float resp8() const noexcept;
  [[nodiscard]] float resp9() const noexcept;
  [[nodiscard]] float stel() const noexcept;
  [[nodiscard]] float stdp() const noexcept;
  [[nodiscard]] float evel() const noexcept;
  [[nodiscard]] float evdp() const noexcept;
  [[nodiscard]] float mag() const noexcept;
  [[nodiscard]] float user0() const noexcept;
  [[nodiscard]] float user1() const noexcept;
  [[nodiscard]] float user2() const noexcept;
  [[nodiscard]] float user3() const noexcept;
  [[nodiscard]] float user4() const noexcept;
  [[nodiscard]] float user5() const noexcept;
  [[nodiscard]] float user6() const noexcept;
  [[nodiscard]] float user7() const noexcept;
  [[nodiscard]] float user8() const noexcept;
  [[nodiscard]] float user9() const noexcept;
  [[nodiscard]] float dist() const noexcept;
  [[nodiscard]] float az() const noexcept;
  [[nodiscard]] float baz() const noexcept;
  [[nodiscard]] float gcarc() const noexcept;
  [[nodiscard]] float depmen() const noexcept;
  [[nodiscard]] float cmpaz() const noexcept;
  [[nodiscard]] float cmpinc() const noexcept;
  [[nodiscard]] float xminimum() const noexcept;
  [[nodiscard]] float xmaximum() const noexcept;
  [[nodiscard]] float yminimum() const noexcept;
  [[nodiscard]] float ymaximum() const noexcept;
  // Doubles
  [[nodiscard]] double delta() const noexcept;
  [[nodiscard]] double b() const noexcept;
  [[nodiscard]] double e() const noexcept;
  [[nodiscard]] double o() const noexcept;
  [[nodiscard]] double a() const noexcept;
  [[nodiscard]] double t0() const noexcept;
  [[nodiscard]] double t1() const noexcept;
  [[nodiscard]] double t2() const noexcept;
  [[nodiscard]] double t3() const noexcept;
  [[nodiscard]] double t4() const noexcept;
  [[nodiscard]] double t5() const noexcept;
  [[nodiscard]] double t6() const noexcept;
  [[nodiscard]] double t7() const noexcept;
  [[nodiscard]] double t8() const noexcept;
  [[nodiscard]] double t9() const noexcept;
  [[nodiscard]] double f() const noexcept;
  [[nodiscard]] double stla() const noexcept;
  [[nodiscard]] double stlo() const noexcept;
  [[nodiscard]] double evla() const noexcept;
  [[nodiscard]] double evlo() const noexcept;
  [[nodiscard]] double sb() const noexcept;
  [[nodiscard]] double sdelta() const noexcept;
  // Ints
  [[nodiscard]] int nzyear() const noexcept;
  [[nodiscard]] int nzjday() const noexcept;
  [[nodiscard]] int nzhour() const noexcept;
  [[nodiscard]] int nzmin() const noexcept;
  [[nodiscard]] int nzsec() const noexcept;
  [[nodiscard]] int nzmsec() const noexcept;
  [[nodiscard]] int nvhdr() const noexcept;
  [[nodiscard]] int norid() const noexcept;
  [[nodiscard]] int nevid() const noexcept;
  [[nodiscard]] int npts() const noexcept;
  [[nodiscard]] int nsnpts() const noexcept;
  [[nodiscard]] int nwfid() const noexcept;
  [[nodiscard]] int nxsize() const noexcept;
  [[nodiscard]] int nysize() const noexcept;
  [[nodiscard]] int iftype() const noexcept;
  [[nodiscard]] int idep() const noexcept;
  [[nodiscard]] int iztype() const noexcept;
  [[nodiscard]] int iinst() const noexcept;
  [[nodiscard]] int istreg() const noexcept;
  [[nodiscard]] int ievreg() const noexcept;
  [[nodiscard]] int ievtyp() const noexcept;
  [[nodiscard]] int iqual() const noexcept;
  [[nodiscard]] int isynth() const noexcept;
  [[nodiscard]] int imagtyp() const noexcept;
  [[nodiscard]] int imagsrc() const noexcept;
  [[nodiscard]] int ibody() const noexcept;
  // Bools
  [[nodiscard]] bool leven() const noexcept;
  [[nodiscard]] bool lpspol() const noexcept;
  [[nodiscard]] bool lovrok() const noexcept;
  [[nodiscard]] bool lcalda() const noexcept;
  // Strings
  [[nodiscard]] std::string kstnm() const noexcept;
  [[nodiscard]] std::string kevnm() const noexcept;
  [[nodiscard]] std::string khole() const noexcept;
  [[nodiscard]] std::string ko() const noexcept;
  [[nodiscard]] std::string ka() const noexcept;
  [[nodiscard]] std::string kt0() const noexcept;
  [[nodiscard]] std::string kt1() const noexcept;
  [[nodiscard]] std::string kt2() const noexcept;
  [[nodiscard]] std::string kt3() const noexcept;
  [[nodiscard]] std::string kt4() const noexcept;
  [[nodiscard]] std::string kt5() const noexcept;
  [[nodiscard]] std::string kt6() const noexcept;
  [[nodiscard]] std::string kt7() const noexcept;
  [[nodiscard]] std::string kt8() const noexcept;
  [[nodiscard]] std::string kt9() const noexcept;
  [[nodiscard]] std::string kf() const noexcept;
  [[nodiscard]] std::string kuser0() const noexcept;
  [[nodiscard]] std::string kuser1() const noexcept;
  [[nodiscard]] std::string kuser2() const noexcept;
  [[nodiscard]] std::string kcmpnm() const noexcept;
  [[nodiscard]] std::string knetwk() const noexcept;
  [[nodiscard]] std::string kdatrd() const noexcept;
  [[nodiscard]] std::string kinst() const noexcept;
  // Data
  [[nodiscard]] std::vector<double> data1() const noexcept;
  [[nodiscard]] std::vector<double> data2() const noexcept;
  // Setters
  // Floats
  void depmin(float input) noexcept;
  void depmax(float input) noexcept;
  void odelta(float input) noexcept;
  void resp0(float input) noexcept;
  void resp1(float input) noexcept;
  void resp2(float input) noexcept;
  void resp3(float input) noexcept;
  void resp4(float input) noexcept;
  void resp5(float input) noexcept;
  void resp6(float input) noexcept;
  void resp7(float input) noexcept;
  void resp8(float input) noexcept;
  void resp9(float input) noexcept;
  void stel(float input) noexcept;
  void stdp(float input) noexcept;
  void evel(float input) noexcept;
  void evdp(float input) noexcept;
  void mag(float input) noexcept;
  void user0(float input) noexcept;
  void user1(float input) noexcept;
  void user2(float input) noexcept;
  void user3(float input) noexcept;
  void user4(float input) noexcept;
  void user5(float input) noexcept;
  void user6(float input) noexcept;
  void user7(float input) noexcept;
  void user8(float input) noexcept;
  void user9(float input) noexcept;
  void dist(float input) noexcept;
  void az(float input) noexcept;
  void baz(float input) noexcept;
  void gcarc(float input) noexcept;
  void depmen(float input) noexcept;
  void cmpaz(float input) noexcept;
  void cmpinc(float input) noexcept;
  void xminimum(float input) noexcept;
  void xmaximum(float input) noexcept;
  void yminimum(float input) noexcept;
  void ymaximum(float input) noexcept;
  // Doubles
  // Doubles
  void delta(double input) noexcept;
  void b(double input) noexcept;
  void e(double input) noexcept;
  void o(double input) noexcept;
  void a(double input) noexcept;
  void t0(double input) noexcept;
  void t1(double input) noexcept;
  void t2(double input) noexcept;
  void t3(double input) noexcept;
  void t4(double input) noexcept;
  void t5(double input) noexcept;
  void t6(double input) noexcept;
  void t7(double input) noexcept;
  void t8(double input) noexcept;
  void t9(double input) noexcept;
  void f(double input) noexcept;
  void stla(double input) noexcept;
  void stlo(double input) noexcept;
  void evla(double input) noexcept;
  void evlo(double input) noexcept;
  void sb(double input) noexcept;
  void sdelta(double input) noexcept;
  // Ints
  void nzyear(int input) noexcept;
  void nzjday(int input) noexcept;
  void nzhour(int input) noexcept;
  void nzmin(int input) noexcept;
  void nzsec(int input) noexcept;
  void nzmsec(int input) noexcept;
  void nvhdr(int input) noexcept;
  void norid(int input) noexcept;
  void nevid(int input) noexcept;
  void npts(int input) noexcept;
  void nsnpts(int input) noexcept;
  void nwfid(int input) noexcept;
  void nxsize(int input) noexcept;
  void nysize(int input) noexcept;
  void iftype(int input) noexcept;
  void idep(int input) noexcept;
  void iztype(int input) noexcept;
  void iinst(int input) noexcept;
  void istreg(int input) noexcept;
  void ievreg(int input) noexcept;
  void ievtyp(int input) noexcept;
  void iqual(int input) noexcept;
  void isynth(int input) noexcept;
  void imagtyp(int input) noexcept;
  void imagsrc(int input) noexcept;
  void ibody(int input) noexcept;
  // Bools
  void leven(bool input) noexcept;
  void lpspol(bool input) noexcept;
  void lovrok(bool input) noexcept;
  void lcalda(bool input) noexcept;
  // Strings
  void kstnm(const std::string &input) noexcept;
  void kevnm(const std::string &input) noexcept;
  void khole(const std::string &input) noexcept;
  void ko(const std::string &input) noexcept;
  void ka(const std::string &input) noexcept;
  void kt0(const std::string &input) noexcept;
  void kt1(const std::string &input) noexcept;
  void kt2(const std::string &input) noexcept;
  void kt3(const std::string &input) noexcept;
  void kt4(const std::string &input) noexcept;
  void kt5(const std::string &input) noexcept;
  void kt6(const std::string &input) noexcept;
  void kt7(const std::string &input) noexcept;
  void kt8(const std::string &input) noexcept;
  void kt9(const std::string &input) noexcept;
  void kf(const std::string &input) noexcept;
  void kuser0(const std::string &input) noexcept;
  void kuser1(const std::string &input) noexcept;
  void kuser2(const std::string &input) noexcept;
  void kcmpnm(const std::string &input) noexcept;
  void knetwk(const std::string &input) noexcept;
  void kdatrd(const std::string &input) noexcept;
  void kinst(const std::string &input) noexcept;
  // Data
  void data1(const std::vector<double> &input) noexcept;
  void data2(const std::vector<double> &input) noexcept;

private:
  // Trying to reduce cognitive complexity down from 28
  // according to clang-tidy (limit 25)
  void read_header(std::ifstream *sac);
  void read_data1() noexcept;
  void read_data2() noexcept;
  void read_footer() noexcept;
  // Convenience methods
  void calc_gcarc() noexcept;
  void calc_dist() noexcept;
  void calc_az() noexcept;
  void calc_baz() noexcept;
  [[nodiscard]] bool geometry_set() const noexcept;
  void resize_data1(size_t size) noexcept;
  void resize_data2(size_t size) noexcept;
  void resize_data(size_t size) noexcept;
  // Objects
  // cppcheck-suppress unusedStructMember
  std::array<float, num_float> floats{};
  // cppcheck-suppress unusedStructMember
  std::array<double, num_double> doubles{};
  // cppcheck-suppress unusedStructMember
  std::array<int, num_int> ints{};
  // cppcheck-suppress unusedStructMember
  std::array<bool, num_bool> bools{};
  // cppcheck-suppress unusedStructMember
  std::array<std::string, num_string> strings{};
  // cppcheck-suppress unusedStructMember
  std::array<std::vector<double>, num_data> data{};
};

class io_error : public std::exception {
private:
  const std::string message{};

public:
  explicit io_error(std::string msg) : message(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override {
    return message.c_str();
  }
};
};  // namespace sacfmt
#endif
