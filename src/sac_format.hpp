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
#include <vector>
/* Boost string algorithms */
#include <boost/algorithm/string.hpp>

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
// This should work for two and four word string headers
const std::string unset_word{"-12345"};
using word_one = std::bitset<binary_word_size>;
using word_two = std::bitset<2 * binary_word_size>;
using word_four = std::bitset<4 * binary_word_size>;
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
class Trace {
public:
    //----------------------------------------------------------------------
    // Header
    //----------------------------------------------------------------------
    // Increment between evenly spaced samples [required]
    float f_delta{unset_float};
    // Minimum value of dependent variable
    // So Amplitude for time series
    // If spectrum, then amplitude or real component
    float depmin{unset_float};
    // Maximum value of dependent variable
    float depmax{unset_float};
    // Observed increment (if differs from delta)
    float odelta{unset_float};
    // Beginning value of independent variable [required]
    // First time value for time-series
    // First frequency for spectral data (f, or 2(pi)f?)
    float f_b{unset_float};
    // End value of independent variable [required]
    // Final time value for time-series
    // Final frequency for spectral data
    float f_e{unset_float};
    // Event origin time, seconds relative to reference time
    float f_o{unset_float};
    // First arrival time, seconds relative to reference time
    float f_a{unset_float};
    // User defined time picks for markers 0-9
    float f_t0{unset_float};
    float f_t1{unset_float};
    float f_t2{unset_float};
    float f_t3{unset_float};
    float f_t4{unset_float};
    float f_t5{unset_float};
    float f_t6{unset_float};
    float f_t7{unset_float};
    float f_t8{unset_float};
    float f_t9{unset_float};
    // End of event time, seconds relative to reference time
    float f_f{unset_float};
    // Instrument response parameters 0-9 [not used by SAC]
    float resp0{unset_float};
    float resp1{unset_float};
    float resp2{unset_float};
    float resp3{unset_float};
    float resp4{unset_float};
    float resp5{unset_float};
    float resp6{unset_float};
    float resp7{unset_float};
    float resp8{unset_float};
    float resp9{unset_float};
    // Station latitude, degrees, north positive
    float f_stla{unset_float};
    // Station longitude, degrees, east positive
    float f_stlo{unset_float};
    // Station elevation above sea-level, meters [not used by SAC]
    float stel{unset_float};
    // Station depth below surface, meters [not used by SAC]
    float stdp{unset_float};
    // Event latitude, degrees, north positive
    float f_evla{unset_float};
    // Event longitude, degrees, east positive
    float f_evlo{unset_float};
    // Event elevation above sea-level, meters [not used by SAC]
    float evel{unset_float};
    // Event depth below surface, kilometers [previously meters]
    float evdp{unset_float};
    // Event magnitude
    float mag{unset_float};
    // User defined variable storage
    float user0{unset_float};
    float user1{unset_float};
    float user2{unset_float};
    float user3{unset_float};
    float user4{unset_float};
    float user5{unset_float};
    float user6{unset_float};
    float user7{unset_float};
    float user8{unset_float};
    float user9{unset_float};
    // Station to event distance, kilometers
    float dist{unset_float};
    // Event to station azimuth, degrees
    float az{unset_float};
    // Station to event azimuth, degrees
    float baz{unset_float};
    // Station to event great circle arc length, degrees
    float gcarc{unset_float};
    // Original b (file), not same as in memory (SAC internal)
    float f_sb{unset_float};
    // Origin delta (file) not same as in memory (SAC internal)
    float f_sdelta{unset_float};
    // Mean value of dependent variable
    float depmen{unset_float};
    // Component azimuth, degrees clockwise from north
    float cmpaz{unset_float};
    // Component incident angle, degrees from upward vertical (incident 0 = -90 dip)
    // SEED/MINISEED use dip, degrees from horizontal down (dip 0 = incident 90)
    float cmpinc{unset_float};
    // Minimum value of X (spectral only)
    float xminimum{unset_float};
    // Maximum value of X (spectral only)
    float xmaximum{unset_float};
    // Minimum value of Y (spectral only)
    float yminimum{unset_float};
    // Maximum value of Y (spectral only)
    float ymaximum{unset_float};
    // GMT year corresponding to reference (zero) time
    int nzyear{unset_int};
    // GMT Day of the Year
    int nzjday{unset_int};
    // GMT Hour
    int nzhour{unset_int};
    // GMT Minute
    int nzmin{unset_int};
    // GMT Second
    int nzsec{unset_int};
    // GMT MilliSecond
    int nzmsec{unset_int};
    // Header version number
    // SAC 102.0+ auto-updates from 6 to 7 upon reading nowadays
    // The constructor for this class also updates from 6 to 7
    // though legacy writing is still supported
    // Going to use 7 as the default instead of unset
    int nvhdr{7};
    // Origin ID
    int norid{unset_int};
    // Event ID
    int nevid{unset_int};
    // Number of points per data component [required] (each component must be same size)
    int npts{unset_int};
    // Original NPTS (file), not same as in memory (SAC internal)
    int nsnpts{unset_int};
    // Waveform ID
    int nwfid{unset_int};
    // Spectral Length (Spectral only)
    int nxsize{unset_int};
    // Spectral Width (Spectral Only)
    int nysize{unset_int};
    // Type of file [required]
    // 01 = ITIME = Time-series
    // 02 = IRLIM = Spectral (real and imaginary)
    // 03 = IAMPH = Spectral (amplitude and phase)
    // 04 = IXY   = General X vs Y
    // UNTIL I HAVE THE CODE FOR IXYZ IT WILL NOT BE IMPLEMENTED!
    // ?? = IXYZ  = General XYZ (3D) file <- integer value not listed in the data format specification
    int iftype{unset_int};
    // Type of dependent variable
    // 05 = IUNKN  = Unknown
    // 06 = IDSIP  = Displacement (nm)
    // 07 = IVEL   = Velocity (nm/sec)
    // 08 = IACC   = Acceleration (nm/sec/sec)
    // 50 = IVOLTS = Velocity (volts)
    // Note other units may be used (like SI units)
    int idep{unset_int};
    // Reference time equivalent
    // 05 = IUNKN = Unknown
    // 09 = IB    = Begin time
    // 10 = IDAY  = Midnight of reference GMT day
    // 11 = IO    = Event origin time
    // 12 = IA    = First arrival time
    // XX = ITn   = User defined time pick (n = 0-9), (13-22)
    int iztype{unset_int};
    // Type of recording instrument [not used by SAC]
    int iinst{unset_int};
    // Station geographic region [not used by SAC]
    int istreg{unset_int};
    // Event geographic region [not used by SAC]
    int ievreg{unset_int};
    // Type of event
    // 05 = IUNKN  = Unknown
    // 37 = INUCL  = Nuclear
    // 38 = IPREN  = Nuclear pre-shot
    // 39 = IPOSTN = Nuclear post-shot
    // 40 = IQUAKE = Earthquake
    // 41 = IPREQ  = Foreshock
    // 42 = IPOSTQ = Aftershock
    // 43 = ICHEM  = Chemical explosion
    // 72 = IQB    = Quarry/mine blast, confirmed by quarry/mine
    // 73 = IQB1   = Quarry/mine blast with designed shot info-ripple fired
    // 74 = IQB2   = Quarry/mine blast with observed shot info-ripple fired
    // 75 = IQBX   = Quarry/mine blast, single shot
    // 76 = IQMT   = Quarry/mining induced events: tremor and rockbursts
    // 77 = IEQ    = Earthquake
    // 78 = IEQ1   = Earthquakes in a swarm or an aftershock sequence
    // 79 = IEQ2   = Felt earthquake
    // 80 = IME    = Marine explosion
    // 81 = IEX    = Other explosion
    // 82 = INU    = Nuclear explosion
    // 83 = INC    = Nuclear cavity collapse
    // 11 = IO     = Other source of known origin
    // 85 = IL     = Local event of unknown origin
    // 86 = IR     = Regional event of unknown origin
    // 87 = IT     = Teleseismic event of unknown origin
    // 88 = IU     = Undetermined or conflicting information
    // 44 = IOTHER = Other
    int ievtyp{unset_int};
    // Quality of data [not used by SAC]
    // 45 = IGOOD  = Good data
    // 46 = IGLCH  = Glitches
    // 47 = IDROP  = Dropouts
    // 48 = ILOWSN = Low signal to noise ratio
    // 44 = IOTHER = Other
    int iqual{unset_int};
    // Synthetic data flag [not used by SAC]
    // 49 = IRLDATA = Real data
    // ?? = Flags for various synthetic seismogram codes... (standardized or per person/group?)
    int isynth{unset_int};
    // Magnitude type
    // 52 = IMB = Body-wave magnitude
    // 53 = IMS = Surface-wave magnitude
    // 54 = IML = Local magnitude
    // 55 = IMW = Moment magnitude
    // 56 = IMD = Duration magnitude
    // 57 = IMX = User defined magnitude
    int imagtyp{unset_int};
    // Source of magnitude information
    // 58 = INEIC    = National Earthquake Information Center
    // 61 = IPDE     = Preliminary Determination of Epicenter
    // 62 = IISC     = Internation Seismological Centre
    // 63 = IREB     = Reviewed Event Bulletin
    // 64 = IUSGS    = US Geological Survey
    // 65 = IBRK     = UC Berkeley
    // 66 = ICALTECH = California Institute of Technology
    // 67 = ILLNL    = Lawrence Livermore National Laboratory
    // 68 = IEVLOC   = Event location (computer program)
    // 69 = IJSOP    = Joint Seismic Observation Program
    // 70 = IUSER    = The user (person running SAC)
    // 71 = IUNKNOWN = Unknown
    int imagsrc{unset_int};
    // Body/spheroid definition used in distance calculations
    // undef/-12345   = Earth: SAC historical spheroid version: a = 6378160.0m, f = 0.00335293 (this is NOT WGS84)
    //  98 = ISUN     = Sun: a = 696000000.0 m, f = 8.189e-6
    //  99 = IMERCURY = Mercury: a = 2439700.0 m, f = 0.0
    // 100 = IVENUS   = Venus: a = 6051800.0 m, f = 0.0
    //      Note: Looking into WGS84 on Wikipedia: a is semi-major axis (equatorial), f is 'inverse flattening'
    //       ((a-c) / a), it is the square of the eccentricity
    // 101 = IEARTH   = Earth: a = 6378137.0 m, f = 1.0 / 298.257223563 ( = 0.0033528106647474805) (this IS WGS84)
    // 102 = IMOON    = Moon: a = 1737400.0 m, f = 0.0
    // 103 = IMARS    = Mars: a = 3396190.0 m, f = 1.0 / 169.89444722361179 ( = 0.005886007555525457)
    int ibody{unset_int};
    // True if data is evenly spaced [required]
    bool leven{unset_bool};
    // True if station has positive-polarity (left-hand rule [e.g. NEZ {North-East-Up}])
    bool lpspol{unset_bool};
    // True if okay to overwrite file
    bool lovrok{unset_bool};
    // True if dist, az, baz, gcarc are to be calculated from station and event coordinates
    bool lcalda{unset_bool};
    // Station name
    std::string kstnm{unset_word};
    // Event name (the only 4 four word string variable)
    std::string kevnm{unset_word};
    // Nuclear: hole identifier; Other: Location identifier (LOCID)
    std::string khole{unset_word};
    // Event origin time identification (text for o)
    std::string ko{unset_word};
    // First arrival time identification (text for a)
    std::string ka{unset_word};
    // User defined pick identifiers (text for t0-t9)
    std::string kt0{unset_word};
    std::string kt1{unset_word};
    std::string kt2{unset_word};
    std::string kt3{unset_word};
    std::string kt4{unset_word};
    std::string kt5{unset_word};
    std::string kt6{unset_word};
    std::string kt7{unset_word};
    std::string kt8{unset_word};
    std::string kt9{unset_word};
    // Fini identification (text for f)
    std::string kf{unset_word};
    // User defined variable storage area (text, n=0-2)
    std::string kuser0{unset_word};
    std::string kuser1{unset_word};
    std::string kuser2{unset_word};
    // Channel Name
    // SEED volumes use 3 coded characters (e.g. BHZ)
    // First is instrument type (B = broadband, H = higher-frequency broadband)
    // Second is related to sampling rate (H = high sampling rate, V = very low sampling rate)
    // Third is direction (Z = up, E = East, N = North, R = Radial, T = Transverse)
    // Current trend is to prefer 1/2 over N/E (in case they're not correctly aligned?)
    std::string kcmpnm{unset_word};
    // Name of seismic network
    std::string knetwk{unset_word};
    // Date data was read onto computer
    std::string kdatrd{unset_word};
    // Generic name of recording instrument
    std::string kinst{unset_word};
    //----------------------------------------------------------------------
    // Data
    //----------------------------------------------------------------------
    // data1 is the first data chunk in the SAC-file
    // It is ALWAYS present
    // SAC format uses single-precision, we convert to double for computing
    // and back to single for writing out (to remain compatible with the standard)
    std::vector<double> data1{};
    // data2 is the second data chunk in the SAC-file
    // It is CONDITIONALLY present
    std::vector<double> data2{};
    //----------------------------------------------------------------------
    // Footer (if nvhdr = 7)
    //----------------------------------------------------------------------
    // These are double-precision versions of float headers
    // Giving them default unset_double value to be consistent with all default values
    // I prefixed all the single-precision header equivalents with `f_` to denote they
    // are floats instead of doubles (`d_` for double would've been annoying as the
    // doubles will ALWAYS be preferred; it isn't 1980 anymore)
    //
    // double precison version of delta
    double delta{unset_double};
    // double-precision version of b
     double b{unset_double};
    // double-precision version of e
    double e{unset_double};
    // double-precision version of o
    double o{unset_double};
    // double-precision version of a
    double a{unset_double};
    // double-precision version of t0-t9
    double t0{unset_double};
    double t1{unset_double};
    double t2{unset_double};
    double t3{unset_double};
    double t4{unset_double};
    double t5{unset_double};
    double t6{unset_double};
    double t7{unset_double};
    double t8{unset_double};
    double t9{unset_double};
    // double-precision version of f
    double f{unset_double};
    // double-precision version of evlo
    double evlo{unset_double};
    // double-precision version of evla
    double evla{unset_double};
    // double-precision version of stlo
    double stlo{unset_double};
    // double-precision version of stla
    double stla{unset_double};
    // double-precision version of sb
    double sb{unset_double};
    // double-precision version of sdelta
    double sdelta{unset_double};
    //----------------------------------------------------------------------
    // Convenience methods
    //----------------------------------------------------------------------
    // Take the single-precision values from the header and insert them into the
    // appropriate footer variables as double-precision values
    void header_to_footer();
    // Take the double-precision values from the footer and insert them into the
    // appropriate header variables as single-precision floats
    // NOTE: this results in a loss of precision
    void footer_to_header();
    //----------------------------------------------------------------------
    // Constructors
    //----------------------------------------------------------------------
    // Copy constructor is unnecessary as the compiler will default
    // to do member-wise copying (only needed for special cases, not here)
    // Parameterized constructor (reader)
    explicit Trace(const std::filesystem::path& file_name);
    // Empty constructor
    Trace() = default;
    //----------------------------------------------------------------------
    // Writing
    //----------------------------------------------------------------------
    // Allows writing out to binary sac file format
    // Note that if the data was read using this class
    // it is automatically converted to nvhdr = v7
    // Some software doesn't like that (ObsPy seems to dislike it)
    void write(const std::filesystem::path& file_name);
    // legacy_write writes as nvhdr = v6 for compatibility
    void legacy_write(const std::filesystem::path& file_name);
    //----------------------------------------------------------------------
    // Overload equality check
    bool operator==(const Trace& other) const;
    bool equal_within_tolerance(const std::vector<double>& vector1,
                                const std::vector<double>& vector2,
                                const double tolerance =
                                std::numeric_limits<float>::epsilon()) const;
};
}
#endif
