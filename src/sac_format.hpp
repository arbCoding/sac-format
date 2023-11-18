#ifndef SAC_FORMAT_HPP_20231115_0945
#define SAC_FORMAT_HPP_20231115_0945
#pragma once
/* Standard library
  https://en.cppreference.com/w/cpp/standard_library */
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>
/* Boost string algorithms */
// No longer using, keeping here in-case want to re-add in future
//#include <boost/algorithm/string.hpp>

namespace sacfmt {
//--------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------
// Size of data chunks in binary SAC files (called words)
constexpr int word_length{4}; // bytes
constexpr int bits_per_byte{8}; // binary character size
// Each word is 32-bits (4 bytes)
constexpr int binary_word_size{word_length * bits_per_byte};
// First word of (first) data-section
constexpr int data_word{158};
constexpr int unset_int{-12345};
constexpr float unset_float{-12345.0f};
constexpr double unset_double{-12345.0};
constexpr bool unset_bool{false};
// Accuracy precision expected from SAC floats
constexpr float f_eps{2.75e-6};
// This should work for two and four word string headers
const std::string unset_word{"-12345"};
using word_one = std::bitset<binary_word_size>;
using word_two = std::bitset<2 * binary_word_size>;
using word_four = std::bitset<4 * binary_word_size>;
// This character and below show up as whitespace
constexpr int ascii_space{32};
//--------------------------------------------------------------------------
// Conversions
//--------------------------------------------------------------------------
// Calculate position of word in SAC-file
int word_position(const int word_number);
// Floats to binary and back
union float_to_bits {
        float value;
        word_one bits;
        explicit float_to_bits(float x) : value(x){}
        explicit float_to_bits(const word_one& binary) : bits(binary){}
    };
// Doubles to binary and back
union double_to_bits {
        double value;
        word_two bits;
        explicit double_to_bits(double x) : value(x){}
        explicit double_to_bits(const word_two& binary) : bits(binary){}
    };
// SAC uses 32 bit ints
word_one int_to_binary(const int x);
int binary_to_int(word_one x);
// SAC uses 32 bit floats
word_one float_to_binary(const float x);
float binary_to_float(const word_one& x);
// SAC uses 64 bit doubles (2 words, 8 bytes)
word_two double_to_binary(const double x);
double binary_to_double(const word_two& x);
// To get rid of requirement on boost library
void remove_leading_spaces(std::string& str);
void remove_trailing_spaces(std::string& str);
// Remove leading/trailing white-space and control characters
std::string string_cleaning(const std::string& str);
// Note the string conversion functions handle over-sized strings
// by truncating them, and undersized strings by padding them with spaces
// SAC uses either 64 bit strings (2 words, 8 bytes, 8 characters)
word_two string_to_binary(std::string x);
std::string binary_to_string(const word_two& x);
// 128 bit string (4 words, 16 bytes, only KEVNM header, 16 characters)
word_four long_string_to_binary(std::string x);
std::string binary_to_long_string(const word_four& x);
// Booleans
word_one bool_to_binary(const bool x);
bool binary_to_bool(const word_one& x);
// Concat words
// For some reason, template functions didn't want to work for these...
word_two concat_words(const word_one& x, const word_one& y);
word_four concat_words(const word_two& x, const word_two& y);
//--------------------------------------------------------------------------
// Reading
//--------------------------------------------------------------------------
// Can read 1, 2, or 4 words and return as a binary bitset
// Conversion functions are then used to do the conversions
word_one read_word(std::ifstream* sac);
word_two read_two_words(std::ifstream* sac);
word_four read_four_words(std::ifstream* sac);
// Can read any number of words into a vector of doubles
// Useful for data values
std::vector<double> read_data(std::ifstream* sac, const size_t n_words,
                              const int start = data_word);
//--------------------------------------------------------------------------
// Writing
//--------------------------------------------------------------------------
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
template <size_t N>
std::array<char, N> convert_to_words(const std::string& s, int n_words = 1);
// Convert a bool value to a word
std::vector<char> bool_to_word(const bool b);
bool equal_within_tolerance(const std::vector<double>& vector1,
                                const std::vector<double>& vector2,
                                const double tolerance =
                                f_eps);
bool equal_within_tolerance(const double x1, const double x2,
                            const double tolerance = f_eps);
// This is the replacement Trace class
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
const std::unordered_map<name, int> sac_map = {
    // Floats
    {name::depmin,    0},
    {name::depmax,    1},
    {name::odelta,    2},
    {name::resp0,     3},
    {name::resp1,     4},
    {name::resp2,     5},
    {name::resp3,     6},
    {name::resp4,     7},
    {name::resp5,     8},
    {name::resp6,     9},
    {name::resp7,    10},
    {name::resp8,    11},
    {name::resp9,    12},
    {name::stel,     13},
    {name::stdp,     14},
    {name::evel,     15},
    {name::evdp,     16},
    {name::mag,      17},
    {name::user0,    18},
    {name::user1,    19},
    {name::user2,    20},
    {name::user3,    21},
    {name::user4,    22},
    {name::user5,    23},
    {name::user6,    24},
    {name::user7,    25},
    {name::user8,    26},
    {name::user9,    27},
    {name::dist,     28},
    {name::az,       29},
    {name::baz,      30},
    {name::gcarc,    31},
    {name::depmen,   32},
    {name::cmpaz,    33},
    {name::cmpinc,   34},
    {name::xminimum, 35},
    {name::xmaximum, 36},
    {name::yminimum, 37},
    {name::ymaximum, 38},
    // Doubles
    {name::delta,     0},
    {name::b,         1},
    {name::e,         2},
    {name::o,         3},
    {name::a,         4},
    {name::t0,        5},
    {name::t1,        6},
    {name::t2,        7},
    {name::t3,        8},
    {name::t4,        9},
    {name::t5,       10},
    {name::t6,       11},
    {name::t7,       12},
    {name::t8,       13},
    {name::t9,       14},
    {name::f,        15},
    {name::stla,     16},
    {name::stlo,     17},
    {name::evla,     18},
    {name::evlo,     19},
    {name::sb,       20},
    {name::sdelta,   21},
    // Ints
    {name::nzyear,    0},
    {name::nzjday,    1},
    {name::nzhour,    2},
    {name::nzmin,     3},
    {name::nzsec,     4},
    {name::nzmsec,    5},
    {name::nvhdr,     6},
    {name::norid,     7},
    {name::nevid,     8},
    {name::npts,      9},
    {name::nsnpts,   10},
    {name::nwfid,    11},
    {name::nxsize,   12},
    {name::nysize,   13},
    {name::iftype,   14},
    {name::idep,     15},
    {name::iztype,   16},
    {name::iinst,    17},
    {name::istreg,   18},
    {name::ievreg,   19},
    {name::ievtyp,   20},
    {name::iqual,    21},
    {name::isynth,   22},
    {name::imagtyp,  23},
    {name::imagsrc,  24},
    {name::ibody,    25},
    // Bools
    {name::leven,     0},
    {name::lpspol,    1},
    {name::lovrok,    2},
    {name::lcalda,    3},
    // Strings
    {name::kstnm,     0},
    {name::kevnm,     1},
    {name::khole,     2},
    {name::ko,        3},
    {name::ka,        4},
    {name::kt0,       5},
    {name::kt1,       6},
    {name::kt2,       7},
    {name::kt3,       8},
    {name::kt4,       9},
    {name::kt5,      10},
    {name::kt6,      11},
    {name::kt7,      12},
    {name::kt8,      13},
    {name::kt9,      14},
    {name::kf,       15},
    {name::kuser0,   16},
    {name::kuser1,   17},
    {name::kuser2,   18},
    {name::kcmpnm,   19},
    {name::knetwk,   20},
    {name::kdatrd,   21},
    {name::kinst,    22},
    // Data
    {name::data1,     0},
    {name::data2,     1}
};
class Trace {
public:
    explicit Trace();
    explicit Trace(const std::filesystem::path& path);
    void write(const std::filesystem::path& path, const bool legacy = false);
    void legacy_write(const std::filesystem::path& path);
    bool operator==(const Trace& other) const;
    // Convenience functions
    // Not implemented yet
    double frequency() const;
    std::string date() const;
    std::string time() const;
    void calc_gcarc();
    void calc_dist();
    // Getters
    // Floats
    float depmin() const;
    float depmax() const;
    float odelta() const;
    float resp0() const;
    float resp1() const;
    float resp2() const;
    float resp3() const;
    float resp4() const;
    float resp5() const;
    float resp6() const;
    float resp7() const;
    float resp8() const;
    float resp9() const;
    float stel() const;
    float stdp() const;
    float evel() const;
    float evdp() const;
    float mag() const;
    float user0() const;
    float user1() const;
    float user2() const;
    float user3() const;
    float user4() const;
    float user5() const;
    float user6() const;
    float user7() const;
    float user8() const;
    float user9() const;
    float dist() const;
    float az() const;
    float baz() const;
    float gcarc() const;
    float depmen() const;
    float cmpaz() const;
    float cmpinc() const;
    float xminimum() const;
    float xmaximum() const;
    float yminimum() const;
    float ymaximum() const;
    // Doubles
    double delta() const;
    double b() const;
    double e() const;
    double o() const;
    double a() const;
    double t0() const;
    double t1() const;
    double t2() const;
    double t3() const;
    double t4() const;
    double t5() const;
    double t6() const;
    double t7() const;
    double t8() const;
    double t9() const;
    double f() const;
    double stla() const;
    double stlo() const;
    double evla() const;
    double evlo() const;
    double sb() const;
    double sdelta() const;
    // Ints
    int nzyear() const;
    int nzjday() const;
    int nzhour() const;
    int nzmin() const;
    int nzsec() const;
    int nzmsec() const;
    int nvhdr() const;
    int norid() const;
    int nevid() const;
    int npts() const;
    int nsnpts() const;
    int nwfid() const;
    int nxsize() const;
    int nysize() const;
    int iftype() const;
    int idep() const;
    int iztype() const;
    int iinst() const;
    int istreg() const;
    int ievreg() const;
    int ievtyp() const;
    int iqual() const;
    int isynth() const;
    int imagtyp() const;
    int imagsrc() const;
    int ibody() const;
    // Bools
    bool leven() const;
    bool lpspol() const;
    bool lovrok() const;
    bool lcalda() const;
    // Strings
    std::string kstnm() const;
    std::string kevnm() const;
    std::string khole() const;
    std::string ko() const;
    std::string ka() const;
    std::string kt0() const;
    std::string kt1() const;
    std::string kt2() const;
    std::string kt3() const;
    std::string kt4() const;
    std::string kt5() const;
    std::string kt6() const;
    std::string kt7() const;
    std::string kt8() const;
    std::string kt9() const;
    std::string kf() const;
    std::string kuser0() const;
    std::string kuser1() const;
    std::string kuser2() const;
    std::string kcmpnm() const;
    std::string knetwk() const;
    std::string kdatrd() const;
    std::string kinst() const;
    // Data
    std::vector<double> data1() const;
    std::vector<double> data2() const;
    // Setters
    // Floats
    void depmin(const float x);
    void depmax(const float x);
    void odelta(const float x);
    void resp0(const float x);
    void resp1(const float x);
    void resp2(const float x);
    void resp3(const float x);
    void resp4(const float x);
    void resp5(const float x);
    void resp6(const float x);
    void resp7(const float x);
    void resp8(const float x);
    void resp9(const float x);
    void stel(const float x);
    void stdp(const float x);
    void evel(const float x);
    void evdp(const float x);
    void mag(const float x);
    void user0(const float x);
    void user1(const float x);
    void user2(const float x);
    void user3(const float x);
    void user4(const float x);
    void user5(const float x);
    void user6(const float x);
    void user7(const float x);
    void user8(const float x);
    void user9(const float x);
    void dist(const float x);
    void az(const float x);
    void baz(const float x);
    void gcarc(const float x);
    void depmen(const float x);
    void cmpaz(const float x);
    void cmpinc(const float x);
    void xminimum(const float x);
    void xmaximum(const float x);
    void yminimum(const float x);
    void ymaximum(const float x);
    // Doubles
    // Doubles
    void delta(const double x);
    void b(const double x);
    void e(const double x);
    void o(const double x);
    void a(const double x);
    void t0(const double x);
    void t1(const double x);
    void t2(const double x);
    void t3(const double x);
    void t4(const double x);
    void t5(const double x);
    void t6(const double x);
    void t7(const double x);
    void t8(const double x);
    void t9(const double x);
    void f(const double x);
    void stla(const double x);
    void stlo(const double x);
    void evla(const double x);
    void evlo(const double x);
    void sb(const double x);
    void sdelta(const double x);
    // Ints
    void nzyear(const int x);
    void nzjday(const int x);
    void nzhour(const int x);
    void nzmin(const int x);
    void nzsec(const int x);
    void nzmsec(const int x);
    void nvhdr(const int x);
    void norid(const int x);
    void nevid(const int x);
    void npts(const int x);
    void nsnpts(const int x);
    void nwfid(const int x);
    void nxsize(const int x);
    void nysize(const int x);
    void iftype(const int x);
    void idep(const int x);
    void iztype(const int x);
    void iinst(const int x);
    void istreg(const int x);
    void ievreg(const int x);
    void ievtyp(const int x);
    void iqual(const int x);
    void isynth(const int x);
    void imagtyp(const int x);
    void imagsrc(const int x);
    void ibody(const int x);
    // Bools
    void leven(const bool x);
    void lpspol(const bool x);
    void lovrok(const bool x);
    void lcalda(const bool x);
    // Strings
    void kstnm(const std::string& x);
    void kevnm(const std::string& x);
    void khole(const std::string& x);
    void ko(const std::string& x);
    void ka(const std::string& x);
    void kt0(const std::string& x);
    void kt1(const std::string& x);
    void kt2(const std::string& x);
    void kt3(const std::string& x);
    void kt4(const std::string& x);
    void kt5(const std::string& x);
    void kt6(const std::string& x);
    void kt7(const std::string& x);
    void kt8(const std::string& x);
    void kt9(const std::string& x);
    void kf(const std::string& x);
    void kuser0(const std::string& x);
    void kuser1(const std::string& x);
    void kuser2(const std::string& x);
    void kcmpnm(const std::string& x);
    void knetwk(const std::string& x);
    void kdatrd(const std::string& x);
    void kinst(const std::string& x);
    // Data
    void data1(const std::vector<double>& x);
    void data2(const std::vector<double>& x);
private:
    //cppcheck-suppress unusedStructMember
    std::array<float, 39> floats{};
    //cppcheck-suppress unusedStructMember
    std::array<double, 22> doubles{};
    //cppcheck-suppress unusedStructMember
    std::array<int, 26> ints{};
    //cppcheck-suppress unusedStructMember
    std::array<bool, 4> bools{};
    //cppcheck-suppress unusedStructMember
    std::array<std::string, 23> strings{};
    //cppcheck-suppress unusedStructMember
    std::array<std::vector<double>, 2> data{};
};
};
#endif
