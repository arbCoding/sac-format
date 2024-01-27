// Copyright 2023-2024 Alexander R. Blanchette

/*!
  \file sac_format.hpp

  \brief Interface of the sac-format library.

  \author Alexander R. Blanchette

  This file is the interface for sac-format library. Everything in this file
  is targeted for testing coverage.
  */

#ifndef SAC_FORMAT_HPP_20231115_0945
#define SAC_FORMAT_HPP_20231115_0945
#pragma once
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// std::ranges::fill
#include <algorithm>
// std::array
#include <array>
// std::endian
#include <bit>
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
// std::numbers::pi_v<double>
#include <numbers>
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

// Apparently MSVC doesn't have uint defined.
#if defined(_MSC_VER) || defined(__MINW32__)
typedef unsigned int uint;
#endif

//! sac-format namespace
namespace sacfmt {
//--------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------
//! Size (bytes) of fundamental data-chunk.
constexpr size_t word_length{4};
//! Size (bits) of binary character.
constexpr size_t bits_per_byte{8};
//! Size (bits) of funamental data-chunk.
constexpr size_t binary_word_size{word_length * bits_per_byte};
//! First word of (first) data-section (stream offset).
constexpr std::streamoff data_word{158};
//! Integer unset value (SAC Magic).
constexpr int unset_int{-12345};
//! Float-point unset value (SAC Magic).
constexpr float unset_float{-12345.0F};
//! Double-precision unset value (SAC Magic).
constexpr double unset_double{-12345.0};
//! Boolean unset value (SAC Magic).
constexpr bool unset_bool{false};
//! String unset value (SAC Magic).
const std::string unset_word{"-12345"};
//! Accuracy precision expected of SAC floating-point values.
constexpr float f_eps{2.75e-6F};
//! One binary character (useful for building strings).
using char_bit = std::bitset<bits_per_byte>;
//! One binary word (useful for non-strings).
using word_one = std::bitset<binary_word_size>;
//! Two binary words (useful for strings).
using word_two = std::bitset<static_cast<size_t>(2) * binary_word_size>;
//! Four binary words (kEvNm only).
using word_four = std::bitset<static_cast<size_t>(4) * binary_word_size>;
//! ASCII-code of 'space' character.
constexpr int ascii_space{32};
//! Number of float-poing header values in SAC format.
constexpr int num_float{39};
//! Number of double-precision header values in SAC format.
constexpr int num_double{22};
//! Number of integer header values in SAC format.
constexpr int num_int{26};
//! Number of boolean header values in SAC format.
constexpr int num_bool{4};
//! Number of string header values in SAC format.
constexpr int num_string{23};
//! Number of data arrays in SAC format.
constexpr int num_data{2};
//! Number of double-precision footer values in SAC format (version 7).
constexpr int num_footer{22};
//! nVHdr value for newest SAC format (2020+).
constexpr int modern_hdr_version{7};
//! nVHdr value for historic SAC format (pre-2020).
constexpr int old_hdr_version{6};
//! Extremely common number of 'internal use' headers in SAC format.
constexpr int common_skip_num{7};
//! Radians per degree.
constexpr double rad_per_deg{std::numbers::pi_v<double> / 180.0};
//! Degrees per radian.
constexpr double deg_per_rad{1.0 / rad_per_deg};
//! Degrees in a circle.
constexpr double circle_deg{360.0};
//! Average radius of Earth (kilometers).
constexpr double earth_radius{6378.14};
//--------------------------------------------------------------------------
// Conversions
//--------------------------------------------------------------------------
// Calculate position of word in SAC-file.
std::streamoff word_position(size_t word_number) noexcept;
// Convert unsigned integer to 32-bit (one word) binary bitset.
word_one uint_to_binary(uint num) noexcept;
// Convert integer to 32-bit (one word) binary bitset.
word_one int_to_binary(int num) noexcept;
// Convert 32-bit (one word) binary bitset to integer.
int binary_to_int(word_one bin) noexcept;
//! bitset type-safety namespace.
namespace bitset_type {
//! Ensure type-safety for conversions between floats/doubles and bitsets.
template <unsigned nbits> struct uint {};
//! Single-word type-safety (non-strings).
template <> struct uint<bits_per_byte> {
  using type = uint8_t;
};
//! Two-word type-safety (strings).
template <> struct uint<2 * bits_per_byte> {
  using type = uint16_t;
};
//! Four-word type-safety (kEvNm).
template <> struct uint<4 * bits_per_byte> {
  using type = uint32_t;
};
//! ? type-safety?
constexpr int bytes{8};
template <> struct uint<bytes * bits_per_byte> {
  using type = uint64_t;
};
}  // namespace bitset_type
//! Convert variable to unsigned-integer using type-safe conversions.
template <class T>
using unsigned_int =
    typename bitset_type::uint<sizeof(T) * bits_per_byte>::type;
// Convert floating-point value to 32-bit (one word) binary bitset.
word_one float_to_binary(float num) noexcept;
// Convert 32-bit (one word) binary bitset to floating-point value.
float binary_to_float(const word_one &bin) noexcept;
// Convert double-precision value to 64-bit (two words) binary bitset.
word_two double_to_binary(double num) noexcept;
// Convert 64-bit (two words) binary bitset to double-precision value.
double binary_to_double(const word_two &bin) noexcept;
// Remove leading spaces from string.
void remove_leading_spaces(std::string *str) noexcept;
// Remove trailing spaces from string.
void remove_trailing_spaces(std::string *str) noexcept;
// Remove leading/trailing white-space and control characters from string.
std::string string_cleaning(const std::string &str) noexcept;
// Clean + truncate/pad string as necessary.
void prep_string(std::string *str, size_t str_size) noexcept;
// Template function to convert string into binary bitset.
template <typename T>
void string_bits(T *bits, const std::string &str, size_t str_size) noexcept;
// Template function to convert binary bitset to string.
template <typename T>
std::string bits_string(const T &bits, size_t num_words) noexcept;
// Convert string to a 64-bit (two word) binary bitset.
word_two string_to_binary(std::string str) noexcept;
// Convert a 64-bit (two word) binary bitset to a string.
std::string binary_to_string(const word_two &str) noexcept;
// Convert a string to a 128-bit (four word) binary bitset.
word_four long_string_to_binary(std::string str) noexcept;
// Convert a 128-bit (four word) binary bitset to a string.
std::string binary_to_long_string(const word_four &str) noexcept;
// Convert a boolean to a 32-bit (one word) binary bitset.
word_one bool_to_binary(bool flag) noexcept;
// Convert a 32-bit (one word) binary bitset to a boolean.
bool binary_to_bool(const word_one &flag) noexcept;
/*! \brief Struct containing a pair of words.

  Prevents bug-prone word-swapping in functions that use a pair of words.

  These are not necessarily single words, it could be a pair of ::word_one or
  a pair of ::word_two.

  \struct word_pair
  */
template <typename T> struct word_pair {
  T first{};   //!< First 'word' in the pair.
  T second{};  //!< Second 'word' in the pair.
};
// Concatenate two one-word strings into a two-word string.
word_two concat_words(const word_pair<word_one> &pair_words) noexcept;
// Concatenate two two-word strings into a four-word string.
word_four concat_words(const word_pair<word_two> &pair_words) noexcept;
//--------------------------------------------------------------------------
// Reading
//--------------------------------------------------------------------------
/*! \brief Struct that specifies parameters for reading.

  Prevents bug-prone number-swapping in functions that use a reading
  specification.

  \struct read_spec
 */
struct read_spec {
  // cppcheck-suppress unusedStructMember
  size_t num_words{};  //!< Number of words to read.
  // cppcheck-suppress unusedStructMember
  size_t start_word{};  //!< Word to start reading from.
};
// Checks safety of reading N-more words from current position in file.
bool nwords_after_current(std::ifstream *sac, const read_spec &spec) noexcept;
// Is the SAC-file large enough to fit a proper header?
void safe_to_read_header(std::ifstream *sac);
// Is the SAC-file large enough to fit a proper footer (if appropriate)?
void safe_to_read_footer(std::ifstream *sac);
// Is the SAC-file large enough to fit a proper data vector?
void safe_to_read_data(std::ifstream *sac, size_t n_words, bool data2 = false);
// Have we reached the end of the SAC-file or are there shenanigans?
void safe_to_finish_reading(std::ifstream *sac);
// Read one word (32 bits, useful for non-strings) from a binary SAC-file.
word_one read_word(std::ifstream *sac);
// Read two words (64 bits, useful for strings) from a binary SAC-file.
word_two read_two_words(std::ifstream *sac);
// Read four words (128 bits, kEvNm only) from a binary SAC-file.
word_four read_four_words(std::ifstream *sac);
// Read arbitrary number of words (vectors) from a binary SAC-file.
std::vector<double> read_data(std::ifstream *sac, const read_spec &spec);
//--------------------------------------------------------------------------
// Writing
//--------------------------------------------------------------------------
// Write arbitrary number of words (vectors) to a binary SAC-file.
void write_words(std::ofstream *sac_file, const std::vector<char> &input);
// Template function to convert input value into a std::vector<char> for
// writing.
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
/*! \enum name
  \brief Enumeration of all SAC fields.

  Additional information can be found at @ref SacFileFormat
  */
enum class name {
  // Floats
  /*!
    Float

    Minimum value of the dependent variable
    (displacement/velocity/acceleration/volts/counts).
   */
  depmin,
  /*!
    Float

    Maximum value of the dependent variable.
   */
  depmax,
  /*!
    Float

    Modified (observational) value of delta.
   */
  odelta,
  /*!
    Float

    Instrument response parameter (poles, zeros, and a constant).

    Not used by SAC - free for other purposes.
   */
  resp0,
  //! See resp0.
  resp1,
  //! See resp0.
  resp2,
  //! See resp0.
  resp3,
  //! See resp0.
  resp4,
  //! See resp0.
  resp5,
  //! See resp0.
  resp6,
  //! See resp0.
  resp7,
  //! See resp0.
  resp8,
  //! See resp0.
  resp9,
  /*!
    Float

    Station elevation in meters above sea level (m.a.s.l.).

    Not used by SAC - free for other purposes.
   */
  stel,
  /*!
    Float

    Station depth in meters below surface (borehole/buried vault).

    Not used by SAC - free for other purposes.
   */
  stdp,
  /*!
    Float

    Event elevation m.a.s.l.

    Not used by SAC - free for other purposes.
   */
  evel,
  /*!
    Float

    Event depth in kilometers (previous meters) below surface.
   */
  evdp,
  /*!
    Float

    Event magnitude.
   */
  mag,
  /*!
    Float

    Storage for user-defined values.
   */
  user0,
  //! See user0
  user1,
  //! See user0
  user2,
  //! See user0
  user3,
  //! See user0
  user4,
  //! See user0
  user5,
  //! See user0
  user6,
  //! See user0
  user7,
  //! See user0
  user8,
  //! See user0
  user9,
  /*!
    Float

    Station-Event distance in kilometers.
   */
  dist,
  /*!
    Float

    Azimuth \f$\color{orange}Station\rightarrow Event\f$ in decimal degrees
    from North.
   */
  az,
  /*!
    Float

    Back-Azimuth \f$\color{orange}Event\rightarrow Station\f$ in decimal degrees
    from North.
   */
  baz,
  /*!
    Float

    Great-circle arc-distance between station and event in decimal degrees.
   */
  gcarc,
  /*!
    Float

    Mean value of dependent variable.
   */
  depmen,
  /*!
    Float

    Instrument measurement azimuth, decimal degrees from North.
   */
  cmpaz,
  /*!
    Float

    Instrument measurement incidence angle, decimal degrees from upward
    vertical (incident 0 = dip -90).

    Note: SEED/MINISEED use dip angle, decimal degrees from horizontal
    (dip 0 = incident 90).
   */
  cmpinc,
  /*!
    Float

    Spectral-only equivalent of depmin (\f$\color{orange}f_{0}\f$ or
    \f$\color{orange}\omega_{0}\f$).
   */
  xminimum,
  /*!
    Float

    Spectral-only equivalent of depman (\f$\color{orange}f_{max}\f$ or
    \f$\color{orange}\omega_{max}\f$).
   */
  xmaximum,
  /*!
    Float

    Spectral-only equivalent of b.
   */
  yminimum,
  /*!
    Float

    Spectral-only equivalent of e.
   */
  ymaximum,
  // Doubles
  /*!
    Double

    Increment between evenly-spaced samples (
    \f$\color{orange}\Delta t\f$ for timeseries,
    \f$\color{orange}\Delta f\f$ or
    \f$\color{orange}\Delta\omega\f$ for spectral).
   */
  delta,
  /*!
    Double

    First value (beginning) of independent variable (\f$\color{orange}t_{0}\f$).
   */
  b,
  /*!
    Double

    Final value (ending) of the independent variable
    (\f$\color{orange}t_{max}\f$).
   */
  e,
  /*!
    Double

    Event origin time, in seconds relative to the reference time.
   */
  o,
  /*!
    Double

    Event first arrival time, in seconds relative to the reference time.
   */
  a,
  /*!
    Double

    User defined time value, in seconds relative to the reference time.
   */
  t0,
  //! See t0
  t1,
  //! See t0
  t2,
  //! See t0
  t3,
  //! See t0
  t4,
  //! See t0
  t5,
  //! See t0
  t6,
  //! See t0
  t7,
  //! See t0
  t8,
  //! See t0
  t9,
  /*!
    Double

    Event end (fini) time, in seconds relative to the reference time.
   */
  f,
  /*!
    Double

    Station latitude in decimal degrees, N/S is positive/negative.

    sac-format automatically enforces \f$\color{orange}\phi\in [-90,90]\f$.
   */
  stla,
  /*!
    Double

    Station longitude in decimal degrees, E/W is positive/negative.

    sac-format automaticall enforces \f$\color{orange}\lambda\in [-180,180]\f$.
   */
  stlo,
  /*!
    Double

    Event latitude in decimal degrees, N/S is positive/negative.

    sac-format automatically enforces \f$\color{orange}\phi\in [-90,90]\f$.
   */
  evla,
  /*!
    Double

    Event longitude in decimal degrees, E/W is positive/negative.

    sac-format automatically enforces \f$\color{orange}\lambda\in [-180,180]\f$.
   */
  evlo,
  /*!
    Double

    Original (saved) value of b (beginning).
   */
  sb,
  /*!
    Double

    Original (saved) value of delta (sample-spacing).
   */
  sdelta,
  // Ints
  /*!
    Integer

    Reference time GMT year.
   */
  nzyear,
  /*!
    Integer

    Reference time GMT day-of-year (often called Julian Date).

    1-366 Not enforced.
   */
  nzjday,
  /*!
    Integer

    Reference time GMT hour.

    00-23 Not enforced.
   */
  nzhour,
  /*!
    Integer

    Reference time GMT minute.

    00-59 Not enforced.
   */
  nzmin,
  /*!
    Integer

    Reference time GMT second.

    00-59 Not enforced.
   */
  nzsec,
  /*!
    Integer

    Reference time GMT millisecond.

    0-999 not enforced.
   */
  nzmsec,
  /*!
    Integer

    SAC-file version.

    7 = 2020+, sac 102.0+, has a Footer.
    6 = pre-2020, sac 101.6a-, no Footer.
   */
  nvhdr,
  /*!
    Integer

    Origin ID.
   */
  norid,
  /*!
    Integer

    Event ID.
   */
  nevid,
  /*!
    Integer

    Number of points in data.
   */
  npts,
  /*!
    Integer

    Original (saved) npts.
   */
  nsnpts,
  /*!
    Integer

    Waveform ID.
   */
  nwfid,
  /*!
    Integer

    Spectral-only equivalent of npts (length of spectrum).
   */
  nxsize,
  /*!
    Integer

    Spectral-only; width of spectrum.
   */
  nysize,
  /*!
    Integer

    File type.
   */
  iftype,
  /*!
    Integer

    Dependent variable type.
   */
  idep,
  /*!
    Integer

    Reference time equivalent.
   */
  iztype,
  /*!
    Integer

    Recording instrument type.

    Not used by SAC - free for other purposes.
   */
  iinst,
  /*!
    Integer

    Station geographic region.

    Not used by SAC - free for other purposes.
   */
  istreg,
  /*!
    Integer

    Event geographic region.

    Not used by SAC - free for other purposes.
   */
  ievreg,
  /*!
    Integer

    Event type.

    Not used by SAC - free for other purposes.
   */
  ievtyp,
  /*!
    Integer

    Quality of data.

    Not used by SAC - free for other purposes.
   */
  iqual,
  /*!
    Integer

    Synthetic data flag.

    Not used by SAC - free for other purposes.
   */
  isynth,
  /*!
    Integer

    Magnitude type.
   */
  imagtyp,
  /*!
    Integer

    Magnitude information source.
   */
  imagsrc,
  /*!
    Integer

    Body/spheroid definition used to calculate distances.

    Not currently-used by sac-format (SAC does used it).
   */
  ibody,
  // Bools
  /*!
    Boolean

    REQUIRED

    Evenly-spaced data flag. True = even.
   */
  leven,
  /*!
    Boolean

    Station polarity flag.

    True = positive (left-handed, e.g. North-East-Up).
   */
  lpspol,
  /*!
    Boolean

    File overwrite flag.

    If true, okay to overwrite file.

    Not used by sac-format.
   */
  lovrok,
  /*!
    Boolean

    Calculate geometry flag.

    Not used by sac-format.
   */
  lcalda,
  // Strings
  /*!
    String (2 words)

    Station name.
   */
  kstnm,
  /*!
    String (4 words)

    Event name.
   */
  kevnm,
  /*!
    String (2 words)

    Nuclear-Hole identifier.

    Other-Location identifier (LOCID).
   */
  khole,
  /*!
    String (2 words)

    Text for o.
   */
  ko,
  /*!
    String (2 words)

    Text for a.
   */
  ka,
  /*!
    String (2 words)

    Text for t0
   */
  kt0,
  //! See kt0
  kt1,
  //! See kt0
  kt2,
  //! See kt0
  kt3,
  //! See kt0
  kt4,
  //! See kt0
  kt5,
  //! See kt0
  kt6,
  //! See kt0
  kt7,
  //! See kt0
  kt8,
  //! See kt0
  kt9,
  /*!
    String (2 words)

    Text for f.
   */
  kf,
  /*!
    String (2 words)

    Text for user0.
   */
  kuser0,
  //! See kuser0.
  kuser1,
  //! See kuser0.
  kuser2,
  /*!
    String (2 words)

    Component name.
   */
  kcmpnm,  // missing in org documentation
  /*!
    String (2 words)

    Network name.
   */
  knetwk,  // missing in org documentation
  /*!
    String (2 words)

    Date the data was read onto a computer.
   */
  kdatrd,
  /*!
    String (2 words)

    Instrument name.
   */
  kinst,
  // Data
  /*!
    std::vector<double>

    First data vector. ALWAYS present, ALWAYS begins at word 158.
   */
  data1,
  /*!
    std::vector<double>

    Second data vector. CONDITIONAL present. IF PRESENT, begins at end of data1.

    Required if leven is false (uneven sampling), or if iftype is
    spectral/XY/XYZ.
   */
  data2
};
// Lookup maps
/*!
\brief Lookup table for variable locations.

Maps SAC variables (headers and data) to their internal locations in the
Trace class.
*/
const std::unordered_map<name, const size_t> sac_map = {
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
// Trace class
/*! \class Trace
 \brief The Trace class.

 This class is the recommended way for reading/writing SAC-files.

 It safely reads all data, provides automatic write support based upon
 the nVHdr header value (determine if a footer should be included or not).

 It provides getters and setters for all SAC headers and the data.
 */
class Trace {
public:
  // Default constructor
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
  std::array<float, num_float> floats{};  //!< Float storage array.
  // cppcheck-suppress unusedStructMember
  std::array<double, num_double> doubles{};  //!< Double storage array.
  // cppcheck-suppress unusedStructMember
  std::array<int, num_int> ints{};  //!< Integer storage array.
  // cppcheck-suppress unusedStructMember
  std::array<bool, num_bool> bools{};  //!< Boolean storage array.
  // cppcheck-suppress unusedStructMember
  std::array<std::string, num_string> strings{};  //!< String storage array.
  std::array<std::vector<double>, num_data>
      // cppcheck-suppress unusedStructMember
      data{};  //!< std::vector<double> storage array.
};

/*! \class io_error
  \brief Class for generic I/O exceptions.

  These errors occur due to bad path, bad permissions, or otherwise corrupt
  SAC-files.

  I/O operations may raise other exceptions (disk failure, out of space, etc.),
  but those are difficult to emulate for testing purposes (therefore I am unable
  to reliably cover them); they also arise due to conditions that would render
  how sac-format handles them moot.
 */
class io_error : public std::exception {
private:
  const std::string message{};  //!< Error message

public:
  /*!
    \brief io_error Constructor

    @param[in] msg std::string Error message.
   */
  explicit io_error(std::string msg) : message(std::move(msg)) {}
  /*!
    \brief Error message delivery.

    @returns what char* Error message.
    */
  [[nodiscard]] const char *what() const noexcept override {
    return message.c_str();
  }
};
};  // namespace sacfmt
#endif
